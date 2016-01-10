#include "sdl/SDL.h"
//#include "sdl/SDL_mixer.h"

//#define ACTION_REPLAY 1

#ifdef ACTION_REPLAY
#include "../sounds/walk.c"
#include "../sounds/bombshht.c"
#include "../sounds/bonus.c"
#include "../sounds/box.c"
#include "../sounds/bullet.c"
#include "../sounds/crawl.c"
#include "../sounds/die.c"
#include "../sounds/explode.c"
#include "../sounds/gameover.c"
#include "../sounds/jump.c"
#include "../sounds/pad.c"
#include "../sounds/sbonus1.c"
#include "../sounds/sbonus2.c"
#include "../sounds/stick.c"
#include "../sounds/tune0.c"	 
#include <sddrvs.dat>
#endif

#define NB_CHAN 6
 //#include <sddrvs.dat>
extern Uint8 control_status ;
Sint32 GetFileSize(int file_id);
void ClsFast(Uint8 *maplocation);
/*
 debug
*/
//Uint32 frame = 0;
//extern Image images[];
static unsigned char vbt_event[13][2];
static int current_event=0;
//int nb_event=0;
//static Uint32 count=0;
Uint32 previouscount=0;
//Uint16 multip=0;
PCM m_dat[8];
//Uint8 *lowram=(Uint8 *)0x00202000;

static Uint16 pad_asign[] = {
	PER_DGT_KU,
	PER_DGT_KD,
	PER_DGT_KR,
	PER_DGT_KL,
	PER_DGT_TA,
	PER_DGT_TB,
	PER_DGT_TC,
	PER_DGT_ST,
	PER_DGT_TX,
	PER_DGT_TY,
	PER_DGT_TZ,
	PER_DGT_TR,
	PER_DGT_TL,
};

static const Sint8	logtbl[] = {
/* 0 */		0, 
/* 1 */		1, 
/* 2 */		2, 2, 
/* 4 */		3, 3, 3, 3, 
/* 8 */		4, 4, 4, 4, 4, 4, 4, 4, 
/* 16 */	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
/* 32 */	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
			6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
/* 64 */	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
			7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
			7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
			7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
/* 128 */	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
			8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
			8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
			8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
			8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
			8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
			8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
			8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
	};

/*
 dummy
 */
//SDL_Screen scr;
//SDL_Screen *screen = &scr;

