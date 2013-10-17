#pragma once
#include "AIL.h"




void BINKW32_DLL_Initialize();


int __stdcall BinkDDSurfaceType(struct IDirectDrawSurface *);
int __stdcall BinkSetSoundSystem(void *pSoundSystem, HDIGDRIVER hDrv);
int __stdcall BinkOpenMiles(int);
HBINK __stdcall BinkOpen(void *hFileHandle, unsigned int uFlags);
int __stdcall BinkWait(HBINK);
int __stdcall BinkDoFrame(HBINK);
int __stdcall BinkNextFrame(HBINK);
int __stdcall BinkGetRects(HBINK hBink, unsigned int uFlags);
int __stdcall BinkCopyToBuffer(HBINK hBink, void *pBuffer, unsigned int lPitch, unsigned int uNumLines, unsigned int uX, unsigned int uY, unsigned int uFlags);
int __stdcall BinkPause(HBINK, int bPause);
int __stdcall BinkClose(HBINK);
int __stdcall BinkGoto(HBINK, long a2, long a3);

int __stdcall BinkBufferSetOffset(void *, int, int);
int __stdcall BinkBufferSetScale(void *, unsigned int uWidth, unsigned int uHeight);

















void SMACKW32_DLL_Initialize();

struct _SMACK
{
  unsigned int Version;           // SMK2 only right now
  unsigned int Width;             // Width (1 based, 640 for example)
  unsigned int Height;            // Height (1 based, 480 for example)
  unsigned int Frames;            // Number of frames (1 based, 100 = 100 frames)
  unsigned int MSPerFrame;        // Frame Rate
  unsigned int SmackerType;       // bit 0 set=ring frame
  unsigned int LargestInTrack[7]; // Largest single size for each track
  unsigned int tablesize;         // Size of the init tables
  unsigned int codesize;          // Compression info   
  unsigned int absize;            // ditto
  unsigned int detailsize;        // ditto
  unsigned int typesize;          // ditto
  unsigned int TrackType[7];      // high byte=0x80-Comp,0x40-PCM data,0x20-16 bit,0x10-stereo
  unsigned int extra;             // extra value (should be zero)
  unsigned int NewPalette;        // set to one if the palette changed
  unsigned char Palette[772];      // palette data
  unsigned int PalType;           // type of palette
  unsigned int FrameNum;          // 0374 Frame Number to be displayed
  unsigned int FrameSize;         // The current frame's size in bytes
  unsigned int SndSize;           // The current frame sound tracks' size in bytes
  int LastRectx;                  // 0380 Rect set in from SmackToBufferRect (X coord)
  int LastRecty;                  // Rect set in from SmackToBufferRect (Y coord)
  int LastRectw;                  // Rect set in from SmackToBufferRect (Width)
  int LastRecth;                  // 038C Rect set in from SmackToBufferRect (Height)
  unsigned int OpenFlags;         // flags used on open
  unsigned int LeftOfs;           // Left Offset used in SmackTo
  unsigned int TopOfs;            // Top Offset used in SmackTo
  unsigned int LargestFrameSize;  // Largest frame size
  unsigned int Highest1SecRate;   // Highest 1 sec data rate
  unsigned int Highest1SecFrame;  // Highest 1 sec data rate starting frame
  unsigned int ReadError;         // Set to non-zero if a read error has ocurred
  unsigned int addr32;            // translated address for 16 bit interface
};



struct _SMACKBLIT
{
  unsigned int    Flags;
  unsigned char  *Palette;
  unsigned int    PalType;
  unsigned short *SmoothTable;
  unsigned short *Conv8to16Table;
  unsigned int    whichmode;
  unsigned int    palindex;
  unsigned int    t16index;
  unsigned int    smoothindex;
  unsigned int    smoothtype;
  unsigned int    firstpalette;
};

