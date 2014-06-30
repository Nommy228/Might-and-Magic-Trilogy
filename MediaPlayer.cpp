extern "C"
{
  #include "lib/libavcodec/avcodec.h"
  #include "lib/libavformat/avformat.h"
  #include "lib/libavutil/avutil.h"
  #include "lib/libavutil/imgutils.h"
  #include "lib/libswscale/swscale.h"
  #include "lib/libswresample/swresample.h"
  #include "lib/libavutil/opt.h"
}
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "swresample.lib")

#include <vector>
#include <deque>

#include "stuff.h"
#include "OpenALSoundProvider.h"

#include "MediaPlayer.h"
using namespace Media;

Media::MPlayer *pMediaPlayer;
Media::IMovie *pMovie;
Media::ITrack *pTrack;
Movie *movie;

int mSourceID;

void PlayMovie(const wchar_t * pFilename);
void PlayAudio(const wchar_t * pFilename);
void LoadMovie(const char *);
 
class MemoryStream
{
  public:
    inline MemoryStream(void *data, size_t data_size)
    {
      this->data_size = data_size;
      this->data = data;
      this->current_pos = 0;
    }
    inline MemoryStream()
    {
      this->data_size = 0;
      this->data = nullptr;
      this->current_pos = 0;
    }

    inline ~MemoryStream()
    {
      if (data)
        delete [] data;
    }

    inline size_t Write(void *buffer, size_t num_bytes)
    {
      if (!data)
      {
        data_size = 32 + num_bytes;
        data = new char[data_size];
        current_pos = 0;
      }
      else if (current_pos + num_bytes >= data_size)
      {
        int  new_data_size = data_size + num_bytes + data_size / 8 + 4;
        auto new_data = new char[new_data_size];

        memcpy(new_data, data, data_size);
        delete [] data;
        
        data_size = new_data_size;
        data = new_data;
      }
      memcpy((char *)data + current_pos, buffer, num_bytes);
      current_pos += num_bytes;
      return num_bytes;
    }

    inline size_t Read(void *buffer, size_t num_bytes)
    {
      size_t read_size = min(num_bytes, data_size - current_pos);
      if (read_size)
      {
        memcpy(buffer, (char *)data + current_pos, read_size);
        current_pos += read_size;
      }
      return read_size;
    }

    inline void Reset()
    {
      current_pos = 0;
    }
    inline void SeekToEnd()
    {
      current_pos = data_size;
    }

    inline size_t Unwind(size_t bytes)//зациклить???
    {
      if (bytes > current_pos)
        current_pos = 0;
      else
        current_pos -= bytes;
      return current_pos;
    }
    
    inline size_t Rewind(size_t bytes)
    {
      if (current_pos + bytes >= data_size)
        current_pos = data_size;
      else
        current_pos += bytes;
      return current_pos;
    }

    inline size_t  Size() const    {return data_size;}
    inline size_t  Current() const {return current_pos;}
    inline void   *Ptr() const     {return data;}

  private:
    void   *data;
    size_t  data_size;
    size_t  current_pos;
};

bool end_current_file;
bool loop_current_file;
DWORD time_video_begin;
int current_movie_width;
int current_movie_height;

OpenALSoundProvider *provider = nullptr;

static int av_num_bytes_per_sample(AVSampleFormat sample_fmt)
{
  switch (sample_fmt)
  {
    case AV_SAMPLE_FMT_U8:
    case AV_SAMPLE_FMT_U8P:
      return 1;
          
    case AV_SAMPLE_FMT_S16:
    case AV_SAMPLE_FMT_S16P:
      return 2;

    case AV_SAMPLE_FMT_S32:
    case AV_SAMPLE_FMT_S32P:
    case AV_SAMPLE_FMT_FLT:
    case AV_SAMPLE_FMT_FLTP:
      return 4;

    case AV_SAMPLE_FMT_DBL:
    case AV_SAMPLE_FMT_DBLP:
      return 8;

    default:
    case AV_SAMPLE_FMT_NONE:
      Error("Invalid av sample format: %u", sample_fmt);
  }
  return 0;
}

struct AVStreamWrapper
{
  inline AVStreamWrapper()
  {
    this->type = AVMEDIA_TYPE_UNKNOWN;
    this->stream_idx = -1;
    this->stream = nullptr;
    this->dec = nullptr;
    this->dec_ctx = nullptr;
  }

  inline void Release()
  {
    type = AVMEDIA_TYPE_UNKNOWN;
    stream_idx = -1;
    stream = nullptr;
    dec = nullptr;
    if (dec_ctx)
    {
	  // Close the codec
      // закрытие видео кодека
      avcodec_close(dec_ctx);
      dec_ctx = nullptr;
    }
  }

  AVMediaType      type;
  int              stream_idx;
  AVStream        *stream;
  AVCodec         *dec;
  AVCodecContext  *dec_ctx;
};

