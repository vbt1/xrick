/*
 * xrick/src/sysvid.c
 *
 * Copyright (C) 1998-2002 BigOrno (bigorno@bigorno.net). All rights reserved.
 *
 * The use and distribution terms for this software are contained in the file
 * named README, which can be found in the root of this distribution. By
 * using this software in any fashion, you are agreeing to be bound by the
 * terms of this license.
 *
 * You must not remove this notice, or any other, from this software.
 */

#include <stdlib.h> /* malloc */

#include "sdl/SDL.h"

#include "../include/system.h"
#include "../include/game.h"
#include "../include/img.h"
#include "../include/debug.h"

#ifdef __MSVC__
#include <memory.h> /* memset */
#endif

U8 *sysvid_fb; /* frame buffer */
//U8 sysvid_fb[SYSVID_WIDTH * SYSVID_HEIGHT];
rect_t SCREENRECT = {0, 0, SYSVID_WIDTH, SYSVID_HEIGHT, NULL}; /* whole fb */
void ClsFast(Uint8 *maplocation);
static SDL_Color palette[256];
//static SDL_Surface *screen;
//static U32 videoFlags;

//static U8 zoom = SYSVID_ZOOM; /* actual zoom level */
//static U8 szoom = 0;  /* saved zoom level */
//static U8 fszoom = 0;  /* fullscreen zoom level */

//#include "img_icon.e"

/*
 * color tables
 */

#ifdef GFXPC
static U8 RED[] = { 0x00, 0x50, 0xf0, 0xf0, 0x00, 0x50, 0xf0, 0xf0 };
static U8 GREEN[] = { 0x00, 0xf8, 0x50, 0xf8, 0x00, 0xf8, 0x50, 0xf8 };
static U8 BLUE[] = { 0x00, 0x50, 0x50, 0x50, 0x00, 0xf8, 0xf8, 0xf8 };
#endif
#ifdef GFXST
static U8 RED[] = { 0x00, 0xd8, 0xb0, 0xf8,
                    0x20, 0x00, 0x00, 0x20,
                    0x48, 0x48, 0x90, 0xd8,
                    0x48, 0x68, 0x90, 0xb0,
                    /* cheat colors */
                    0x50, 0xe0, 0xc8, 0xf8,
                    0x68, 0x50, 0x50, 0x68,
                    0x80, 0x80, 0xb0, 0xe0,
                    0x80, 0x98, 0xb0, 0xc8
};
static U8 GREEN[] = { 0x00, 0x00, 0x6c, 0x90,
                      0x24, 0x48, 0x6c, 0x48,
                      0x6c, 0x24, 0x48, 0x6c,
                      0x48, 0x6c, 0x90, 0xb4,
		      /* cheat colors */
                      0x54, 0x54, 0x9c, 0xb4,
                      0x6c, 0x84, 0x9c, 0x84,
                      0x9c, 0x6c, 0x84, 0x9c,
                      0x84, 0x9c, 0xb4, 0xcc
};
static U8 BLUE[] = { 0x00, 0x00, 0x68, 0x68,
                     0x20, 0xb0, 0xd8, 0x00,
                     0x20, 0x00, 0x00, 0x00,
                     0x48, 0x68, 0x90, 0xb0,
		     /* cheat colors */
                     0x50, 0x50, 0x98, 0x98,
                     0x68, 0xc8, 0xe0, 0x50,
                     0x68, 0x50, 0x50, 0x50,
                     0x80, 0x98, 0xb0, 0xc8};
#endif

/*
 * Initialize screen
 */
static
SDL_Surface *initScreen(U16 w, U16 h, U8 bpp, U32 flags)
{
  return (SDL_Surface *)SDL_SetVideoMode(w, h, bpp, flags);
}

void
sysvid_setPalette(img_color_t *palo, U16 n)
{
  U16 i;

  for (i = 0; i < n; i++) {
    palette[i].r = palo[i].r;
    palette[i].g = palo[i].g;
    palette[i].b = palo[i].b;
  }
  //SDL_SetColors(screen, (SDL_Color *)&palette, 0, n);
  SDL_SetColors(NULL, (SDL_Color *)&palette, 0, n);
}
void
sysvid_setGamePalette()
{
  U8 i;
  img_color_t palo[32];

  for (i = 0; i < 32; ++i) {
    palo[i].r = RED[i];
    palo[i].g = GREEN[i];
    palo[i].b = BLUE[i];
  }
  sysvid_setPalette(palo, 32);
}
/*
 * Initialise video
 */
void
sysvid_init(void)
{
  /* SDL */
  SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
  /* video modes and screen */
  //videoFlags = SDL_HWSURFACE|SDL_HWPALETTE;

  /*screen =*/ initScreen(SYSVID_WIDTH,
		      SYSVID_HEIGHT,
		      8, SDL_HWSURFACE|SDL_HWPALETTE/*videoFlags*/);
  /*
   * create v_ frame buffer
   */
  sysvid_fb = malloc(SYSVID_WIDTH * SYSVID_HEIGHT);
}
/*
 * Update screen
 * NOTE errors processing ?
 */
void
sysvid_update(rect_t *rects)
{
  if (rects == NULL)
    return;
  //slTVOn();
  slBMPut(0, 0, SYSVID_WIDTH-1, SYSVID_HEIGHT-1, sysvid_fb);
  //slSynch();
  slTVOn();
  //slSynch();
}


/*
 * Clear screen
 * (077C)
 */
void
sysvid_clear(void)
{
  slTVOff();
  //memset(sysvid_fb, 0, sizeof(sysvid_fb));//SYSVID_WIDTH * SYSVID_HEIGHT);
  ClsFast(sysvid_fb);
}	