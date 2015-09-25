/******************************************************************/
/* main.c                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Date creation: 20/09/15                                        */
/* Derniere modification: 20/09/15                                */
/******************************************************************/

#include "constant.h"
#include "game.h"
#include "time.h"

int main(int argc, char* argv[])
{
  int close,i,previous_plateform_time,current_time=0;
  int NB_plateform=20;
  s_information player;
  s_surface sprite;
  SDL_Rect position,rcSrc, pos, test, plat;
  Uint8 *keystate = SDL_GetKeyState(NULL);
  int state=2;

  /****************************************************************************************************/
  /* INITIALIZE */

  /* initialize video system */
  SDL_Init(SDL_INIT_VIDEO);
  /* initialize ttf */
  TTF_Init();
  /* set the title bar */
  SDL_WM_SetCaption("S3", NULL);
  /* create window */
  sprite.screen = SDL_SetVideoMode(800, 400, 0, 0);
  /* load sprite */
  sprite = load_sprite(sprite);

  /* initialize variable */
  close = 0;
  /* position dans le sprite */
  rcSrc.x = 0;
  rcSrc.y = 0;
  rcSrc.w = 95;
  rcSrc.h = 95;
  /* position dans l'écran */
  position.x = 0;
  position.y = 400-95;
  test.x = 0;
  test.y = 0;
  test.w = 800;
  test.h = 400;
  pos.x = 0;
  pos.y = 0;
  plat.x = 20;
  plat.y = 320+rand()%40;

  /* loop for game */
  while (!close)
    {
      /****************************************************************************************************/
      /* TIME */
      current_time=SDL_GetTicks();
      
      /* KEYBOARD AND MOUSE */

       if (keystate[SDLK_LEFT]){
	 if (state!=1)
	   state=1;
	 position.x-=20;
	 if (rcSrc.x<=7*95 || rcSrc.x==13*95)
	   rcSrc.x=8*95;
	 else
	   rcSrc.x+=95;
       }
       else
	 {
	   if (state==1)
	     rcSrc.x=7*95;
	 }

       if (keystate[SDLK_RIGHT]){
	 if (state!=0)
	   state=0;
	 position.x+=20;
	 if (rcSrc.x==0 || rcSrc.x>=6*95)
	   rcSrc.x=95;
	 else
	   rcSrc.x+=95;
       }
       else 
	 {
	   if (state==0)
	     rcSrc.x=state;
	 }
	 
       if (keystate[SDLK_UP]){

       }
       if (keystate[SDLK_DOWN]){

      }


      /* croix ou échap */
      close = quit(close);
      
      SDL_BlitSurface(sprite.background, &test, sprite.screen, &pos);
      if(current_time-previous_plateform_time>5000){
	previous_plateform_time=current_time;
	for(i=0;i<NB_plateform;i++){
	  plat.x=plat.x+95+rand()%25;
	  plat.y=320+rand()%40;
	  SDL_BlitSurface(sprite.plateform, NULL, sprite.screen, &plat);
	}
      }
      SDL_BlitSurface(sprite.player, &rcSrc, sprite.screen, &position);
      
 
      /****************************************************************************************************/
      /* OTHER */
      
      /* update the screen */
      SDL_UpdateRect(sprite.screen,0,0,0,0);

      /* fps */
      SDL_Delay(90);
    }

  /****************************************************************************************************/
  /* CLEAN */

  free_all_sprite(sprite);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
