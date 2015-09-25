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


/* constant */


/* structure */
struct s_information
{
  /* largeur et hauteur du sprite */
  int width;
  int height;
  /* positon dans la map */
  int pos_mapX;
  int pos_mapY;
  /* position dans l'ecran  */
  int pos_screenX;
  int pos_screenY;
  /* le nombre du sprite */
  int rotation;
  /* vitesse */
  int speed;
  /* vie */
  int life;
};
typedef struct s_information s_information;

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
};
typedef struct s_surface s_surface;

#endif 
