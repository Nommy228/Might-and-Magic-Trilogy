#define _CRT_SECURE_NO_WARNINGS

#include "MMT.h"
#include "GUIWindow.h"
#include "mm7_unsorted_subs.h"
#include "mm7_data.h"
#include "AudioPlayer.h"
#include "Mouse.h"
#include "LOD.h"
#include "Render.h"
#include "VideoPlayer.h"
#include "CShow.h"
#include "GUIFont.h"
#include "lib/libpng/png.h"
#include "MediaPlayer.h"

void  ShowLogoVideo()
{
  RGBTexture tex; // [sp+Ch] [bp-30h]@1
  //unsigned int uTrackStartMS; // [sp+34h] [bp-8h]@8
  //unsigned int uTrackEndMS; // [sp+38h] [bp-4h]@8

  pVideoPlayer->bStopBeforeSchedule = false;
  pVideoPlayer->pResetflag = 0;
  bGameoverLoop = 1;
  if (!bNoVideo)
  {
    pRenderer->PresentBlackScreen();
    PlayFullscreenMovie(MOVIE_3DOLogo, true);
    if ( !pVideoPlayer->bStopBeforeSchedule )
    {
      PlayFullscreenMovie(MOVIE_NWCLogo, true);
      if ( !pVideoPlayer->bStopBeforeSchedule )
        PlayFullscreenMovie(MOVIE_JVC, true);
    }
  }
  char pContainerName[64];

  sprintf(pContainerName, "data\\New_Icons/%s", "MMTTITLE.pcx");
  tex.LoadPCXFile(pContainerName, 0);

  pRenderer->BeginScene();
  pRenderer->DrawTextureRGB(0, 0, &tex);
  free(tex.pPixels);
  tex.pPixels = 0;
  MainMenuUI_LoadFontsAndSomeStuff();
  DrawMMTCopyrightWindow();//��������� ��������� ��� ����

  pRenderer->EndScene();
  pRenderer->Present();

  #ifndef _DEBUG
    Sleep(1500);   // let the copyright window stay for a while
  #endif

  /*if (!bNoSound && pAudioPlayer->hAILRedbook )
  {
    pAudioPlayer->SetMusicVolume((signed __int64)(pSoundVolumeLevels[uMusicVolimeMultiplier] * 64.0));
    AIL_redbook_stop(pAudioPlayer->hAILRedbook);
    AIL_redbook_track_info(pAudioPlayer->hAILRedbook, 14, &uTrackStartMS, &uTrackEndMS);
    AIL_redbook_play(pAudioPlayer->hAILRedbook, uTrackStartMS + 1, uTrackEndMS);
  }	 */
  bGameoverLoop = 0;
}

void abort_(const char * s, ...)
{
  va_list args;
  va_start(args, s);
  vfprintf(stderr, s, args);
  fprintf(stderr, "\n");
  va_end(args);
  abort();
}