struct _SMACKBUF
{
        unsigned int Reversed;             // 1 if the buffer is upside down
        unsigned int SurfaceType;          // SMACKSURFACExxxx defines
        unsigned int BlitType;             // SMACKxxxxBLIT defines
        unsigned int FullScreen;           // 1 if full-screen
        unsigned int Width;
        unsigned int Height;
        unsigned int Pitch;
        unsigned int Zoomed;
        unsigned int ZWidth;
        unsigned int ZHeight;
        unsigned int DispColors;           // colors on the screen
        unsigned int MaxPalColors;         // total possible colors in palette (usually 256)
        unsigned int PalColorsInUse;       // Used colors in palette (usually 236)
        unsigned int StartPalColor;        // first usable color index (usually 10)
        unsigned int EndPalColor;          // last usable color index (usually 246)
        RGBQUAD Palette[256];
        unsigned int PalType;
        unsigned int forceredraw;  // force a complete redraw on next blit (for >8bit)
        unsigned int didapalette;  // force an invalidate on the next palette change

        void * Buffer;
        void * DIBRestore;
        unsigned int OurBitmap;
        unsigned int OrigBitmap;
        unsigned int OurPalette;
        unsigned int WinGDC;
        unsigned int FullFocused;
        unsigned int ParentHwnd;
        unsigned int OldParWndProc;
        unsigned int OldDispWndProc;
        unsigned int DispHwnd;
        unsigned int WinGBufHandle;
        void * lpDD;
        void * lpDDSP;
        unsigned int DDSurfaceType;
        struct _SMACKBLIT DDblit;
        int ddSoftwarecur;
        int didaddblit;
        int lastwasdd;
        RECT ddscreen;
        int manyblits;
        int * blitrects;
        int * rectsptr;
        int maxrects;
        int numrects;
        HDC lastdc;
};

#define SMACKBUFFER555      0x80000000
#define SMACKBUFFER565      0xC0000000

#define SMACKBLIT1X                1
#define SMACKBLIT2X                2
#define SMACKBLIT2XSMOOTHING       4
#define SMACKBLIT2XINTERLACE       8


int __stdcall SmackSoundUseMSS(HDIGDRIVER hDrv);
unsigned int __stdcall SmackUseMMX(unsigned int flag);
HSMACK __stdcall SmackOpen(HANDLE hSourceFile, unsigned int uFlags, unsigned int uExtraBuffers);
HSMACKBLIT __stdcall SmackBlitOpen(unsigned int uSurfaceFormat);
void __stdcall SmackToBuffer(HSMACK, unsigned int uX, unsigned int uY, unsigned int uPitch, unsigned int uHeight, void *pBuffer, unsigned int uFlags);
void __stdcall SmackBlitSetPalette(HSMACKBLIT hBlit, void *pPalette, unsigned int uPalType);
unsigned int __stdcall SmackDoFrame(HSMACK);
unsigned int __stdcall SmackToBufferRect(HSMACK, unsigned int uSmackSurface);
void __stdcall SmackBlit(HSMACKBLIT, void *pDest, unsigned int uDestPitch, unsigned int uDestX, unsigned int uDestY, void *pSrc, unsigned int uSrcPitch, unsigned int uSrcX, unsigned int uSrcY, unsigned int uSrcZ, unsigned int uSrcW);
void __stdcall SmackNextFrame(HSMACK);
unsigned int __stdcall SmackWait(HSMACK);
unsigned int __stdcall SmackSoundOnOff(HSMACK, unsigned int bOn);
void __stdcall SmackClose(HSMACK);
void __stdcall SmackBufferClose(HSMACKBUF);
void __stdcall SmackBlitClose(HSMACKBLIT);
int __stdcall SmackBlitClear(HSMACKBLIT a1, unsigned short *pFrameData, unsigned int uTargetSurfacePitch, unsigned int uOutX, unsigned int uOutY, unsigned int uOutZ, unsigned int uOutW, int a8);





int __stdcall SmackBufferOpen(HWND a1, long a2, long a3, long a4, long a5, long a6);
int __fastcall SmackVolumePan(_SMACK *a3, long a4, long a5, long a6);

int __stdcall SmackGoto(_SMACK *a1, long a2);

// sub_4D83D0: using guessed type int __stdcall SmackBufferNewPalette(_DWORD, _DWORD, _DWORD);
int __stdcall SmackBufferNewPalette(long a1, long a2, long a3);

// sub_4D83D4: using guessed type int __stdcall SmackColorRemapWithTrans(_DWORD, _DWORD, _DWORD, _DWORD, _DWORD);
int __stdcall SmackColorRemapWithTrans(long a1, long a2, long a3, long a4, long a5);














