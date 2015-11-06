/******************************************************************/
/* draw.c                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 21/10/15                                             */
/* Last modification: 05/11/15                                    */
/******************************************************************/

#include "constant.h"
#include "game.h"
#include "draw.h"


/****************************************************************************************************/
/* DRAW */

void draw(int x_max, int y_max, int tab[y_max][x_max], s_surface sprite, s_information player)
{
  int x,y,a,b;
  SDL_Rect pos_sprite,pos_screen;

  pos_sprite.x = 0;
  pos_sprite.y = 0;
  pos_sprite.w = 50;
  pos_sprite.h = 50;

  if (player.movement > 800/2 && player.movement < x_max*50 - 800/2) {
    a = 0;
    for (y=0 ; y<y_max ; y++) {
      b = 0;
      for (x=player.map_x-800/100 ; x<x_max ; x++) {
	pos_screen.x = b*50-player.movement%50;
	pos_screen.y = a*50;

	draw_element(x_max,y_max,tab,x,y,sprite,pos_sprite,pos_screen);

	b++;
      }
      a++;
    }
  } else if (player.movement <= 800/2) {
    for (y=0 ; y<y_max ; y++) {
      for (x=0;x<x_max;x++) {
	pos_screen.x = x*50;
	pos_screen.y = y*50;

	draw_element(x_max,y_max,tab,x,y,sprite,pos_sprite,pos_screen);
      }
    }
  } else {
    a = 0;
    for (y=0 ; y<y_max ; y++) {
      b = 0;
      for (x=x_max-800/50 ; x<x_max ; x++) {
	pos_screen.x = b*50;
	pos_screen.y = a*50;

	draw_element(x_max,y_max,tab,x,y,sprite,pos_sprite,pos_screen);

	b++;
      }
      a++;
    }
  }
}

void draw_element(int x_max, int y_max, int tab[y_max][x_max], int x, int y, s_surface sprite, SDL_Rect pos_sprite, SDL_Rect pos_screen) 
{
  if (tab[y][x] == 0) {
    SDL_BlitSurface(sprite.background, &pos_sprite, sprite.screen, &pos_screen);
  }
  
  if (tab[y][x] == -1) {
    SDL_BlitSurface(sprite.block, &pos_sprite, sprite.screen, &pos_screen);
  } 

  if (tab[y][x] == 1) {
    SDL_BlitSurface(sprite.platform, &pos_sprite, sprite.screen, &pos_screen);
  } 
}

void draw_ennemis(list_ptr ennemi_ptr, s_surface sprite, s_information player)
{
  list_ptr ennemi_list = ennemi_ptr;
  SDL_Rect position;

  while (ennemi_list != NULL) {
    position.y = ennemi_list->info.position.y;
    position.x = ennemi_list->info.movement-20-(player.movement-13-player.position.x);

    SDL_BlitSurface(sprite.ennemi,&ennemi_list->info.rcSrc,sprite.screen,&position);
    ennemi_list = ennemi_list->next;
  }
}


void draw_shooting(s_information player, list_ptr shots, s_surface sprite)
{
  list_ptr shots_copy=shots;
  while (shots_copy!=NULL)
    {
      if (shots_copy->info.state==0){
	shots_copy->info.position.x+=24;
	shots_copy->info.movement+=24;
      
      } else{ 
	shots_copy->info.position.x-=24;
	shots_copy->info.movement-=24;
      }
      SDL_BlitSurface(sprite.bullet,&shots_copy->info.rcSrc,sprite.screen,&shots_copy->info.position);
      shots_copy=shots_copy->next;
    }
}

void draw_tab(int x_max, int y_max, int tab[y_max][x_max]) 
{
  int x,y;

  for (y=0;y<y_max;y++) {
    for (x=0;x<x_max;x++) {
      if (tab[y][x] >= 0) {
	printf(" %d ",tab[y][x]);
      } else {
	printf("%d ",tab[y][x]);
      }
    }
    printf("\n");
  }
}

/****************************************************************************************************/
/* ANIM */

s_information anim_right(int x_max, int y_max, int tab[y_max][x_max], s_information player) 
{
  Uint8 *keystate = SDL_GetKeyState(NULL);

  if (keystate[SDLK_RIGHT] && !keystate[SDLK_LEFT]) {
    /* AU SOL */
    if (distance_of_floor(x_max,y_max,tab,player) == 0) {
      player.rcSrc.y=0;
      if (player.rcSrc.x==0 || player.rcSrc.x>=10*75) {
	player.rcSrc.x=75;
      } else {
	player.rcSrc.x+=75;
      }
    } else {	
      player.rcSrc.y=0;
      player.rcSrc.x=3*75;
    }
  } else {
    /* PAS DE TOUCHE: STATIC */
    if (player.state==0) {
      player.rcSrc.x=0;
      player.rcSrc.y=0;
    }
  }

  return player;
}

s_information anim_left(int x_max, int y_max, int tab[y_max][x_max], s_information player) 
{
  Uint8 *keystate = SDL_GetKeyState(NULL);

  if (keystate[SDLK_LEFT] && !keystate[SDLK_RIGHT]){
    /* AU SOL */
    if (distance_of_floor(x_max,y_max,tab,player) == 0) {
      /* direction droite ou au bout des sprites */
      player.rcSrc.y=0;
      if (player.rcSrc.x<12*75 || player.rcSrc.x>=21*75) {
	player.rcSrc.x=12*75;
      } else {
	player.rcSrc.x+=75;
      }
    } else {
      player.rcSrc.y=0;
      player.rcSrc.x=14*75;
    }
  } else {
    /* PAS DE TOUCHE: STATIC */
    if (player.state==1) {
      player.rcSrc.x=11*75;
      player.rcSrc.y=0;
    }
  }

  return player;
}

s_information anim_jump(int x_max, int y_max, int tab[y_max][x_max], s_information player)
{
  Uint8 *keystate= SDL_GetKeyState(NULL);
  /* JUMP */ 
  /* si EN SAUT  */
  if (player.jump > 0) {
    //deuxieme ligne de sprite 
    
    if (!keystate[SDLK_SPACE]){
      player.rcSrc.y=75;
      if (player.state==0) {
	player.rcSrc.x=75;
      } else {
	player.rcSrc.x=3*75;
      }
    }
  }
  /* si PAS DE SAUT mais PAS AU SOL */
  if (player.jump == 0 && distance_of_floor(x_max,y_max,tab,player) != 0) {
    if (!keystate[SDLK_SPACE]){
      player.rcSrc.y = 75;
      if (player.state == 0) {
	player.rcSrc.x = 75*2;
      } else {
	player.rcSrc.x = 75*5;
      }   
    }
  }
  return player;
}

s_information anim_shoot(int x_max, int y_max, int tab[y_max][x_max], s_information player)
{
  Uint8 *keystate = SDL_GetKeyState(NULL);
  
  if (keystate[SDLK_SPACE]) {
    player.rcSrc.y=75*2;
    if (distance_of_floor(x_max,y_max,tab,player) !=0) {
      if (player.state==0) {
	player.rcSrc.x=22*75;
      } else {
	player.rcSrc.x=23*75;
      }
    }
  }

  return player;
}