Texture *LoadPNG(const char *name)
{
  int x, y;
  int width, height;
  png_byte color_type;
  png_byte bit_depth;
  png_structp png_ptr;
  png_infop info_ptr;
  int number_of_passes;
  png_bytep * row_pointers;
  uint i = 0;
  Texture *tex;

  char header[8];    // 8 is the maximum size that can be checked

  /* open file and test for it being a png */
  FILE *fp = fopen(name, "rb");
  if (!fp)
    abort_("[read_png_file] File %s could not be opened for reading", name);
  fread(header, 1, 8, fp);
  if (png_sig_cmp((png_bytep)header, 0, 8))
    abort_("[read_png_file] File %s is not recognized as a PNG file", name);
  /* initialize stuff */
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr)
    abort_("[read_png_file] png_create_read_struct failed");
  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    abort_("[read_png_file] png_create_info_struct failed");

  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[read_png_file] Error during init_io");

  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, 8);

  png_read_info(png_ptr, info_ptr);

  width = png_get_image_width(png_ptr, info_ptr);
  height = png_get_image_height(png_ptr, info_ptr);
  color_type = png_get_color_type(png_ptr, info_ptr);
  bit_depth = png_get_bit_depth(png_ptr, info_ptr);

  number_of_passes = png_set_interlace_handling(png_ptr);
  png_read_update_info(png_ptr, info_ptr);

  /* read file */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[read_png_file] Error during read_image");
  row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
  for (y=0; y<height; y++)
    row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

  png_read_image(png_ptr, row_pointers);

  fclose(fp);

  tex = new Texture;
  tex->uTextureHeight = height;
  tex->uTextureWidth = width;
  tex->uSizeOfMaxLevelOfDetail = png_get_rowbytes(png_ptr, info_ptr);
  tex->uTextureSize = png_get_rowbytes(png_ptr, info_ptr);
  tex->uDecompressedSize = png_get_rowbytes(png_ptr, info_ptr);
  tex->pPalette16 = (unsigned __int16 *) malloc(sizeof(unsigned __int16) * width * height);
  tex->pLevelOfDetail0_prolly_alpha_mask = (unsigned __int8 *) malloc(sizeof(unsigned __int8) * width * height);

  for (y=0; y<height; y++)
  {
    png_byte* row = row_pointers[y];
    for (x=0; x<width; x++)
    {
      png_byte* ptr = &(row[x*4]);
      //Log::Warning(L"Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
      //                   x, y, ptr[0], ptr[1], ptr[2], ptr[3]);
      png_byte tmp = ptr[2];
      ptr[2] = ptr[0];
      ptr[0] = ptr[3];
      ptr[3] = 255 - tmp;
      tex->pPalette16[i] = Color16(ptr[0], ptr[1], ptr[2]);
      tex->pLevelOfDetail0_prolly_alpha_mask[i] = ptr[3];
      i++;
    }
  }
  //Ritor1: temporarily stopped, needed change RGBTexture structure/���� �������������� � ����� � �������������� ������ ��������� � ���������(�������)RGBTexture
  /*for (int i = 0; i < width * height; ++i)
    tex->pPalette16[i] = 0x7FF;
  memset(tex->pLevelOfDetail0_prolly_alpha_mask, 1, sizeof(unsigned __int8) * width * height);*/
  return tex;
}

