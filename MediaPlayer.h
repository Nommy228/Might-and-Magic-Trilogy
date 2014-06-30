#pragma once

namespace Media
{
  class ITrack
  {
    public: virtual void Play(bool loop = false) = 0;
  };

  class IMovie
  {
    public: virtual void Play() = 0;
            virtual void GetNextFrame(double dt, void *target_surface) = 0;
  };

  class MPlayer
  {
    public:
               MPlayer();
      virtual ~MPlayer();

      ITrack *LoadTrack(const wchar_t *name);
      IMovie *LoadMovie(const wchar_t *name, int width, int height, int cache_ms);
      IMovie *LoadMovieFromLOD(HANDLE h, int readFunction(void*, uint8_t*, int), int64_t seekFunction(void*, int64_t, int), int width, int height);
  };
};

extern Media::MPlayer *pMediaPlayer;
extern Media::IMovie *pMovie;
extern Media::ITrack *pTrack;
extern class Movie *movie;

extern bool end_current_file;
extern bool loop_current_file;
extern DWORD time_video_begin;
extern int current_movie_width;
extern int current_movie_height;

extern int mSourceID;

//int64_t seekFunction(void* opaque, int64_t offset, int whence);
extern void PlayMovie(const wchar_t * pFilename);
extern void PlayAudio(const wchar_t * pFilename);
extern void MovieRelease();

//////////////////////////////////////////////////////////////////////////
//Included from a VideoPlayer.h file/вставлено из файла VideoPlayer.h/
//////////////////////////////////////////////////////////////////////////

//used in void VideoPlayer::Initialize(OSWindow *target_window) for open .vid files
struct MovieHeader
{
  char pVideoName[40];
  unsigned int uFileOffset;
};

extern MovieHeader *pMightVideoHeaders;
extern MovieHeader *pMagicVideoHeaders;
extern HANDLE hMightVid;
extern HANDLE hMagicVid;
extern unsigned __int64 uBinkVersion;
extern unsigned int uNumMightVideoHeaders;
extern unsigned int uNumMagicVideoHeaders;
//