/******************************************************************/
/* main.c                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 16/11/15                                    */
/******************************************************************/

#include "constant.h"
#include "game.h"
#include "draw.h"
#include "control.h"
#include "time.h"
#include "physic.h" 

int main(int argc, char* argv[])
{
    int close, x_max, y_max, level, ammo=60, energy=1, load=0, nb_ennemi_spawn, map, previous_map, choice, pos_choice,score=0;
  s_information player;
  s_surface sprite;
  s_time time;

  list_ptr shots = NULL;
  list_ptr ennemi = NULL;
  list_ptr army_shots = NULL;

  TTF_Font *font = NULL;

  /****************************************************************************************************/
  /* INITIALIZE */
  /* initialize video system */
  SDL_Init(SDL_INIT_VIDEO);
  /* initialize ttf */
  TTF_Init();
  /* open font */
  SDL_WM_SetCaption("Gestion du texte avec SDL_ttf", NULL);
  font = TTF_OpenFont("font.ttf", 21);
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
  if (map <= 1) {
    time.time_max = 0;
    time.chrono = 0;
  }
  previous_map = 0;
  size_tab(&x_max,&y_max,map);
  int tab[8][100];
  recup_map(x_max,y_max,tab,map);

  close = 0;
  level = 0;
  nb_ennemi_spawn=0;
  choice = 1;
  while (!close) {
    time.current = SDL_GetTicks();

    /****************************************************************************************************/
    /* KEYBOARD AND MOUSE */
    close = quit(close);

    if (map == 0 && player.movement < x_max*50/2 && choice == 0) {
      player = control_auto(x_max,y_max,tab,player,x_max*50/2);
    } else if (map == 0 && choice == 1) {
      // 1: on commence le jeu
      player = control_auto(x_max,y_max,tab,player,x_max*50);
    } else if (map != 0 && player.movement < 50) {
      player = control_auto(x_max,y_max,tab,player,50);
    } else if (map != 0 && player.movement >= (x_max-1)*50) {
      player = control_auto(x_max,y_max,tab,player,x_max*50);
    } else {
      player = control(x_max,y_max,tab,player);
    }

    if (map == 0 && choice == 0) {
      //pos_choice = control_menu();
    }

    shots = shooting(player,shots,&ammo,energy,&time);
    a_and_z(x_max,y_max,tab,player);

    /****************************************************************************************************/
    /* GAME */
    killing(&ennemi);
    ennemi = respawn(ennemi,&level,player,&time,&nb_ennemi_spawn,&load,x_max,y_max,tab,map);

    player = gravity(x_max,y_max,tab,player);
    ennemi_gravity(x_max,y_max,tab,ennemi,sprite);

    shots = wall_bang(shots,x_max,y_max,tab);
    army_shots = wall_bang(army_shots,x_max,y_max,tab);

    ennemis_jump(x_max,y_max,tab,ennemi,player);
    time = duration_chrono(player,time,x_max);

    door_ennemy(x_max,y_max,tab,player,load,time,map);
    door_player(x_max,y_max,tab,player,time);

    army_shots = collision_bullet_player(army_shots,&player,&time);

    army_shots = ennemis_shots(ennemi,army_shots,player,x_max,y_max,tab,&time);


    /****************************************************************************************************/
    /* DRAW */
    draw(x_max,y_max,tab,sprite,player);
    draw_player(player,sprite,&time,map);    
    draw_ennemis(ennemi,sprite,player);
    collision_bullet_ennemi(&shots,&ennemi,&score,level);
    draw_shooting(player,shots,sprite);
    draw_ennemis_shooting(army_shots,sprite,player);    
    draw_ammo(sprite,ammo);
    draw_health(player,sprite);
    draw_chrono(sprite.screen,font,time,map);
    draw_level(sprite.screen,font,level,map);
    ennemies_moves(ennemi,player,x_max,y_max,tab);
    anim_ennemis(ennemi,player,x_max,y_max,tab);

    /****************************************************************************************************/
    /* OTHER */
    if (player.position.y > 400) {
      player = ini_player(player);
    }
    
    if (player.movement > x_max*50) {
      change_map(&map,&previous_map);     
      change_lvl(&player, &time, &shots, &ennemi, &army_shots, &load, &level, &x_max, &y_max, map, tab);
    }

    SDL_UpdateRect(sprite.screen,0,0,0,0);
    /* ~ 12,5 fps */
    SDL_Delay(60);
  }

  /****************************************************************************************************/
  /* CLEAN */

  free_all_sprite(sprite);
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
