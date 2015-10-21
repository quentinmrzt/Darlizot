/******************************************************************/
/* main.c                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 15/10/15                                    */
/******************************************************************/

#include "constant.h"
#include "game.h"
#include "draw.h"
#include "control.h"
#include "time.h"

int main(int argc, char* argv[])
{
  int close, x_max, y_max,i,nb_ennemi;
  s_information player;
  s_surface sprite;
  list_ptr shots = NULL;
  list_ptr ennemi= NULL;
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
  nb_ennemi=10;
  sprite = load_sprite(sprite);
  player = ini_player(player);
  /* table */
  size_tab(&x_max,&y_max);
  int tab[y_max][x_max];
  recup_map(x_max,y_max,tab);

  //draw_tab(x_max,y_max,tab);

  close = 0;
  nb_ennemi = 5;
  ennemi = ennemi_spawn(player,ennemi,nb_ennemi,x_max,y_max,tab);
  

  while (!close)
    { 
      /****************************************************************************************************/
      /* KEYBOARD AND MOUSE */

      close = quit(close);
      player = control(x_max,y_max,tab,player);
      shots = shooting(player,shots);
      player = gravity(x_max,y_max,tab,player);

      /****************************************************************************************************/
      /* DRAW */
      
      draw(x_max,y_max,tab,sprite);
      draw_shooting(player,shots,sprite);
      draw_ennemis(ennemi,sprite);
      SDL_BlitSurface(sprite.player, &player.rcSrc, sprite.screen, &player.position);
      
      //nb_ennemi=update_ennemi(nb_ennemi,ennemi);
      //printf("%d\n",ennemi->info.position.y);
      ennemi_gravity(x_max,y_max,tab,ennemi,sprite);

      printf("%d\n",player.rcSrc.x/75);
      //ennemi=ennemi_gravity(x_max,y_max,tab,ennemi);
      //printf("%d\n",ennemi->info.position.y);


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