struct AVAudioStream: public AVStreamWrapper
{
  inline AVAudioStream():
    AVStreamWrapper()
  {
    this->bytes_per_sample = 0;
    this->bytes_per_second = 0;
  }

  int bytes_per_sample;
  int bytes_per_second;
};

struct AVVideoStream: public AVStreamWrapper
{
  inline AVVideoStream():
    AVStreamWrapper()
  {
    this->frames_per_second = 0.0;
  }

  double frames_per_second;
};

static bool av_open_stream(AVFormatContext *format_ctx, AVMediaType type, AVStreamWrapper *out_stream)
{
  // найдем первый stream
  int stream_idx = av_find_best_stream(format_ctx, type, -1, -1, nullptr, 0);
  if (stream_idx >= 0)
  {
    auto stream = format_ctx->streams[stream_idx];
    //Информация о кодеке в потоке называется «контекстом кодека» (AVCodecContext).
    //Используя эту информацию, мы можем найти необходимый кодек (AVCodec) и открыть его.
	// получаемм кодек
    auto dec_ctx = stream->codec;

	// Find the decoder for the video stream
    // ищем декодер
    auto dec = avcodec_find_decoder(dec_ctx->codec_id);
    if (dec)
    {
	  // Open codec
      // открываем кодек
      if (avcodec_open2(dec_ctx, dec, nullptr) >= 0)
      {
        out_stream->type = type;
        out_stream->stream_idx = stream_idx;
        out_stream->stream = stream;
        out_stream->dec = dec;
        out_stream->dec_ctx = dec_ctx;
        return true;
      }
      fprintf(stderr, "ffmpeg: Could not open codec\n");// Не открылся кодек
	  return false;
    }
    fprintf(stderr, "ffmpeg: Unable to open codec\n");// Кодек не найден
	return false;
  }
  fprintf(stderr, "ffmpeg: Didn't find a stream\n");// Не найден stream
  return false;	
}

static bool av_open_audio_stream(AVFormatContext *format_ctx, AVAudioStream *out_stream)
{
  if (!av_open_stream(format_ctx, AVMEDIA_TYPE_AUDIO, out_stream))
    return Error("Audio stream not found"), false;
  
  // we support only 2-channel audio for now
  //if (out_stream->dec_ctx->channels != 2)//закомментировал потому что при воспроизведении jvc.bik вылетает на этом месте
  //{
   // out_stream->Release();
   // return Error("Unsupported number of channels: %u", out_stream->dec_ctx->channels), false;
  //}
  
  out_stream->bytes_per_sample = av_num_bytes_per_sample(out_stream->dec_ctx->sample_fmt);
  out_stream->bytes_per_second = out_stream->dec_ctx->channels * out_stream->dec_ctx->sample_rate * out_stream->bytes_per_sample;

  return true;
}

static bool av_open_video_stream(AVFormatContext *format_ctx, AVVideoStream *out_stream)
{
  if (!av_open_stream(format_ctx, AVMEDIA_TYPE_VIDEO, out_stream))
    return Error("Video stream not found"), false;

  out_stream->frames_per_second = (double)out_stream->dec_ctx->time_base.den / (double)out_stream->dec_ctx->time_base.num;
  return true;
}

void InterleaveAudioData(MemoryStream *stream, AVSampleFormat src_format, int num_channels, int num_samples, uint8_t **channels)
{
  unsigned int bytes_per_sample;
  switch (src_format)
  {
    case AV_SAMPLE_FMT_U8:
    case AV_SAMPLE_FMT_U8P:
      __debugbreak();

    case AV_SAMPLE_FMT_S16:
      bytes_per_sample = sizeof(__int16);
      stream->Write(channels[0], num_channels * num_samples * bytes_per_sample);
    break;

    case AV_SAMPLE_FMT_S16P:
    {
      bytes_per_sample = sizeof(__int16);
      for (int i = 0; i < num_samples; ++i)
        for (int j = 0; j < num_channels; ++j)
          stream->Write(channels[j] + i * bytes_per_sample, bytes_per_sample);
    }
    break;

    case AV_SAMPLE_FMT_FLT:
    {
      SwrContext *converter = swr_alloc();
      av_opt_set_int(converter, "in_channel_layout",    av_get_default_channel_layout(2), 0);
      //av_opt_set_int(converter, "in_sample_rate",       sample_ra, 0);
      av_opt_set_sample_fmt(converter, "in_sample_fmt", AV_SAMPLE_FMT_FLT, 0);

      av_opt_set_int(converter, "out_channel_layout",    av_get_default_channel_layout(2), 0);
      //av_opt_set_int(converter, "out_sample_rate",       dst_sample_rate, 0);
      av_opt_set_sample_fmt(converter, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);

      if (swr_init(converter) < 0)
      {
        __debugbreak();
        swr_free(&converter);
        return;
      }

      uint8_t **dst_channels;
      int       dst_linesize[8];
      //int dst_nb_channels = av_get_channel_layout_nb_channels(dst_channel_layout);
      if (av_samples_alloc_array_and_samples(&dst_channels, dst_linesize, 2, num_channels * num_samples, AV_SAMPLE_FMT_S16, 0) < 0)
      {
        __debugbreak();
        swr_free(&converter);
        return;
      }

      if (swr_convert(converter, dst_channels, num_channels * num_samples, (const uint8_t **)channels, num_channels * num_samples) >= 0)
        stream->Write(dst_channels[0], num_channels * num_samples * sizeof(__int16));
      else
        __debugbreak();

      av_free(dst_channels[0]);
      swr_free(&converter);
    }
    break;

    default:
      __debugbreak();
      //if (Resample(next_frame->avframe, next_frame->avframe->channel_layout, next_frame->avframe->sample_rate,
      //                                            av_get_default_channel_layout(2),    next_frame->avframe->sample_rate, AV_SAMPLE_FMT_S16P, resampled_data))
  }
}

