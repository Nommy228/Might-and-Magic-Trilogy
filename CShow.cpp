/*#include <assert.h>

#include "CShow.h"

#include "mm7_data.h"
#include "VideoPlayer.h"
#include "Mouse.h"

void CShow::PlayMovie(MovieType eVideo, bool bShowMouseAfterPlayback)
{
  if (bNoVideo) return;

  switch (eVideo)
  {
    case MOVIE_3DOLogo: VideoPlayer::MovieLoop("3dologo", 0, 0, 1);        break;
    case MOVIE_NWCLogo: VideoPlayer::MovieLoop("new world logo", 0, 1, 1); break;
    case MOVIE_JVC:     VideoPlayer::MovieLoop("jvc", 0, 1, 1);            break;
    case MOVIE_Intro:   VideoPlayer::MovieLoop("Intro", 0, 1, 1);          break;
    case MOVIE_Emerald: VideoPlayer::MovieLoop("Intro Post", 0, 1, 1);     break;
    case MOVIE_Death:   VideoPlayer::MovieLoop("losegame", 2, 1, 1);       break;
    case MOVIE_Outro:   VideoPlayer::MovieLoop("end_seq1", 20, 1, 1);      break;

    default:
      assert(false && "Invalid movie requested in " __FUNCTION__);
      break;
  }
}

*/