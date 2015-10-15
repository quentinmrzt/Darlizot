/******************************************************************/
/* constant.h                                                     */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 15/10/15                                    */
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
  int speed;
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
  SDL_Surface *player;
  SDL_Surface *block;
};
typedef struct s_surface s_surface;

#endif 