const uint16_t ff_wma_critical_freqs[25] = {
    100,   200,  300, 400,   510,  630,  770,    920,
    1080, 1270, 1480, 1720, 2000, 2320, 2700,   3150,
    3700, 4400, 5300, 6400, 7700, 9500, 12000, 15500,
    24500,
};
extern const uint16_t ff_wma_critical_freqs[25];
static float quant_table[96];

bool DecodeAudioFrame(AVCodecContext *dec_ctx, AVPacket *avpacket, AVFrame *avframe, MemoryStream *out_audio_data, int *out_num_audio_samples)
{
  volatile int decoded = false;
  do
  {
    //Декодирование аудио-пакета осуществляется функцией avcodec_decode_audio4
    if (avcodec_decode_audio4(dec_ctx, avframe, (int *)&decoded, avpacket) < 0)
    {
      log("Cannot decode audio frame\n");
      return false;
    }

    if (!decoded)
      log("Cannot decode audio frame in one piece\n");
  } while (!decoded);

  switch (dec_ctx->codec_id)
  {
    case  AV_CODEC_ID_BINKAUDIO_DCT:
    {
      __debugbreak();	
    }
    case AV_CODEC_ID_BINKAUDIO_RDFT:
    {//pts	samples		dpts
     //    0	960
     //17280	960		17280    18x960
     //18240	960		960       1x960
     //20160	960		1920      2x960
     //21120	960		960       1x960
     //23040	960		1920      2x960
      /*static int bink_next_pts = 0;

        // there's a gap in the sound - fill empty samples in
      if (bink_next_pts < avpacket->pts)
      {
        short silence[1024];
        memset(silence, 0, sizeof(silence));

        int samples_to_fill = /*dec_ctx->channels *  (avpacket->pts - bink_next_pts);
        while (samples_to_fill > 0)
        {
          int samples_to_fill_this_step = samples_to_fill >= 1024 ? 1024 : samples_to_fill;
          out_audio_data->Write(silence, samples_to_fill_this_step  * sizeof(short));

          samples_to_fill -= samples_to_fill_this_step;
        }
      }

      bink_next_pts = avpacket->pts + /*dec_ctx->channels *  avframe->nb_samples; */

  AVFrame frame;
  int first;
  int version_b;
  int frame_len;
  int overlap_len;        
  int block_size;
  int num_bands;
  unsigned int *bands;
  float root;
  int sample_rate = dec_ctx->sample_rate;
  int sample_rate_half;
  int i;
  int frame_len_bits;
  int channels;

  //compresses audio in chunks of varying sizes depending on sample rate:
   // if sample rate < 22050, frame size is 2048 samples
   // if sample rate < 44100, frame size is 4096 samples
   // else, frame size is 8192 samples

 //сжимает аудио в куски различных размеров в зависимости от частоты дискретизации:
   //если частота дискретизации < 22050, размер кадра составляет 2048 самплов
   //если частота дискретизации < 44100, размер кадра составляет 4096 самплов
   //или, размер кадра составляет 8192 самплов

  /* determine frame length */
  if (dec_ctx->sample_rate < 22050)
    frame_len_bits = 9;
  else if (dec_ctx->sample_rate < 44100)
    frame_len_bits = 10;
  else
    frame_len_bits = 11;

  //проверка количества каналов (не меньше 1 и не больше 2)
  if (dec_ctx->channels < 1 || dec_ctx->channels > 2) 
  {
        av_log(dec_ctx, AV_LOG_ERROR, "invalid number of channels: %d\n", dec_ctx->channels);
        return AVERROR_INVALIDDATA;
  }

	  version_b = dec_ctx->extradata_size >= 4 && dec_ctx->extradata[3] == 'b';
      if (version_b)
	    __debugbreak();

      if (dec_ctx->codec->id == AV_CODEC_ID_BINKAUDIO_RDFT)
      {
        // audio is already interleaved for the RDFT format variant
        dec_ctx->sample_fmt = AV_SAMPLE_FMT_FLT;
        sample_rate  *= dec_ctx->channels;
        channels = 1;
        if (!version_b)
          frame_len_bits += av_log2(dec_ctx->channels);
      }
      else
      {
        channels = dec_ctx->channels;
        dec_ctx->sample_fmt = AV_SAMPLE_FMT_FLTP;
      }

	  frame_len     = 1 << frame_len_bits;                          //2048

	  //a frame is windowed with the previous frame; the size of the window is frame size / 16 
	  //кадр оконный с предыдущего кадра;Размер окна = размер кадра / 16
      overlap_len   = frame_len / 16;                               //128
      block_size    = (frame_len - overlap_len) * channels;         //1920

	  //compute half the sample rate as (sample rate + 1) / 2;
	  //initialize an array of band frequencies corresponding to an array of 25 critical frequencies (same as WMA, apparently),
	  // any for which the critical frequencies are less than half the sample rate 

	  //вычислить половину частоты дискретизации(частота дискретизации + 1) / 2;
	  //инициализировать массив группы частот, соответствующих массиву 25 критических частот (аналогично WMA, очевидно),
	  // любой, для которых критические частоты в два раза меньше частота дискретизации
      sample_rate_half = (sample_rate + 1) / 2;	                    //22050
      if (dec_ctx->codec->id == AV_CODEC_ID_BINKAUDIO_RDFT)
         root = 2.0 / (sqrt(float(frame_len)) * 32768.0);
      else
         root = frame_len / (sqrt(float(frame_len)) * 32768.0);
      for (i = 0; i < 96; i++) 
	  {
         /* constant is result of 0.066399999/log10(M_E) */
        quant_table[i] = expf(i * 0.15289164787221953823f) * root;
      }
 
       /* calculate number of bands */
	  //bands calculation:
	  //bands[0] = 1;
	  //foreach (i in 1..# of bands-1):
	  //bands[i] = crit_freq[i-1] * (frame length / 2) / (sample rate / 2); 
	  //bands[# of bands] = frame length / 2 
       for (num_bands = 1; num_bands < 25; num_bands++)
         if (sample_rate_half <= ff_wma_critical_freqs[num_bands - 1])
            break;

       bands = (unsigned int *)(av_malloc((num_bands + 1) * sizeof(*bands)));
       if (!bands)
         return AVERROR(ENOMEM);

	   /* populate bands data */
       bands[0] = 2;
       for (i = 1; i < num_bands; i++)
         bands[i] = (ff_wma_critical_freqs[i - 1] * frame_len / sample_rate_half) & ~1;
       bands[num_bands] = frame_len;

       first = 1;

       //ff_rdft_init(&trans.rdft, frame_len_bits, DFT_C2R);

       avcodec_get_frame_defaults(&frame);
       dec_ctx->coded_frame = &frame;
    }
    break;
                /*
      case AV_CODEC_ID_SMACKAUDIO:
      {
        static int smack_debug_next_audio_time = 0;
        if (smack_debug_next_audio_time != packet->pts)
        {
          Error("There's a gap in the sound before frame %u\n", num_audio_frames);
          __debugbreak(); // there's a gap in the sound
        }

        int num_actual_data_channels = 0;
        switch (dec_ctx->sample_fmt)
        {
          case AV_SAMPLE_FMT_U8:
          case AV_SAMPLE_FMT_S16:
          case AV_SAMPLE_FMT_S32:
          case AV_SAMPLE_FMT_FLT:
          case AV_SAMPLE_FMT_DBL:
            num_actual_data_channels = 1;
          break;

          case AV_SAMPLE_FMT_U8P:
          case AV_SAMPLE_FMT_S16P:
          case AV_SAMPLE_FMT_S32P:
          case AV_SAMPLE_FMT_FLTP:
          case AV_SAMPLE_FMT_DBLP:
            num_actual_data_channels = dec_ctx->channels;
          break;

          default:
          case AV_SAMPLE_FMT_NONE:
          case AV_SAMPLE_FMT_NB:
            __debugbreak();
        }

        smack_debug_next_audio_time += dec_ctx->channels * frame->nb_samples * bytes_per_sample;
        Assert(frame->avframe->linesize[0] == audio.dec_ctx->channels * frame->avframe->nb_samples * audio.bytes_per_sample / num_actual_data_channels,
               "Smack audio size mismatch in frame %u in %s\n", audio_num_read_frames, movie_filename);

        frame->play_time = (double)frame->avpacket->pts / (double)audio.bytes_per_second;
      }
      break;

                case AV_CODEC_ID_MP3:
                {
                  static int mp3_samples_decoded_so_far = 0;
                  static int mp3_prev_samples_count = frame->avframe->nb_samples; // mp3 seems to always feed same amount of samples
                  frame->play_time = (double)mp3_samples_decoded_so_far / (double)audio.dec_ctx->sample_rate;

                  mp3_samples_decoded_so_far += frame->avframe->nb_samples;
                  Assert(mp3_prev_samples_count == frame->avframe->nb_samples,
                          "MP3 audio have variable sample count in frame %u in %s\n", audio_num_read_frames, movie_filename);
                }
                break;

                default:
                {
                  __debugbreak();
                  double samples_per_second = (double)audio.dec_ctx->time_base.den / (double)audio.dec_ctx->time_base.num;
                  double play_length = frame->avframe->nb_samples / samples_per_second;
                  frame->play_time = (double)frame->avpacket->pts / samples_per_second;
                }
                break;*/
  }

  if (!avframe->channel_layout)
  {
    log("Audio channel layout not specified, rolling back to default\n");
    avframe->channel_layout = av_get_default_channel_layout(dec_ctx->channels);
  }

  *out_num_audio_samples = dec_ctx->channels * avframe->nb_samples;
  InterleaveAudioData(out_audio_data, dec_ctx->sample_fmt,
                      dec_ctx->channels, avframe->nb_samples, avframe->data);
  return true;
}

