/******************************************************************/
/* main.c                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Date creation: 20/09/15                                        */
/* Derniere modification: 20/09/15                                */
/******************************************************************/

#include "constant.h"
#include "game.h"

int main(int argc, char* argv[])
{
  int close;
  s_information player;
  s_surface sprite;
  SDL_Rect position,rcSrc;

  /****************************************************************************************************/
  /* INITIALIZE */

  /* initialize video system */
  SDL_Init(SDL_INIT_VIDEO);
  /* initialize ttf */
  TTF_Init();
  /* set the title bar */
  SDL_WM_SetCaption("S3", NULL);
  /* create window */
  sprite.screen = SDL_SetVideoMode(500, 500, 0, 0);
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
  position.x = 5;
  position.y = 5;


  /* loop for game */
  while (!close)
    {
      /****************************************************************************************************/
      /* KEYBOARD AND MOUSE */

      /* croix ou échap */
      close = quit(close);
      
      rcSrc.x = rcSrc.x+95;
      SDL_BlitSurface(sprite.player, &rcSrc, sprite.screen, &position);
 
      /****************************************************************************************************/
      /* OTHER */
      
      /* update the screen */
      SDL_UpdateRect(sprite.screen,0,0,0,0);

      /* fps */
      SDL_Delay(300);
    }

  /****************************************************************************************************/
  /* CLEAN */

  free_all_sprite(sprite);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
