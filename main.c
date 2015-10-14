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
  int close,i;
  int x_max, y_max;
  s_information player;
  s_surface sprite;



  SDL_Rect pos_sprite,pos_screen;
  int x,y;


  list_info enemies=NULL;
  list_info shots=NULL;

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

  player = ini_player(player); 



  /* pos in sprite: block / sky */
  pos_sprite.x = 0;
  pos_sprite.y = 0;
  pos_sprite.w = 50;
  pos_sprite.h = 50;


  /* tableau */
  size_tab(&x_max,&y_max);
  int tab[y_max][x_max];
  recup_map(tab);


  /* loop for game */
  while (!close)
    { 
      /* KEYBOARD AND MOUSE */
      /* croix ou échap */
      close = quit(close);
      /* clavier */
      control(tab,&player);

      /* tableau */
      /* draw floor */
      for (y=0;y<400/50;y++) {
	for (x=0;x<800/50;x++) {
	  pos_screen.x = x*50;
	  pos_screen.y = y*50;

	  if (tab[y][x] == 0) {
	    SDL_BlitSurface(sprite.background, &pos_sprite, sprite.screen, &pos_screen);
	  }

	  if (tab[y][x] == -1) {
	    SDL_BlitSurface(sprite.block, &pos_sprite, sprite.screen, &pos_screen);
	  }
	}
      }


      SDL_BlitSurface(sprite.player, &player.rcSrc, sprite.screen, &player.position);

      
      /* GRAVITY */
      if (player.jump == 0)
	gravity(&player,tab);

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
