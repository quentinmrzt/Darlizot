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
  int close,i,previous_plateform_time=0,current_time=0,saut=0,state=2, sol=75;
  int NB_plateform=0;
  s_information player;
  s_surface sprite;
  SDL_Rect position,rcSrc, pos, test, plat;
  

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
  position.y = 305-sol;
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

      control(&position,&rcSrc,&state,&saut,&sol);

      /* croix ou échap */
      close = quit(close);
      
      SDL_BlitSurface(sprite.background, &test, sprite.screen, &pos);
      if(current_time-previous_plateform_time>500){
	
	for(i=0;i<NB_plateform;i++){
	  plat.x=plat.x+95+rand()%25;
	  plat.y=320+rand()%40;
	  SDL_BlitSurface(sprite.plateform, NULL, sprite.screen, &plat);
	}
	previous_plateform_time=current_time;
      }
      SDL_BlitSurface(sprite.player, &rcSrc, sprite.screen, &position);
      
      /* GRAVITY */
      if (saut==0)
	gravity(&position,&sol);
      /****************************************************************************************************/
      /* OTHER */
      
      /* update the screen */
      SDL_UpdateRect(sprite.screen,0,0,0,0);

      /* fps */
      SDL_Delay(80);
    }

  /****************************************************************************************************/
  /* CLEAN */

  free_all_sprite(sprite);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
