/******************************************************************/
/* constant.h                                                     */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 15/11/15                                    */
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
  int life;
  SDL_Rect position;
  SDL_Rect rcSrc;
  int state;
  int jump;
  int movement;
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
};
typedef struct s_surface s_surface;

struct s_time
{
  int current; 
  int previous_time;
  int previous_time_ennemi; 
  int level;
  int chrono;
  int time_max;
};
typedef struct s_time s_time;

struct s_tab
{
  int id;
  int x_max;
  int y_max;
  int tab[10][100];
};
typedef struct s_tab s_tab;

#endif 
