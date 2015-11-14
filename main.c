/******************************************************************/
/* main.c                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 06/11/15                                    */
/******************************************************************/

#include "constant.h"
#include "game.h"
#include "draw.h"
#include "control.h"
#include "time.h"
#include "physic.h"

int main(int argc, char* argv[])
{
  int close, x_max, y_max, level,ammo=60,energy=10;
  s_information player;
  s_surface sprite;
  SDL_Rect position;
  list_ptr shots = NULL;
  list_ptr ennemi = NULL;
  int current_time,previous_time=0,previous_time_ennemi=0,load=1,nb_ennemi_spawn;
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
  /* table */
  size_tab(&x_max,&y_max);
  int tab[y_max][x_max];
  recup_map(x_max,y_max,tab);

  //draw_tab(x_max,y_max,tab);
  close = 0;
  level= 2;
  nb_ennemi_spawn=0;
  while (!close) {
    current_time=SDL_GetTicks();
    /****************************************************************************************************/
    /* KEYBOARD AND MOUSE */
    ennemi = respawn(ennemi,&level,player,&previous_time_ennemi,&nb_ennemi_spawn,&load,x_max,y_max,tab);
    close = quit(close);
    player = control(x_max,y_max,tab,player);
    shots = shooting(player,shots,&ammo,energy,&previous_time,current_time);
    /****************************************************************************************************/
    /* GAME */
    shots=wall_bang(shots,x_max,y_max,tab);
    player = gravity(x_max,y_max,tab,player);
    ennemi_gravity(x_max,y_max,tab,ennemi,sprite);
    ennemis_jump(x_max,y_max,tab,ennemi,player);
    collision_bullet_ennemi(&shots,&ennemi);
    /****************************************************************************************************/
    /* DRAW */
    draw(x_max,y_max,tab,sprite,player);
    draw_ennemis(ennemi,sprite,player);
    draw_shooting(player,shots,sprite);
    draw_ammo(sprite,ammo);
    draw_health(player,sprite);
    //draw_pos(sprite.screen,font,player); 
    ennemies_moves(ennemi,player);
    anim_ennemis(ennemi,player);
    // tampon car BlitSurface remet a 0 si nega
    position = player.position;
    SDL_BlitSurface(sprite.player, &player.rcSrc, sprite.screen, &position);
    // fermeture de porte
    if (current_time >= 3500 && current_time <= 4000) {
      tab[1][0] = -1;
      tab[5][0] = -1;

      tab[1][x_max-1] = -1;
      tab[5][x_max-1] = -1;
    }
    if (current_time >= 3000 && current_time <= 3500) {
      tab[2][0] = -1;
      tab[6][0] = -1;

      tab[2][x_max-1] = -1;
      tab[6][x_max-1] = -1;
    }

    // ouverture de la porte
    if (current_time > 6000) {
      tab[1][0] = 0;
      tab[5][0] = 0;

      tab[1][x_max-1] = 0;
      tab[5][x_max-1] = 0;
    }
    if (current_time > 6500) {
      tab[2][0] = 0;
      tab[6][0] = 0;

      tab[2][x_max-1] = 0;
      tab[6][x_max-1] = 0;
    }

    /****************************************************************************************************/
    /* OTHER */
    if (player.position.y > 400) {
      player = ini_player(player);
    }
    //printf("Gauche :%d\n",distance_wall_left(x_max,y_max,tab,player));
    a_and_z(x_max,y_max,tab,player);
    ennemi=killing(ennemi);
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
