/******************************************************************/
/* constant.h                                                     */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 17/11/15                                    */
/******************************************************************/

#ifndef CONSTANT_H
#define CONSTANT_H

#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

struct s_information
{
  int id;
  int limit;
  int life;
  SDL_Rect position;
  SDL_Rect rcSrc;
  int state;
  int jump;
  int movement;
  int dying;
};
typedef struct s_information s_information;

struct s_node
{
  struct s_node * next;
  s_information info;
};
typedef struct s_node * list_ptr;

struct s_surface
{
  SDL_Surface *screen;
  SDL_Surface *background;
  SDL_Surface *player;
  SDL_Surface *block;  
  SDL_Surface *bullet;
  SDL_Surface *ennemi;
  SDL_Surface *platform;
  SDL_Surface *ammo_set;
  SDL_Surface *ammo;
  SDL_Surface *health;
  SDL_Surface *outline;
  SDL_Surface *locked;
  SDL_Surface *download;
  SDL_Surface *unlocked;
  SDL_Surface *black;
};
typedef struct s_surface s_surface;

struct s_time
{
  int current; 
  int previous_time;
  int previous_time_ennemi;
  int previous_time_ennemi_hit;
  int previous_time_hit;
  int level;
  int chrono;
  int time_max;
  int menu;
  int birth;
  int dead;
};
typedef struct s_time s_time;

#endif 
