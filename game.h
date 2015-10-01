/******************************************************************/
/* game.h                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Date creation: 20/09/15                                        */
/* Derniere modification: 20/09/15                                */
/******************************************************************/

#ifndef GAME_H
#define GAME_H


/****************************************************************************************************/
/* INITIALISATION */

SDL_Surface* load(SDL_Surface *surface, char name[], SDL_Surface *screen);
s_surface load_sprite(s_surface sprite);
s_information ini_player(s_information player);

/****************************************************************************************************/
/* KEYBOARD AND MOUSE */

void gravity(SDL_Rect *s1, int *s2);

void control(SDL_Rect *p1, SDL_Rect *r1,int *s1, int *saut, int *f1);

int quit(int close);

/****************************************************************************************************/
/* CLEAN */

void free_all_sprite(s_surface sprite);

#endif