bool LoadAudioTrack(AVFormatContext *format_ctx, AVCodecContext *dec_ctx, int audio_stream_idx, MemoryStream *out_audio_stream, int *out_num_audio_frames, int *out_num_audio_samples)
{
  out_audio_stream->Reset();

  //Чтение аудио данных.
  //Данные из файла читаются пакетами (AVPacket), а для воспроизведения используется фрейм (AVFrame).

  //Выделим память для фрейма
  AVFrame  *frame = avcodec_alloc_frame();

  AVPacket *packet = new AVPacket;
  av_init_packet(packet);

  int num_audio_frames = 0;
  int num_audio_samples = 0;

  //чтение пакетов
  while (av_read_frame(format_ctx, packet) >= 0)
  {
	// Is this a packet from the audio stream?
    //Принадлежит ли пакет к аудиопотоку
    if (packet->stream_index != audio_stream_idx)
    {
      //log("Suspicious stream id %u in %s", packet->stream_index, filenamea);
      continue;
    }

	// Decode audio frame
	//Декодируем аудио фрейм
    int num_samples_decoded;
    DecodeAudioFrame(dec_ctx, packet, frame, out_audio_stream, &num_samples_decoded);

    num_audio_samples += num_samples_decoded;
    num_audio_frames++;
  }
  *out_num_audio_frames = num_audio_frames;
  *out_num_audio_samples = num_audio_samples;

  //Освободить память выделенную для фрейма
  avcodec_free_frame(&frame);
  delete frame;
  av_free_packet(packet);
  delete packet;

  return true;
}