void MMT_MainMenu_Loop()
{
  GUIButton *pButton; // eax@27
  unsigned int pControlParam; // ecx@35
  unsigned int pX;
  unsigned int pY; // [sp-18h] [bp-54h]@39
  Texture *pTexture; // [sp-14h] [bp-50h]@39
  char pContainerName[64];

  pCurrentScreen = SCREEN_GAME;

  pGUIWindow2 = 0;
  pAudioPlayer->StopChannels(-1, -1);//����������/����������� �����

  if (!bNoSound )
    PlayAudio(L"Sounds\\New_Sounds/Stronghold_Theme.mp3");//������������� ��3
  //if (!bNoVideo )
    //pVideoPlayer->PlayMovie(L"Anims\\New_Video/3DOLOGO.smk");

  pMouse->RemoveHoldingItem();//�������� ������ �� ����

  pIcons_LOD->_inlined_sub2();

  //�������� ������ ����
  //WINDOW_MainMenu ��������� � GUIWindow.h
  pWindow_MMT_MainMenu = GUIWindow::Create(0, 0, window->GetWidth(), window->GetHeight(), WINDOW_MainMenu, 0, 0);

  //�������� ������
  //Texture* MMT_MM6      = pIcons_LOD->LoadTexturePtr("title_new", TEXTURE_16BIT_PALETTE);

  sprintf(pContainerName, "data\\New_Icons/%s", "mm6_button_oval.png");//��������� png �� �����
  Texture* MMT_MM6 = LoadPNG(pContainerName);
								
  Texture* MMT_MM7      = pIcons_LOD->LoadTexturePtr("title_load", TEXTURE_16BIT_PALETTE);
  Texture* MMT_MM8      = pIcons_LOD->LoadTexturePtr("title_cred", TEXTURE_16BIT_PALETTE);
  Texture* MMT_Continue = pIcons_LOD->LoadTexturePtr("title_exit", TEXTURE_16BIT_PALETTE);
  Texture* MMT_Exit     = pIcons_LOD->LoadTexturePtr("title_exit", TEXTURE_16BIT_PALETTE);

  pMMT_MainMenu_BtnMM6      = pWindow_MMT_MainMenu->CreateButton(0,                 0,                                MMT_MM6->uTextureWidth,      MMT_MM6->uTextureHeight,      1, 0, UIMSG_MMT_MainMenu_MM6,      0, 0, "", MMT_MM6, 0);
  pMMT_MainMenu_BtnMM7      = pWindow_MMT_MainMenu->CreateButton(window->GetWidth() - (window->GetWidth()  / 4), window->GetHeight() / 4,                                MMT_MM7->uTextureWidth,      MMT_MM7->uTextureHeight,      1, 0, UIMSG_MMT_MainMenu_MM7,      1, 0, "", MMT_MM7, 0);
  pMMT_MainMenu_BtnMM8      = pWindow_MMT_MainMenu->CreateButton(window->GetWidth() - (window->GetWidth()  / 4), window->GetHeight() - ((window->GetHeight() / 4) + 50), MMT_MM8->uTextureWidth,      MMT_MM8->uTextureHeight,      1, 0, UIMSG_MMT_MainMenu_MM8,      2, 0, "", MMT_MM8, 0);
  pMMT_MainMenu_BtnContinue = pWindow_MMT_MainMenu->CreateButton((window->GetWidth() / 4) - 100,                 window->GetHeight() - ((window->GetHeight() / 4) + 50), MMT_Continue->uTextureWidth, MMT_Continue->uTextureHeight, 1, 0, UIMSG_MMT_MainMenu_Continue, 3, 0, "", MMT_Continue, 0);
  pMMT_MainMenu_BtnExit     = pWindow_MMT_MainMenu->CreateButton(window->GetWidth() - 130,                       window->GetHeight() - 35,                               MMT_Exit->uTextureWidth,     MMT_Exit->uTextureHeight,     1, 0, UIMSG_ExitToWindows,         4, 0, "", MMT_Exit, 0);

  pTexture_PCX.Release();

  sprintf(pContainerName, "data\\New_Icons/%s", "MMTTITLE.pcx");//��������� pcx �� �����
  pTexture_PCX.LoadPCXFile(pContainerName, 0);
  SetCurrentMenuID(MENU_MMT_MAIN_MENU);//��������� � enum MENU_STATE � GUIWindows.h
  SetForegroundWindow(window->GetApiHandle());
  SendMessageW(window->GetApiHandle(), WM_ACTIVATEAPP, 1, 0);
  while (GetCurrentMenuID() == MENU_MMT_MAIN_MENU )
  {
    POINT cursor;
    pMouse->GetCursorPos(&cursor);

    for (MSG msg; PeekMessageW(&msg, 0, 0, 0, PM_REMOVE);)
    {
      if (msg.message == WM_QUIT)
        Game_DeinitializeAndTerminate(0);
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
    }

    if (dword_6BE364_game_settings_1 & GAME_SETTINGS_APP_INACTIVE)
    {
      WaitMessage();
      continue;
    }

      pRenderer->BeginScene();
      pRenderer->DrawTextureRGB(0, 0, &pTexture_PCX);

      MMT_MenuMessageProc();//��������� ��� ��� ���� ���������� ���������
      GUI_UpdateWindows();

      if ( !pModalWindow )// ???
      {
          pButton = pWindow_MMT_MainMenu->pControlsHead;
          for ( pButton = pWindow_MMT_MainMenu->pControlsHead; pButton; pButton = pButton->pNext )
          {
           if ( cursor.x >= (signed int)pButton->uX && cursor.x <= (signed int)pButton->uZ
             && cursor.y >= (signed int)pButton->uY && cursor.y <= (signed int)pButton->uW )
           {
            pControlParam = pButton->msg_param;
            switch (pControlParam) // ��������� ������
            {
              case 0:
                pTexture = MMT_MM6;
                pX = 0;
                pY = 0;
                break;
              case 1:
                pTexture = MMT_MM7;
                pX = window->GetWidth() - (window->GetWidth() / 4);
                pY = window->GetHeight() / 4;
                break;
              case 2:
                pTexture = MMT_MM8;
                pX = window->GetWidth() - (window->GetWidth() / 4);
                pY = window->GetHeight() - ((window->GetHeight() / 4) + 50);
                break;
              case 3:
                pTexture = MMT_Continue;
                pX = (window->GetWidth() / 4) - 100;
                pY = window->GetHeight() - ((window->GetHeight() / 4) + 50);
                break;
              case 4:
                pTexture = MMT_Exit;
                pX = window->GetWidth() - 130;
                pY = window->GetHeight() - 35;
                break;
            }
            pRenderer->DrawTextureIndexed(pX, pY, pTexture); //��������� ������
           }
          }
        }
      //}
      pRenderer->EndScene();
      pRenderer->Present();
  }
  MMT_MenuMessageProc();
  pRenderer->BeginScene();
  GUI_UpdateWindows();
  pRenderer->EndScene();
  pRenderer->Present();

  //����������� �������
  pTexture_PCX.Release();
  if ( pGUIWindow2 )
  {
    pGUIWindow2->Release();
    pGUIWindow2 = 0;
  }
  pWindow_MMT_MainMenu->Release();
  pIcons_LOD->RemoveTexturesPackFromTextureList();
}

