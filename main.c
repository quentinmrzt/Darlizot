/******************************************************************/
/* main.c                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 15/10/15                                    */
/******************************************************************/

#include "constant.h"
#include "game.h"
#include "time.h"


int main(int argc, char* argv[])
{
  int close, x_max, y_max;
  s_information player;
  s_surface sprite;

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

  /****************************************************************************************************/
  /* initialize variable */

  sprite = load_sprite(sprite);
  player = ini_player(player);
  close = 0;
  /* table */
  size_tab(&x_max,&y_max);
  int tab[y_max][x_max];
  recup_map(x_max,y_max,tab);

  while (!close)
    { 
      /****************************************************************************************************/
      /* KEYBOARD AND MOUSE */
      close = quit(close);

      control(x_max,y_max,tab,&player);

      if (player.jump == 0) {
	player = gravity(x_max,y_max,tab,player);
      }

      /****************************************************************************************************/
      /* DRAW */

      /* decor */
      draw(x_max,y_max,tab,sprite);

      /* player */
      SDL_BlitSurface(sprite.player, &player.rcSrc, sprite.screen, &player.position);

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
