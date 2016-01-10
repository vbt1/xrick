/*
 * xrick/src/system.c
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

#include "sdl/SDL.h"

//#include <stdarg.h>   /* args for sys_panic */
//#include <fcntl.h>    /* fcntl in sys_panic */
#include <stdio.h>    /* printf */
#include <stdlib.h>
#include <signal.h>

#include "../include/system.h"

/*
 * Return number of microseconds elapsed since first call
 */

U32
sys_gettime(void)
{
  static U32 ticks_base = 0;
  U32 ticks;

  ticks = SDL_GetTicks();
  if (!ticks_base)
    ticks_base = ticks;
  return ticks - ticks_base;
}

/*
 * Sleep a number of microseconds
 */
void
sys_sleep(int s)
{
  //SDL_Delay(s);
}

/*
 * Initialize system
 */
void
sys_init(int argc, char **argv)
{
/*	sysarg_init(argc, argv);	  */
	sysvid_init();
#ifdef ENABLE_JOYSTICK
	sysjoy_init();
#endif

#ifdef ENABLE_SOUND
	//if (sysarg_args_nosound == 0)
		syssnd_init();
#endif

/*	atexit(sys_shutdown);
	signal(SIGINT, exit);
	signal(SIGTERM, exit);	  */
}
/* eof */