class Track: public Media::ITrack
{
  public:
    inline Track()
    {
      this->format_ctx = nullptr;
      this->audio_num_samples = 0;
    }

    void Release()
    {
      ReleaseAvcodec();
    }

    void ReleaseAvcodec()
    {
      audio.Release();
      if (format_ctx)
      {
        // закрытия контекста файла
        av_close_input_file(format_ctx);
        format_ctx = nullptr;
      }
    }

    bool LoadAudio(const wchar_t *filename) //Загрузка из папки	для mp3
    {
      char filenamea[1024];
      sprintf(filenamea, "%S", filename);
      // Open audio file
      //откроем входной файл(Шаг 2)
      //Функция avformat_open_input читает файловый заголовок и сохраняет информацию о найденных форматах в структуре
      //AVFormatContext. Остальные аргументы могут быть установлены в NULL, в этом случае libavformat использует 
      //автоматическое определение параметров.
      if (avformat_open_input(&format_ctx, filenamea, nullptr, nullptr) >= 0)
      {
        // Retrieve stream information
        //Т.к. avformat_open_input читает только заголовок файла, то следующим шагом нужно получить информацию о потоках
        //в файле. Это делается функцией avformat_find_stream_info.
        if (avformat_find_stream_info(format_ctx, nullptr) >= 0)
        {
          // Dump information about file onto standard error
          //После этого format_context->streams содержит все существующие потоки файла. 
          //Их количество равно format_context->nb_streams.
          //Вывести подробную информацию о файле и обо всех потоках можно функцией av_dump_format.
          av_dump_format(format_ctx, 0, filenamea, 0);

		  //Открыть поток
          if (!av_open_audio_stream(format_ctx, &audio))
          {
            Error("Cannot open strack: %s", filenamea);
            return Release(), false;
          }
          
          MemoryStream audio_plain_data;
          int          num_audio_frames;
          int          num_audio_samples;

		  //Загрузить аудио трек
          if (LoadAudioTrack(format_ctx, audio.dec_ctx, audio.stream_idx, &audio_plain_data, &num_audio_frames, &num_audio_samples))
          {
            /*#ifdef _DEBUG
              char debug_filename[1024];
              sprintf(debug_filename, "%s.wav", filenamea);
              FILE *wav = fopen(debug_filename, "w+b");

              extern void write_wav_header(FILE *wav, int channel_count = 2, int sample_rate = 22050, int bytes_per_sample = 2);
              write_wav_header(wav, audio.dec_ctx->channels, audio.dec_ctx->sample_rate, audio.bytes_per_sample);

              fwrite(audio_plain_data.Ptr(), audio_plain_data.Current(), 1, wav);
            
              extern void fix_wav_header(FILE *wav, int wav_bytes_in_stream);
              fix_wav_header(wav, audio_plain_data.Current());
            #endif*/

            device_buffer = provider->CreateTrack16(audio.dec_ctx->channels, audio.dec_ctx->sample_rate, 2, num_audio_samples, audio_plain_data.Ptr());

            Release();
            return true;
          }
        }
        Release();
        fprintf(stderr, "ffmpeg: Unable to find stream info\n"); //Не найден поток
        return false;
      }
      fprintf(stderr, "ffmpeg: Unable to open input file\n"); //Не может открыть файл
      return false;
    }

