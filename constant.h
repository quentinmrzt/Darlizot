/******************************************************************/
/* constant.h                                                     */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Date creation: 20/09/15                                        */
/* Derniere modification: 20/09/15                                */
/******************************************************************/

#ifndef CONSTANT_H
#define CONSTANT_H

#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#define NB_GRID        19
#define NB_TAB         NB_GRID+1
#define TAB            40

#define SCREEN_WIDTH   NB_GRID*32
#define SCREEN_HEIGHT  NB_GRID*32
#define SPRITE_SIZE    32
#define SPEED          250
#define FPS            30
#define GRID           0
#define POSITION       0
#define END            147


/* constant */


/* structure */
struct s_information
{
  /* largeur et hauteur du sprite */
  int width;
  int height;
  /* le nombre du sprite */
  int rotation;
  /* vitesse */
  int speed;
  /* vie */
  int life;

  SDL_Rect position;
  SDL_Rect rcSrc;
  int state;
  int jump;
};
typedef struct s_information s_information;

typedef struct list_s_information
{
  s_information val;
  struct list_s_information* next;
}* list_info;

struct s_surface
{
  SDL_Surface *screen;
  SDL_Surface *background;
  SDL_Surface *grass;
  SDL_Surface *player;
  SDL_Surface *grid;
  SDL_Surface *rock;
  SDL_Surface *black;
  SDL_Surface *barrier;
  SDL_Surface *plateform;
  SDL_Surface *block;
};
typedef struct s_surface s_surface;

#endif 
