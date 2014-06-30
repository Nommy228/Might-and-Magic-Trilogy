#pragma once
#include "VideoPlayer.h"
#include "ErrorHandling.h"


/*  302 */
enum MovieType
{
  MOVIE_Invalid = 0x0,
  MOVIE_3DOLogo = 0x1,
  MOVIE_NWCLogo = 0x2,
  MOVIE_JVC = 0x3,
  MOVIE_Emerald = 0x4,
  MOVIE_Intro = 0x5,
  MOVIE_Death = 0x6,
  MOVIE_Outro = 0x7,
};

inline void PlayFullscreenMovie(MovieType movie, bool bShowMouseAfterPlayback = false)
{
  extern unsigned int bNoVideo;
  if (bNoVideo) return;

  switch (movie)
  {
    case MOVIE_3DOLogo: pVideoPlayer->MovieLoop("3dologo", 0, 0, 1);        break;
	case MOVIE_NWCLogo: pVideoPlayer->MovieLoop("new world logo", 0, 1, 1); break;
	case MOVIE_JVC:     pVideoPlayer->MovieLoop("jvc", 0, 1, 1);            break;
	case MOVIE_Intro:   pVideoPlayer->MovieLoop("Intro", 0, 1, 1);          break;
	case MOVIE_Emerald: pVideoPlayer->MovieLoop("Intro Post", 0, 1, 1);     break;
	case MOVIE_Death:   pVideoPlayer->MovieLoop("losegame", 2, 1, 1);       break;
	case MOVIE_Outro:   pVideoPlayer->MovieLoop("end_seq1", 20, 1, 1);      break;

    default:
      Error("Invalid movie requested: %u", movie);
      break;
  }
}

/*
#pragma pack(push, 1)
class CShow
{
public:
  //----- (004A9507) --------------------------------------------------------
  CShow() {}

  //----- (004A9510) --------------------------------------------------------
  virtual ~CShow() {}

  
  //----- (004A952D) --------------------------------------------------------
  void PlayMovie(MovieType eVideo, bool bShowMouseAfterPlayback);


  void ( ***vdestructor_ptr)(CShow *, bool);
};
#pragma pack(pop)*/