    virtual void Play(bool loop)
    {
      provider->PlayTrack16(device_buffer, loop);
      mSourceID = device_buffer->source_id;
    }
  
  protected:
    AVFormatContext *format_ctx;
    AVAudioStream    audio;
    int              audio_num_samples;

    bool             stopped;

    OpenALSoundProvider::TrackBuffer *device_buffer;
};

class Movie: public Media::IMovie
{
  public:
    inline Movie()
    {
      this->movie_filename[0] = 0;
      this->width = 0;
      this->height = 0;
      this->format_ctx = nullptr;
      this->end_of_file = false;
      this->playback_time = 0.0;

      this->num_audio_frames = 0;
      this->num_audio_samples = 0;

      this->last_resampled_frame_num = -1;
      memset(last_resampled_frame_data, 0, sizeof(last_resampled_frame_data));
      memset(last_resampled_frame_linesize, 0, sizeof(last_resampled_frame_linesize));

      decoding_packet = nullptr;
		ioBuffer = nullptr;
		format_ctx = nullptr;
		avioContext = nullptr;
    }
 
    inline void Release()
    {
      ReleaseAVCodec();
    }

    inline void ReleaseAVCodec()
    {
      audio.Release();
      video.Release();

      if (format_ctx)
      {
		// Close the video file
        // закрытие контекста файла(видео файла)
        av_close_input_file(format_ctx);
        format_ctx = nullptr;
	  }
	  if(avioContext)
	  {
		av_free(avioContext);
		avioContext = nullptr;
	  }
	  if (ioBuffer)
	  {
		//av_free(ioBuffer);
		ioBuffer = nullptr;
      }
	  av_free_packet(decoding_packet);
	  delete decoding_packet;
      avcodec_free_frame(&decoding_frame);
	  delete decoding_frame;
      if (last_resampled_frame_data[0])
        av_freep(&last_resampled_frame_data[0]);

    }

    bool Load(const wchar_t *filename, int dst_width, int dst_height, int cache_ms)	//Загрузка
    {
      char filenamea[1024];
      sprintf(filenamea, "%S", filename);
      sprintf(movie_filename, "%S", filename);

      width = dst_width;
      height = dst_height;
      // Open video file
      //откроем входной файл(Шаг 2)
      //Функция avformat_open_input читает файловый заголовок и сохраняет информацию о найденных форматах в структуре
      //AVFormatContext. Остальные аргументы могут быть установлены в NULL, в этом случае libavformat использует 
      //автоматическое определение параметров.  Последние 2 аргумента используются для формата файла и опций.
      if (avformat_open_input(&format_ctx, filenamea, nullptr, nullptr) >= 0)
      {
        // Retrieve stream information
        // Проверяем потоки
        //Т.к. avformat_open_input читает только заголовок файла, то следующим шагом нужно получить информацию о потоках
        //в файле. Это делается функцией avformat_find_stream_info.(Шаг 3)
        if (avformat_find_stream_info(format_ctx, nullptr) >= 0)
        {
          // Dump information about file onto standard error
          //Инициализируем pFormatCtx->streams
          //После этого format_context->streams содержит все существующие потоки файла. 
          //Их количество равно format_context->nb_streams.
          //Вывести подробную информацию о файле и обо всех потоках можно функцией av_dump_format.
          av_dump_format(format_ctx, 0, filenamea, 0);

          //pFormatCtx->streams - массив указателей, размера pFormatCtx->nb_streams, поищем тут потоки.
          if (!av_open_audio_stream(format_ctx, &audio))
          {
            Error("Cannot open audio stream: %s", filenamea);
            return Release(), false;
          }
          
          if (!av_open_video_stream(format_ctx, &video))
          {
            Error("Cannot open video stream: %s", filenamea);
            return Release(), false;
          }

          //Ritor1: include 
		  if (_stricmp("binkvideo", video.dec->name) ) 
		  {
			current_movie_width = video.dec_ctx->width;
			current_movie_height = video.dec_ctx->height;
		  }
		  else
		  {
			current_movie_width = width;
			current_movie_height = height;
	      } 
		  //
          decoding_packet = new AVPacket;
          av_init_packet(decoding_packet);
      
		  // Allocate video frame
		  //Выделим память для фрейма
          decoding_frame = avcodec_alloc_frame();

          audio_data_in_device = provider->CreateStreamingTrack16(audio.dec_ctx->channels, audio.dec_ctx->sample_rate, 2);

          return true;
        }
        fprintf(stderr, "ffmpeg: Unable to find stream info\n");
        return Release(), false; // Не найдена информация о потоке
      }
      fprintf(stderr, "ffmpeg: Unable to open input file\n");
      return Release(), false; // Не может открыть файл
    }

