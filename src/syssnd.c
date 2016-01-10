/*
 * xrick/src/syssnd.c
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
#include <sgl.h>
#include "sdl/SDL.h"
#include <stdlib.h>
/*
#include <memory.h>
 */
#include "../include/config.h"

#ifdef ENABLE_SOUND

#include "../include/system.h"
#include "../include/game.h"
#include "../include/syssnd.h"
//#include "../include/debug.h"
//#include "../include/data.h"

#define NB_CHAN 6
//#define ADJVOL(S) (((S)*sndVol)/SDL_MIX_MAXVOLUME)
 extern PCM m_dat[8];
static U8 music_chn=0;
 extern sound_t *music_snd;
// extern 	char *lowram;
//char music_name[4][128];
//static U8 isAudioActive = FALSE;

void
syssnd_init(void)
{
  SDL_AudioSpec desired, obtained;
  //U16 c;

  if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
 //   IFDEBUG_AUDIO(
 //     sys_printf("xrick/audio: can not initialize audio subsystem\n");
   //   );
    return;
  }

  desired.freq = SYSSND_FREQ;
  desired.format = AUDIO_U8;
  desired.channels = SYSSND_CHANNELS;
  desired.samples = SYSSND_MIXSAMPLES;
#ifdef VBT
  desired.callback = syssnd_callback;
#endif
  desired.userdata = NULL;


  if (SDL_OpenAudio(&desired, &obtained) < 0) {
   // IFDEBUG_AUDIO(
   //   sys_printf("xrick/audio: can not open audio (%s)\n", SDL_GetError());
  //    );
    return;
  }
#ifdef VBT
  sndlock = SDL_CreateMutex();
  if (sndlock == NULL) {
    IFDEBUG_AUDIO(sys_printf("xrick/audio: can not create lock\n"););
    SDL_CloseAudio();
    return;
  }
#endif
  /*if (sysarg_args_vol != 0) {
    sndUVol = sysarg_args_vol;
    sndVol = SDL_MIX_MAXVOLUME * sndUVol / SYSSND_MAXVOL;
  }*/

 // for (c = 0; c < SYSSND_MIXCHANNELS; c++)
 //   channel[c].loop = 0;  /* deactivate */

	//isAudioActive = TRUE;
	//SDL_PauseAudio(0);
}

/*
 * Play a sound
 *
 * loop: number of times the sound should be played, -1 to loop forever
 * returns: channel number, or -1 if none was available
 *
 * NOTE if sound is already playing, simply reset it (i.e. can not have
 * twice the same sound playing -- tends to become noisy when too many
 * bad guys die at the same time).
 */
S8
syssnd_play(sound_t *sound, S8 loop)
{
	if (sound == NULL) return -1;
 	 U8 i;
/*	sound->buf==0x00232000
music_chn
	   vbtvbt
*/		   
//	if(sound->buf==0x00231000)
	if(sound->buf==0x00202000)
	{
		slSndFlush() ;
		if (!slPCMStat(&m_dat[music_chn]))
		{
			slPCMOff(&m_dat[music_chn]);
			slPCMOn(&m_dat[music_chn],sound->buf,sound->len);
		}
			return 0;
	}		  

	if(slSndPCMNum(_Mono)>-1)
	for(i=1;i<NB_CHAN;i++)
	{
		if(!slPCMStat(&m_dat[i]))
		{
				slSndFlush() ;
				//slPCMOff(&m_dat[i]);
				PCM_SetLoop(pcm, cnt_loop);

				slPCMOn(&m_dat[i],sound->buf,sound->len);
			//if(sound->buf!=0x00232000)
				//slSndFlush() ;
				break;
		}		 
	}
  //return c;
			//slSynch();
//  return 1;
			return i;
}

/*
 * Pause
 *
 * pause: TRUE or FALSE
 * clear: TRUE to cleanup all sounds and make sure we start from scratch
 */

void
syssnd_pause(U8 pause, U8 clear)
{
  //U8 c;

  //if (!isAudioActive) return;
 /*
  if (clear == TRUE) {
#ifdef VBT
    SDL_mutexP(sndlock);
#endif
//    for (c = 0; c < 4; c++)
//		slPCMOff(&m_dat[c]);
//      channel[c].loop = 0;
#ifdef VBT
    SDL_mutexV(sndlock);
#endif
  }		 */
	   
  if (pause == TRUE)
    SDL_PauseAudio(1);
  else
    SDL_PauseAudio(0);		
}
 /*
void
syssnd_waitsound()
{
		while(slPCMStat(&m_dat[0]));
}	*/

/*
 * Stop a sound
 */
void
syssnd_stopsound(sound_t *sound)
{

	U8 i;
//slPrint("bef slPCMOff(i)          ",slLocate(7,18));
	if (!sound) return;

//slPrint("bef slPCMOff(0x00202000)          ",slLocate(7,18));
	if(sound->buf==0x00202000)
	{
		if(slPCMStat(&m_dat[i]))
			slPCMOff(&m_dat[music_chn]);
	}
	else
	{
		for(i=1;i<NB_CHAN;i++)
			if(slPCMStat(&m_dat[i]))
				slPCMOff(&m_dat[i]);
	}
#ifdef VBT
	SDL_mutexP(sndlock);
#endif
//	for (i = 0; i < SYSSND_MIXCHANNELS; i++)
//		if (channel[i].snd == sound) end_channel(i);
#ifdef VBT
	SDL_mutexV(sndlock);
#endif
//slPrint("aft syssnd_stopsound          ",slLocate(7,18));
}

/*
 * Stops all channels.
 */
 /*
void
syssnd_stopall(void)
{
	U8 i;
#ifdef VBT
	SDL_mutexP(sndlock);
#endif
//	for (i = 0; i < SYSSND_MIXCHANNELS; i++)
//		if (channel[i].snd) end_channel(i);
#ifdef VBT
	SDL_mutexV(sndlock);
#endif
}
*/
/*
 * Load a sound.
 */

// long loaded=0;

sound_t *
syssnd_load(char *name)
{
	sound_t *s;
	SDL_RWops *context;
	SDL_AudioSpec audiospec;
				/*
loaded++;
//slPrintHex(loaded,slLocate(2,10));
//slPrint(name,slLocate(2,11));
slSynch();
			*/

	/* alloc context */
	context = malloc(sizeof(SDL_RWops));
	strcpy(context->hidden.stdio.name,name);
	/* alloc sound */
	s = malloc(sizeof(sound_t));
#ifdef DEBUG
	s->name = malloc(strlen(name) + 1);
	strncpy(s->name, name, strlen(name) + 1);
#endif
	/* read */
//SDL_LoadWAV_RW(context, 1, &audiospec, &(s->buf), &(s->len));
	if (!SDL_LoadWAV_RW(context, 1, &audiospec, &(s->buf), &(s->len)))
	{
		if(s->buf!=0x00202000)
			free(s);

		return NULL;
	}
	free (context);
	s->dispose = FALSE;
	 //slSynch();
	return s;
}

/*
 *
 */
 #ifdef VBT
void
syssnd_free(sound_t *s)
{
	if (!s) return;

	if (s->buf) SDL_FreeWAV(s->buf);
	s->buf = NULL;
	s->len = 0;		 
}
#endif

#endif /* ENABLE_SOUND */

/* eof */

