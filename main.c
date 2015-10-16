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
  list_ptr shots = NULL;

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

  int x,y;
  for (y=0;y<y_max;y++) {
    for (x=0;x<x_max;x++) {
      if (tab[y][x] == 0) {
	printf(" %d ",tab[y][x]);
      } else {
	printf("%d ",tab[y][x]);
      }
    }
    printf("\n");
  }

  printf("%d %d\n",x_max,y_max);

  while (!close)
    { 
      /****************************************************************************************************/
      /* KEYBOARD AND MOUSE */

      close = quit(close);
      player = control(x_max,y_max,tab,player);
      player = gravity(x_max,y_max,tab,player);
      shots=shooting(player,shots);

      /****************************************************************************************************/
      /* DRAW */

      draw(x_max,y_max,tab,sprite);
      SDL_BlitSurface(sprite.player, &player.rcSrc, sprite.screen, &player.position);
      if (shots!=NULL)
	printf("1\n");

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