	bool LoadFromLOD(HANDLE h, int readFunction(void*, uint8_t*, int), int64_t seekFunction(void*, int64_t, int), int width, int height)
	{
		if (!ioBuffer)
			ioBuffer = (unsigned char *)av_malloc(0x4000 + FF_INPUT_BUFFER_PADDING_SIZE); // can get av_free()ed by libav
		if (!avioContext)
			avioContext = avio_alloc_context(ioBuffer, 0x4000, 0, h, readFunction, NULL, seekFunction);
		if (!format_ctx)
			format_ctx = avformat_alloc_context();
		format_ctx->pb = avioContext;
		return Load(L"dummyFilename", width, height, 0);
	}

    virtual void GetNextFrame(double dt, void *dst_surface)// Получить следующий фрейм
    {
      playback_time += dt;//изменение времени

      //Данные из файла читаются пакетами (AVPacket), а для отображения используется фрейм (AVFrame).
      AVPacket *avpacket = decoding_packet;
      AVFrame *avframe = decoding_frame;
      
	  //Инизиализируем avframe
      avcodec_get_frame_defaults(avframe);

      int desired_frame_number = floor(playback_time * video.dec_ctx->time_base.den / video.dec_ctx->time_base.num + 0.5);
      if (last_resampled_frame_num == desired_frame_number)
      {
        memcpy(dst_surface, last_resampled_frame_data[0], current_movie_height * last_resampled_frame_linesize[0]);
        return;
      }

      volatile int frameFinished = false;

	  //чтение пакетов
      // keep reading packets until we hit the end or find a video packet
      do
      {
        if (loop_current_file)
        {
          //Now seek back to the beginning of the stream
          if (video.dec_ctx->frame_number >= video.stream->duration - 1 )
            end_current_file = true;
        }
        if (av_read_frame(format_ctx, avpacket) < 0) //воспроизведение завершено
        {
          // probably movie is finished
          end_current_file = true;
          av_free_packet(avpacket);
		  return;
        }
		// Is this a packet from the video stream?
        // audio packet - queue into playing
        //Принадлежит ли пакет к аудиопотоку
        if (avpacket->stream_index == audio.stream_idx)
        {
          MemoryStream audio_data;
          if (DecodeAudioFrame(audio.dec_ctx, avpacket, avframe, &audio_data, &num_audio_samples))
            provider->Stream16(audio_data_in_device, num_audio_samples, audio_data.Ptr());
          //continue;
        }

		// Decode video frame
        //пакет к видеопотоку
        // video packet - decode & maybe show
        else if (avpacket->stream_index == video.stream_idx)
        {
          do
          {
	       //Функция avcodec_decode_video2 осуществляет декодирование пакета в фрейм с использованием кодека,
           //который мы получили раньше (codec_context). Функция устанавливает положительное значение frame_finished в случае
           //если фрейм декодирован целиком (то есть один фрейм может занимать несколько пакетов и frame_finished будет 
           //установлен только при декодировании последнего пакета).
            if (avcodec_decode_video2(video.dec_ctx, avframe, (int *)&frameFinished, avpacket) < 0)
              __debugbreak();
          } while (!frameFinished);
        }
      }
      while (avpacket->stream_index != video.stream_idx ||
               avpacket->pts != desired_frame_number);

      if (frameFinished)
      {
        if (last_resampled_frame_data[0])
          av_freep(&last_resampled_frame_data[0]);

        AVPixelFormat  rescaled_format = AV_PIX_FMT_RGB32;
        uint8_t       *rescaled_data[4] = {nullptr, nullptr, nullptr, nullptr};
        int            rescaled_linesize[4] = {0, 0, 0, 0};

        if (av_image_alloc(rescaled_data, rescaled_linesize, current_movie_width, current_movie_height, rescaled_format, 1) >= 0)
        {
          // создание контекста для преобразования
          SwsContext *converter = sws_getContext(avframe->width, avframe->height, (AVPixelFormat)avframe->format,
                                               current_movie_width, current_movie_height, rescaled_format,
                                               SWS_BICUBIC, nullptr, nullptr, nullptr);
          // преобразование кадра(масштабирование)
          sws_scale(converter, avframe->data, avframe->linesize, 0, avframe->height, rescaled_data, rescaled_linesize);
          sws_freeContext(converter);

          //копирование в возвращаемую переменную
          memcpy(dst_surface, rescaled_data[0], current_movie_height * rescaled_linesize[0]);

          last_resampled_frame_num = desired_frame_number;
          memcpy(last_resampled_frame_data, rescaled_data, sizeof(rescaled_data));
          memcpy(last_resampled_frame_linesize, rescaled_linesize, sizeof(rescaled_linesize));
          //av_freep(&rescaled_data[0]);//вроде должен быть 
        }
      }
      else
        memset(dst_surface, 0, width * current_movie_height * 4);

      // Free the packet that was allocated by av_read_frame
      av_free_packet(avpacket);
    }