void MMT_MenuMessageProc()
{
  int pParam;
  int pParam2;
  UIMessageType pUIMessageType;

  if ( pMessageQueue_50CBD0->uNumMessages )
  {
    do
    {

      pMessageQueue_50CBD0->PopMessage(&pUIMessageType, &pParam, &pParam2);

      switch (pUIMessageType)
      {
        case UIMSG_MMT_MainMenu_MM7: //������ ���� ��7
          //GUIWindow::Create(495, 172, 0, 0, WINDOW_PressedButton2, (int)pMainMenu_BtnNew, 0);
          alSourceStop(mSourceID);
          ShowMM7IntroVideo_and_LoadingScreen();
          SetCurrentMenuID(MENU_MAIN);
          break;

        default:
          break;
      }
    }
    while ( pMessageQueue_50CBD0->uNumMessages );
  }
}
void DrawMMTCopyrightWindow()
{
  GUIWindow Dst; // [sp+8h] [bp-54h]@1

  memset(&Dst, 0, 0x54u);
  Dst.uFrameWidth = 624;
  Dst.uFrameHeight = 256;
  Dst.uFrameX = 8;
  Dst.uFrameY = 30;
  Dst.uFrameHeight = pFontSmallnum->CalcTextHeight("Text Verification: Here we can write an explanation of the project", &Dst, 24, 0)
                   + 2 * LOBYTE(pFontSmallnum->uFontHeight)
                   + 24;
  Dst.uFrameY = 470 - Dst.uFrameHeight;
  Dst.uFrameZ = Dst.uFrameX + Dst.uFrameWidth - 1;
  Dst.uFrameW = 469;
  //Dst.Hint = "abcagfdsgsg ljsrengvlkjesnfkjwnef";
  Dst.DrawMessageBox(0);

  Dst.uFrameWidth -= 24;
  Dst.uFrameX += 12;
  Dst.uFrameY += 12;
  Dst.uFrameHeight -= 12;
  Dst.uFrameZ = Dst.uFrameX + Dst.uFrameWidth - 1;
  Dst.uFrameW = Dst.uFrameY + Dst.uFrameHeight - 1;
  Dst.DrawTitleText(pFontSmallnum, 0, 0xCu, ui_mainmenu_copyright_color, "Text Verification: Here we can write an explanation of the project", 3);
}