//--------------------------------------------------------------------------------------------------------------------------------------
/*int SDL_BlitSurface(Uint8 img, void *arg2, void *arg3, SDL_Rect *dest)
{
	//slBMPut((dest) == NULL ? 0 : ((SDL_Rect *)dest)->x, (dest) == NULL ? 0 : ((SDL_Rect *)dest)->y, ((dest) == NULL ? 0 : ((SDL_Rect *)dest)->x) + images[img].w - 1, ((dest) == NULL ? 0 : ((SDL_Rect *)dest)->y) + images[img].h - 1, images[img].data);
}	  */
/*
 #define SDL_SetVideoMode (arg1, arg2, arg3, arg4){}
 */
 SDL_Surface * SDL_SetVideoMode  (int width, int height, int bpp, Uint32 flags)
{
	unsigned char tv_mode;

	if(width==320)
	{
		tv_mode = TV_320x224;
		if(height==240) tv_mode = TV_320x240; 
	}

	if(width==352)
	{
		tv_mode = TV_352x224;
		if(height==240) tv_mode = TV_352x240; 
	}

	if(width==640)
	{
		tv_mode = TV_640x224;
		if(height==240) tv_mode = TV_640x240; 
	}
 
	slInitSystem(tv_mode, NULL, 1);

	if(bpp==8)
	{
		slColRAMMode(CRM16_1024);
		slCharNbg1(COL_TYPE_256 , CHAR_SIZE_1x1);
	}
	else
	{
		slColRAMMode(CRM32_1024);
		slCharNbg1(COL_TYPE_32768 , CHAR_SIZE_1x1);
	}

	slScrPosNbg1(toFIXED(0) , toFIXED(0));
    slInitBitMap(bmNBG1, BM_512x256, (void *)NBG1_CEL_ADR);
    slBMPaletteNbg1(1);
    // screen coordinates like in SDL
    slBitMapBase(0, 0);
    slScrAutoDisp(NBG0ON|NBG1ON);
	//slPriorityNbg1(1);
ClsFast(NBG0_MAP);
//	slPrint("C4 - 2008-2009   www.rockin-b.de",slLocate(4,25));
	//slTVOn();
	return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------
int SDL_SetColors(SDL_Surface *surface, 	SDL_Color *colors, int firstcolor, int ncolors)
{
	Uint16	palo[256];

	unsigned int i;

	for(i=0;i<ncolors;i++)
	{
		palo[i] = RGB(colors[i].r>>3,colors[i].g>>3,colors[i].b>>3);
	}
	Pal2CRAM(palo , (void *)NBG1_COL_ADR , ncolors);
	return 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------
/*int SDL_SetColorKey	(SDL_Surface *surface, Uint32 flag, Uint32 key)
{
	return 0;
}	*/
//--------------------------------------------------------------------------------------------------------------------------------------
static unsigned char initcddone=0;
int SDL_Init(Uint32 flags)
{
	//lowram=(Uint8 *)0x00202000;
#ifndef ACTION_REPLAY
	if(initcddone==0)
	{
		InitCD();
//		InitCDBlock();
		initcddone=1;
	}
//	 char toto[100];
//sprintf(toto,"filesxxx ** %s %s %s %s **",GFS_IdToName(0),GFS_IdToName(4),GFS_IdToName(5), GFS_IdToName(6));
//			slPrint(toto,slLocate(1,5));
#endif
	DMA_ScuInit();
	SDL_InitSubSystem(flags);
	return 0;
}




//--------------------------------------------------------------------------------------------------------------------------------------
int SDL_InitSubSystem(Uint32 flags)
{
	if(flags &= SDL_INIT_AUDIO)
	{
		char sound_map[] =  {0x00};//,0xff,0xff};//,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

#ifdef ACTION_REPLAY
		slInitSound(sddrvstsk , sizeof(sddrvstsk) , (Uint8 *)sound_map , sizeof(sound_map)) ;
#else
#define	SDDRV_NAME	"SDDRVS.TSK"
#define	SDDRV_SIZE	26610 //0x7000
#define	SDDRV_ADDR	0x00202000//0x6080000
	 /*
char toto[100];
	if(initcddone==0)
	{
		InitCD();
	}
			slSynch();
sprintf(toto,"files ** %s %s %s %s **",GFS_IdToName(0),GFS_IdToName(4),GFS_IdToName(5), GFS_IdToName(6));
//			slPrint(toto,slLocate(1,10));
			slSynch();
			*/
//	while(1);
		char *tutu = (unsigned char *)malloc(SDDRV_SIZE);
		GFS_Load(GFS_NameToId((Sint8*)SDDRV_NAME),0,(void *) tutu,SDDRV_SIZE);
		slInitSound(tutu , SDDRV_SIZE , (Uint8 *)sound_map , sizeof(sound_map)) ;
		free(tutu);	
		
//		if(GFS_Load(GFS_NameToId(SDDRV_NAME),0,(void *) 0x00202000,SDDRV_SIZE)>0)
//			slInitSound(0x00202000 , SDDRV_SIZE , (Uint8 *)sound_map , sizeof(sound_map)) ; 
		slDSPOff();		
#endif
	}
  
	if(flags &= SDL_INIT_TIMER)
	{
		TIM_FRT_INIT(TIM_CKS_128);
		TIM_FRT_SET_16(0);
	}		  

	return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------
/*Uint32 SDL_MapRGB (SDL_PixelFormat *format, Uint8 r, Uint8 g, Uint8 b)
{
	return 0;
}	  */
//--------------------------------------------------------------------------------------------------------------------------------------
/*int SDL_LockSurface(SDL_Surface *surface)
{
	return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void SDL_UnlockSurface(SDL_Surface *surface)
{

}												*/
//--------------------------------------------------------------------------------------------------------------------------------------
int SDL_OpenAudio(SDL_AudioSpec *desired, SDL_AudioSpec *obtained)
{
	Sint32		oct, shift_freq, fns;
	Uint8 i;
	Sint8 err=0;
	memcpy(obtained,desired,sizeof(SDL_AudioSpec));
			   
	for (i=0; i<NB_CHAN; i++)
	{
		m_dat[i].mode = 0;

		if(desired->format==AUDIO_U8 || desired->format==AUDIO_S8)  m_dat[i].mode|=_PCM8Bit;
		if(desired->format==AUDIO_U16LSB || desired->format==AUDIO_S16LSB)  m_dat[i].mode|=_PCM16Bit;
		if(desired->channels==1) m_dat[i].mode|=_Mono;
		if(desired->channels==2) m_dat[i].mode|=_Stereo;

		oct			= PCM_CALC_OCT(desired->freq);
		shift_freq = PCM_CALC_SHIFT_FREQ(oct);
		fns			= PCM_CALC_FNS(desired->freq, shift_freq);

		m_dat[i].pitch =PCM_SET_PITCH_WORD(oct, fns);
		m_dat[i].level = 127;
		m_dat[i].channel = i*2;
	   	/*
		if(!slPCMParmChange(&m_dat[i]))
		{
			err--;
		}		*/
	}
	
	return err;
}
//--------------------------------------------------------------------------------------------------------------------------------------
/*void SDL_UpdateRects (SDL_Surface *screen, int numrects, SDL_Rect *rects)
{

}
//--------------------------------------------------------------------------------------------------------------------------------------
void SDL_UpdateRect (SDL_Surface *screen, Sint32 x, Sint32 y, Uint32 w, Uint32 h)
{

}	  */
//--------------------------------------------------------------------------------------------------------------------------------------
/*
int SDL_mutexP(SDL_mutex *mutex)
{
	return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------
int SDL_mutexV(SDL_mutex *mutex)
{
	return 0;
}	 
//--------------------------------------------------------------------------------------------------------------------------------------
void SDL_CloseAudio(void)
{
//	slPCMOff(&m_dat);
}	  */
//--------------------------------------------------------------------------------------------------------------------------------------
void SDL_PauseAudio(int pause_on)
{
	Uint8 i;
	for (i=0;i<NB_CHAN;i++)
		if(slPCMStat(&m_dat[i]))
			slPCMOff(&m_dat[i]);
}
//--------------------------------------------------------------------------------------------------------------------------------------
/*
SDL_mutex * SDL_CreateMutex(void)
{

}
//--------------------------------------------------------------------------------------------------------------------------------------
void SDL_FreeWAV(Uint8 *audio_buf)
{
	//free(audio_buf);
}
*/
//--------------------------------------------------------------------------------------------------------------------------------------
unsigned char cdir=0;

SDL_AudioSpec * SDL_LoadWAV_RW(SDL_RWops *src, int freesrc, SDL_AudioSpec *spec, Uint8 **audio_buf, Uint32 *audio_len)
{
#ifndef ACTION_REPLAY
	Uint8 i, fileId;
	long fileSize;
    char directory[15];
	char filename[15];
	unsigned char *mem_buf;

	if(strlen(src->hidden.stdio.name)>5)
	{
		for(i=0;i<strlen(src->hidden.stdio.name);i++)
			if (src->hidden.stdio.name[i]=='/')	break;

		if(i<strlen(src->hidden.stdio.name) )
		{
			strncpy(directory,src->hidden.stdio.name,i);
			 directory[i]='\0';

			strcpy(filename,&src->hidden.stdio.name[i+1]);
			if(cdir==0)
			{		  
				i=0;
				while (directory[i])
				{
					directory[i]= toupper(directory[i]);
					i++;
				}

				ChangeDir(directory);
				cdir=1;
//				slPrint(directory,slLocate(1,1));

			}
		}
//		slPrint(filename,slLocate(1,2));
	}
	else
		strcpy(filename,src->hidden.stdio.name);
	i=0;
	while (filename[i])
	{
		filename[i]= toupper(filename[i]);
		i++;
	}	  

	fileId = GFS_NameToId(filename);
	fileSize = GetFileSize(fileId);
 
	if(strncmp("TUNE",filename,4)==0)
	{
		mem_buf =  (Uint8 *)(0x00202000);
	}
	else
		mem_buf = malloc(fileSize)	;

	GFS_Load(fileId, 0, mem_buf, fileSize);
	audio_buf[0] = mem_buf;
	*audio_len = fileSize;
	if (fileSize<0x900)
	*audio_len = 0x900;
	//ChangeDir(NULL);
		return spec;
#else

	 if(strcmp("sounds/gameover.wav",src->hidden.stdio.name)==0)
	{
		audio_buf[0] = gameover;
		*audio_len = sizeof(gameover);
	}

	 if(strcmp("sounds/sbonus2.wav",src->hidden.stdio.name)==0)
	{
		audio_buf[0] = sbonus2;
		*audio_len = sizeof(sbonus2);
	}

	 if(strcmp(src->hidden.stdio.name,"sounds/bullet.wav")==0)
	{
		audio_buf[0] = bullet;
		*audio_len = sizeof(bullet);
	}

	 if(strcmp(src->hidden.stdio.name,"sounds/bombshht.wav")==0)
	{
		audio_buf[0] = bombshht;
		*audio_len = sizeof(bombshht);
	}

	 if(strcmp(src->hidden.stdio.name,"sounds/explode.wav")==0)
	{
 		audio_buf[0] = explode;
		*audio_len = sizeof(explode);
	}

	 if(strcmp(src->hidden.stdio.name,"sounds/stick.wav")==0)
	{
   		audio_buf[0] = stick;
		*audio_len = sizeof(stick);
	}

	 if(strcmp(src->hidden.stdio.name,"sounds/walk.wav")==0)
	{
   		audio_buf[0] = walk;
		*audio_len = 0x900;//sizeof(walk);
	}

	 if(strcmp(src->hidden.stdio.name,"sounds/crawl.wav")==0)
	{
   		audio_buf[0] = crawl;
		*audio_len = sizeof(crawl);
	}

	 if(strcmp(src->hidden.stdio.name,"sounds/jump.wav")==0)
	{
   		audio_buf[0] = jump;
		*audio_len = sizeof(jump);
	}

	 if(strcmp(src->hidden.stdio.name,"sounds/pad.wav")==0)
	{
   		audio_buf[0] = pad;
		*audio_len = sizeof(pad);
	}

	 if(strcmp(src->hidden.stdio.name,"sounds/box.wav")==0)
	{
		audio_buf[0] = box;
		*audio_len = sizeof(box);
	}

	 if(strcmp(src->hidden.stdio.name,"sounds/bonus.wav")==0)
	{
		audio_buf[0] = bonus;
		*audio_len = sizeof(bonus);
	}

	 if(strcmp(src->hidden.stdio.name,"sounds/sbonus1.wav")==0)
	{
		audio_buf[0] = sbonus1;
		*audio_len = sizeof(sbonus1);
	}

	 if(strcmp(src->hidden.stdio.name,"sounds/die.wav")==0)
	{
		audio_buf[0] = die;
		*audio_len = sizeof(die);
	}


	 if(strcmp(src->hidden.stdio.name,"sounds/tune0.wav")==0)
	{
		audio_buf[0] = tune0;
		*audio_len = sizeof(tune0);
	}

	 if(strcmp(src->hidden.stdio.name,"sounds/tune1.wav")==0)
	{
		audio_buf[0] = tune0;
		*audio_len = sizeof(tune0);
	}

	 if(strcmp(src->hidden.stdio.name,"sounds/tune2.wav")==0)
	{
		audio_buf[0] = tune0;
		*audio_len = sizeof(tune0);
	}

	 if(strcmp(src->hidden.stdio.name,"sounds/tune3.wav")==0)
	{
		audio_buf[0] = tune0;
		*audio_len = sizeof(tune0);
	}	

		 if(strcmp(src->hidden.stdio.name,"sounds/tune4.wav")==0)
	{
		audio_buf[0] = tune0;
		*audio_len = sizeof(tune0);
	}	

	 if(strcmp(src->hidden.stdio.name,"sounds/tune5.wav")==0)
	{
		audio_buf[0] = tune0;
		*audio_len = sizeof(tune0);
	}	  
#endif


	return spec;
}
//--------------------------------------------------------------------------------------------------------------------------------------
/*SDL_Rect ** SDL_ListModes(SDL_PixelFormat *format, Uint32 flags)
{
	return (SDL_Rect **)-1;
}		  */
//--------------------------------------------------------------------------------------------------------------------------------------
/*void SDL_WM_SetCaption(const char *title, const char *icon)
{

}		 
//--------------------------------------------------------------------------------------------------------------------------------------
void SDL_WM_GetCaption(char **title, char **icon)
{

}
//--------------------------------------------------------------------------------------------------------------------------------------
SDL_Surface * SDL_CreateRGBSurfaceFrom(void *pixels,
			int width, int height, int depth, int pitch,
			Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
{

}
//--------------------------------------------------------------------------------------------------------------------------------------
void SDL_WM_SetIcon(SDL_Surface *icon, Uint8 *mask)
{

}		  */
/*
<vbt> :) looks easy but hard for me
<MartinMan> hmm
<MartinMan> ah
<vbt> for me at first poll I can add multiple events
<MartinMan> no, you just need to return 1 at a time
<vbt> then return the first event not executed
<MartinMan> First, make a list of all supported buttons/directions
<MartinMan> then you assign a number (starting from 0) to each of them
<MartinMan> then when poll() is called, you increase the number until you find a changed button/direction
<MartinMan> and if no more left, return false
<MartinMan> of course you need to store and update a joypad state variable to look for changed buttons/directions
*/
int SDL_PollEvent(SDL_Event *event)
{
	//Uint16 push = 0, 
	Uint16 data = 0;
	Uint8 i,found=0;

	//event->type = SDL_NOEVENT;
	//event->key.keysym.sym = SDLK_FIRST;	  

	if(Per_Connect1) {
		//push = ~Smpc_Peripheral[0].push;
		data = ~Smpc_Peripheral[0].data;
	}
/*	if(Per_Connect2) {
		push |= ~Smpc_Peripheral[15].push;
		data |= ~Smpc_Peripheral[15].data;
	}				 */
	/*
	 quit?
	*/
	
	if(data & PER_DGT_ST && data & PER_DGT_TA && data & PER_DGT_TB && data & PER_DGT_TC) 
	{
		event->type = SDL_QUIT;
		return 1;
	}

	for (i=0;i <13; i++)
	{
		if(data & pad_asign[i])
		{
			//if(vbt_event[i][0]!=SDL_KEYDOWN)
			{
				vbt_event[i][0]=SDL_KEYDOWN;
				vbt_event[i][1]=1;
			}
		}
		else/* if(push & pad_asign[i])		 */
		{
			
			if(vbt_event[i][0]==SDL_KEYDOWN)
			{
				vbt_event[i][0]=SDL_KEYUP;
				vbt_event[i][1]=1;
				 //control_status =0;
			}		  
			else	// if(vbt_event[i][0]==SDL_KEYUP)
			{
				vbt_event[i][0]=0;
				vbt_event[i][1]=0;
			}
			//else
			  //	vbt_event[i][1]=0;				 
		}
	}	  
	for (i=0;i <13; i++)
	{
		if (vbt_event[i][1]==1)
		{
			//slPrintHex(i,slLocate(3,21));
			current_event=i;
			found=1;
			break;
		}
	}

	if(found)
	{
		//char toto[500];
		//sprintf(toto,"evt %d action %d key %d ctr%d ",vbt_event[current_event][0],vbt_event[current_event][1],event->key.keysym.sym,control_status );
		//		slPrint(toto,slLocate(3,25));
		event->type = 	 vbt_event[current_event][0];
		//event->type = SDL_KEYDOWN;

		switch(current_event)
		{
			case 4:/*PER_DGT_TA: */
			event->key.keysym.sym = SDLK_SPACE;
//			slPrint("bouton A",slLocate(3,24));
			break;
			
			case 5:/*PER_DGT_TB: */
			event->key.keysym.sym = SDLK_F8;
//			slPrint("bouton B",slLocate(3,24));
			break;	
			
			case 6:/*PER_DGT_TC: */
			event->key.keysym.sym = SDLK_p;
//			slPrint("bouton C",slLocate(3,24));
			break;	

//			case 7:/*PER_DGT_ST: */
				//slPrint("gros connard",slLocate(3,24));
//			event->key.keysym.sym = SDLK_ESCAPE;
//			break;	

			case 3:/*PER_DGT_KL: */
				event->key.keysym.sym = SDLK_z;
			break;	

			case 2:/*PER_DGT_KR: */
				event->key.keysym.sym = SDLK_x;
			break;	

			case 0:/*PER_DGT_KU: */
				//slPrint("gros ggggg",slLocate(3,20));
			event->key.keysym.sym = SDLK_o;
			break;	

			case 1:/*PER_DGT_KD: */
				//slPrint("gros ggggg",slLocate(3,20));
			event->key.keysym.sym = SDLK_k;
			break;	

			default:
				//slPrint("pas trouvé",slLocate(3,20));
				//event->key.keysym.sym =999;
				event->type = SDL_NOEVENT;
				event->key.keysym.sym = SDLK_FIRST;	  
				break;
		}
	//slSynch();
	//while(1);
		return 1;
	}
	for (i=0;i <13; i++)
	{
		if (vbt_event[i][1]==1)
		{
				vbt_event[i][0]=0;
				vbt_event[i][1]=0;
		}
	}
	control_status =0;
	event->type = SDL_NOEVENT;
	event->key.keysym.sym = SDLK_FIRST;	  
	//slSynch();
	return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void SDL_WaitEvent(SDL_Event *event)
{
	//event->type = SDL_NOEVENT;

	if (event->type== SDL_KEYUP) {
		event->type = SDL_NOEVENT;
		event->key.keysym.sym = SDLK_FIRST;	
		return;
	}
 
	do {	
		 /*
		 take pressed buttons of both pads, to enable two player
		 */
		//Uint16 push = 0;
		Uint16 data = 0;
		if(Per_Connect1) {
			//push = ~Smpc_Peripheral[0].push;
			data = ~Smpc_Peripheral[0].data;
		}
/*		if(Per_Connect2) {
			push |= ~Smpc_Peripheral[15].push;
			data |= ~Smpc_Peripheral[15].data;
		}			   */
		/*
		 quit?
		*/
		if(data & PER_DGT_ST && data & PER_DGT_TA && data & PER_DGT_TB && data & PER_DGT_TC) {
			event->type = SDL_QUIT;
			return;
		}		
		/*
			move cursor
		*/
/*		if(data & PER_DGT_ST) {
			event->type = SDL_KEYDOWN;
			event->key.keysym.sym = SDLK_SPACE;
			return;
		}

		if(!(data & PER_DGT_ST) 
			&& event->key.keysym.sym == SDLK_SPACE
			&& event->type                     == SDL_KEYDOWN) {
			event->type = SDL_KEYUP;
			return;
		}
  */
		if(data & PER_DGT_TA) {
			event->type = SDL_KEYDOWN;
			event->key.keysym.sym = SDLK_SPACE;
			return;
		}
		
		if(!(data & PER_DGT_TA) 
			&& event->key.keysym.sym == SDLK_SPACE
			&& event->type                     == SDL_KEYDOWN) {
			event->type = SDL_KEYUP;
			return;
		}

		if(data & PER_DGT_TB) {
			event->type = SDL_KEYDOWN;
			event->key.keysym.sym = SDLK_e;
			return;
		}		
		
		if(!(data & PER_DGT_TB) 
			&& event->key.keysym.sym == SDLK_e
			&& event->type                     == SDL_KEYDOWN) {
			event->type = SDL_KEYUP;
			return;
		}

		if(data & PER_DGT_TC) {
			event->type = SDL_KEYDOWN;
			event->key.keysym.sym = SDLK_p;
			return;
		}
		
		if(!(data & PER_DGT_TC) 
			&& event->key.keysym.sym == SDLK_p
			&& event->type                     == SDL_KEYDOWN) {
			event->type = SDL_KEYUP;
			return;
		}

		if(data & PER_DGT_KL) {
			event->type = SDL_KEYDOWN;
			event->key.keysym.sym = SDLK_z;
			return;
		}
		
		if(!(data & PER_DGT_KL) 
			&& event->key.keysym.sym == SDLK_z
			&& event->type                     == SDL_KEYDOWN) {
			event->type = SDL_KEYUP;
			return;
		}

		if(data & PER_DGT_KR) {
			event->type = SDL_KEYDOWN;
			event->key.keysym.sym = SDLK_x;
			return;
		}
		
		if(!(data & PER_DGT_KR) 
			&& event->key.keysym.sym == SDLK_x
			&& event->type                     == SDL_KEYDOWN) {
			event->type = SDL_KEYUP;
			return;
		}

		if(data & PER_DGT_KU) {
			event->type = SDL_KEYDOWN;
			event->key.keysym.sym = SDLK_o;
			return;
		}
		
		if(!(data & PER_DGT_KU) 
			&& event->key.keysym.sym == SDLK_o
			&& event->type                     == SDL_KEYDOWN) {
			event->type = SDL_KEYUP;
			return;
		}

		if(data & PER_DGT_KD) {
			event->type = SDL_KEYDOWN;
			event->key.keysym.sym = SDLK_k;
			return;
		}
		
		if(!(data & PER_DGT_KD) 
			&& event->key.keysym.sym == SDLK_k
			&& event->type                     == SDL_KEYDOWN) {
			event->type = SDL_KEYUP;
			return;
		}

		//slSynch();
		
	} while(event->type == SDL_NOEVENT);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void SDL_DestroyMutex(SDL_mutex *mutex)
{

}
//--------------------------------------------------------------------------------------------------------------------------------------
Uint32 SDL_GetTicks(void)
{
	int			imask;
	//imask = get_imask();
   // set_imask(15);
   /*
	Uint16 count1 = TIM_FRT_GET_16();
	if (count1<previouscount)
		multip++;
	count=((multip<<16)+count1);
	
	previouscount=count1;				*/

	/*return TIM_FRT_CNT_TO_MCR(count)/1000;		 */
	/*char tutu[20];
	sprintf(tutu,"%d",previouscount),
//	slPrint(tutu,slLocate(10,10));					 */
	//previouscount += 100;//TIM_FRT_CNT_TO_MCR(TIM_FRT_GET_16()) / 1000; 
	previouscount += TIM_FRT_CNT_TO_MCR(TIM_FRT_GET_16()) / 1000; 
	TIM_FRT_SET_16(0); 
    //set_imask(imask);
	//
	return previouscount;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void SDL_Delay(int delay)
{
/*	Uint16 cnt_delay;
	//Uint16 count1 = TIM_FRT_GET_16();
	cnt_delay= TIM_FRT_MCR_TO_CNT(delay);
	TIM_FRT_DELAY_16(cnt_delay);			   
	TIM_FRT_SET_16(0);			 */
}
//--------------------------------------------------------------------------------------------------------------------------------------
/*
void ss_main()
{
	//InitCD();
}			  */

void ClsFast(Uint8 *maplocation){

//This version clears any tilemap.
int i;
Uint8 *ASCITile;

ASCITile = (Uint8 *) maplocation;
for(i = 0; i <= 0xFA00; i++) *(ASCITile++) = 0x00;
}

//--------------------------------------------------------------------------------------------------------------------------------------
void Pal2CRAM( Uint16 *Pal_Data , void *Col_Adr , Uint32 suu )
{
	Uint16 i;
	Uint16 *VRAM;

	VRAM = (Uint16 *)Col_Adr;

	for( i = 0; i < suu; i++ )
		*(VRAM++) = *(Pal_Data++);
}
//--------------------------------------------------------------------------------------------------------------------------------------
#ifndef ACTION_REPLAY
void InitCD()
{
	Uint32 lib_work[GFS_WORK_SIZE(MAX_OPEN) / sizeof(Uint32)];
	GfsDirTbl dirtbl; 

#ifndef ACTION_REPLAY

#ifdef VBT
CdUnlock();
#endif

//	Sint32 ret;
 
	CDC_CdInit(0x00,0x00,0x05,0x0f);
    GFS_DIRTBL_TYPE(&dirtbl) = GFS_DIR_NAME;
    GFS_DIRTBL_DIRNAME(&dirtbl) = dir_name;
    GFS_DIRTBL_NDIR(&dirtbl) = MAX_DIR;
    GFS_Init(MAX_OPEN, lib_work, &dirtbl);
#endif	    
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ChangeDir(char *dirname)
{
#ifndef ACTION_REPLAY	
    Sint32 fid;
	GfsDirTbl dirtbl; 
	
    fid = GFS_NameToId((Sint8 *)dirname);

	GFS_DIRTBL_TYPE(&dirtbl) = GFS_DIR_NAME;
	GFS_DIRTBL_DIRNAME(&dirtbl) = dir_name;
	GFS_DIRTBL_NDIR(&dirtbl) = MAX_DIR;

	GFS_LoadDir(fid,&dirtbl);
//	GFS_SetDir(&dirtbl) ;
	for (;;) {
		if (GFS_SetDir(&dirtbl) == 0) {
			break;
		}				 
	}	 
#endif	
}
//--------------------------------------------------------------------------------------------------------------------------------------
/*
int LoadFile(char *name, Uint8 *location, Uint32 size)
{
#ifndef ACTION_REPLAY	
	Sint32 		fid, i;

	for (i = 0; i < 10; i++) 
	{
		fid = GFS_NameToId(name);
		if (fid >= 0) {
			GFS_Load(fid, 0, location, size);
			return 0;
		}
	}
	return -1;
#else
    return 0;
#endif	
}
*/
//--------------------------------------------------------------------------------------------------------------------------------------
Sint32 GetFileSize(int file_id)
{
#ifndef ACTION_REPLAY	
	GfsHn gfs;
    //Sint32 sctsize, nsct; 
	Sint32	lastsize;
	//Sint32 lastsize;
    
    gfs = GFS_Open(file_id);
 
    //GFS_GetFileSize(gfs, &sctsize, &nsct, &lastsize);
	/*
    GFS_Close(gfs);
//	return sctsize*nsct;
	if(sctsize*(nsct-1)<0)
		return lastsize;
	else
		return (sctsize*(nsct-1) + lastsize);
	*/  
	GFS_GetFileInfo(gfs,&file_id,NULL,&lastsize,NULL);
    GFS_Close(gfs);
	return lastsize;	  
#else
    return 300;
#endif	
}
//--------------------------------------------------------------------------------------------------------------------------------------

#ifdef VBT

 #include "sega_cdc.h"

int CdUnlock (void)
{

Sint32 ret;
CdcStat stat;
volatile int delay;
unsigned int vbt=0;
SYS_CDINIT1(3);

SYS_CDINIT2();

do {

for(delay = 100000; delay; delay--);

ret = CDC_GetCurStat(&stat);
} while ((ret != 0) || (CDC_STAT_STATUS(&stat) == 0xff));


return (int) CDC_STAT_STATUS(&stat);

}



#endif
#endif