    virtual void Play()
    {
    }

  protected:
    char             movie_filename[256];
    int              width;
    int              height;
    bool             stopped;
    AVFormatContext *format_ctx;
    double           playback_time;
    bool             end_of_file;

    AVPacket        *decoding_packet;
    AVFrame         *decoding_frame;

    AVAudioStream   audio;
    int             num_audio_frames;
    int             num_audio_samples;
	unsigned char * ioBuffer;
	AVIOContext *avioContext;
    OpenALSoundProvider::StreamingTrackBuffer *audio_data_in_device;

    AVVideoStream   video;
    int             last_resampled_frame_num;
    uint8_t        *last_resampled_frame_data[4];
    int             last_resampled_frame_linesize[4];
};	

ITrack *MPlayer::LoadTrack(const wchar_t *filename) //Загрузить mp3
{
  auto track = new Track;
  if (!track->LoadAudio(filename))
  {
    delete track;
    track = nullptr;
  }
  return track;
}

IMovie *MPlayer::LoadMovie(const wchar_t *filename, int width, int height, int cache_ms)	//Загрузить видео
{
  movie = new Movie;
  if (!movie->Load(filename, width, height, cache_ms))
  {
    delete movie;
    movie = nullptr;
  }
  return movie;
}

IMovie *MPlayer::LoadMovieFromLOD(HANDLE h, int readFunction(void*, uint8_t*, int), int64_t seekFunction(void*, int64_t, int), int width, int height)
{
	movie = new Movie;
	if (movie)
	{
		if (movie->LoadFromLOD(h, readFunction, seekFunction, width, height))
			return movie;
		delete movie;
	}
	return nullptr;
}

void av_logger(void *, int, const char *format, va_list args)
{
  va_list va;
  va_start(va, format);
  char msg[256];
  vsprintf(msg, format, va);
  va_end(va);

  log("av: %s", msg);
}

MPlayer::MPlayer()
{
  static int libavcodec_initialized = false;

  if (!libavcodec_initialized)
  {
    av_log_set_callback(av_logger);
    avcodec_register_all();

    // Register all available file formats and codecs
    //инициализируем библиотеку ffmpeg(Шаг 1)
    //Во время инициализации регистрируются все имеющиеся в библиотеке форматы файлов и кодеков.
    //После этого они будут использоваться автоматически при открытии файлов этого формата и с этими кодеками.
    av_register_all();

    libavcodec_initialized = true;
  }

  if (!provider)
  {
    provider = new OpenALSoundProvider;
    provider->Initialize();
  }
}

MPlayer::~MPlayer()
{
}

void PlayAudio(const wchar_t * pFilename)
{
  pTrack = pMediaPlayer->LoadTrack(pFilename);
  pTrack->Play();
}

void PlayMovie(const wchar_t * pFilename)
{
  Media::IMovie *track = pMediaPlayer->LoadMovie(pFilename, 640, 480, 0);
  track->Play();
}

void MovieRelease()
{
  movie->Release();
  delete movie;
  movie = nullptr;
}

//////////////////////////////////////////////////////////////////////////
//Included from a VideoPlayer.cpp file/вставлено из файла VideoPlayer.cpp/
//////////////////////////////////////////////////////////////////////////

//used in void VideoPlayer::Initialize(OSWindow *target_window) for open .vid files
MovieHeader *pMightVideoHeaders;
MovieHeader *pMagicVideoHeaders;
HANDLE hMightVid;
HANDLE hMagicVid;
unsigned __int64 uBinkVersion;
unsigned int uNumMightVideoHeaders;
unsigned int uNumMagicVideoHeaders;
//

