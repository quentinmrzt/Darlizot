/******************************************************************/
/* main.c                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 15/11/15                                    */
/******************************************************************/

#include "constant.h"
#include "game.h"
#include "draw.h"
#include "control.h"
#include "time.h"
#include "physic.h"

int main(int argc, char* argv[])
{
  int close, x_max, y_max, level, ammo=60, energy=1, load=0, nb_ennemi_spawn, map;
  s_information player;
  s_surface sprite;
  s_time time;

  list_ptr shots = NULL;
  list_ptr ennemi = NULL;
  TTF_Font *font = NULL;

  /****************************************************************************************************/
  /* INITIALIZE */

  /* initialize video system */
  SDL_Init(SDL_INIT_VIDEO);
  /* initialize ttf */
  TTF_Init();
  /* open font */
  SDL_WM_SetCaption("Gestion du texte avec SDL_ttf", NULL);
  font = TTF_OpenFont("font.ttf", 20);
  /* set the title bar */
  SDL_WM_SetCaption("S3", NULL);
  /* create window */
  sprite.screen = SDL_SetVideoMode(800, 400, 0, 0);

  /****************************************************************************************************/
  /* initialize variable */
  sprite = load_sprite(sprite);
  player = ini_player(player);
  time = ini_time(time);


  /* table */
  map = 0;
  size_tab(&x_max,&y_max);
  int tab[y_max][x_max];
  recup_map(x_max,y_max,tab);
  draw_tab(x_max,y_max,tab);

  printf("----------------------------------------\n");

  /*tableau.id = map;
  size_tab(&tableau.x_max,&tableau.y_max,map);
  recup_map(tableau.x_max, tableau.y_max, tableau.tab, map);*/

  close = 0;
  level= 1;
  nb_ennemi_spawn=0;

  printf("Nb map: %d\n",nb_map());

  while (!close) {
    time.current = SDL_GetTicks();

    /****************************************************************************************************/
    /* KEYBOARD AND MOUSE */
    close = quit(close);
    player = control(x_max,y_max,tab,player);
    shots = shooting(player,shots,&ammo,energy,&time);

    /****************************************************************************************************/
    /* GAME */
    ennemi = respawn(ennemi,&level,player,&time,&nb_ennemi_spawn,&load,x_max,y_max,tab);

    player = gravity(x_max,y_max,tab,player);
    ennemi_gravity(x_max,y_max,tab,ennemi,sprite);

    collision_bullet_ennemi(&shots,&ennemi);
    shots = wall_bang(shots,x_max,y_max,tab);

    ennemis_jump(x_max,y_max,tab,ennemi,player);
    time = duration_chrono(player,time,x_max);
    door_ennemy(x_max,y_max,tab,player,load,time);
    door_player(x_max,y_max,tab,player,time);

    /****************************************************************************************************/
    /* DRAW */
    draw(x_max,y_max,tab,sprite,player);
    draw_player(player,sprite);
    draw_ennemis(ennemi,sprite,player);
    draw_shooting(player,shots,sprite);
    draw_ammo(sprite,ammo);
    draw_health(player,sprite);
    draw_chrono(sprite.screen,font,player,time);

    /* ??? */
    ennemies_moves(ennemi,player,x_max,y_max,tab);
    anim_ennemis(ennemi,player,x_max,y_max,tab);


    /****************************************************************************************************/
    /* OTHER */
    if (player.position.y > 400) {
      player = ini_player(player);
    }
    if (player.movement > x_max*50) {
      close = 1;
    }

    a_and_z(x_max,y_max,tab,player);
    ennemi = killing(ennemi);

    SDL_UpdateRect(sprite.screen,0,0,0,0);
    /* ~ 12,5 fps */
    SDL_Delay(60);
  }

  /****************************************************************************************************/
  /* CLEAN */

  TTF_CloseFont(font);
  free_all_sprite(sprite);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
