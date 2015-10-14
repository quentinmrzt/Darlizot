/******************************************************************/
/* game.h                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Date creation: 20/09/15                                        */
/* Derniere modification: 12/10/15                                */
/******************************************************************/

#ifndef GAME_H
#define GAME_H

void size_tab(int *x_ptr, int *y_ptr) ;




/****************************************************************************************************/
/* INITIALISATION */

SDL_Surface* load(SDL_Surface *surface, char name[], SDL_Surface *screen);
s_surface load_sprite(s_surface sprite);
s_information ini_player(s_information player);

/****************************************************************************************************/
/* KEYBOARD AND MOUSE */

void draw(int tab[][800/50], s_surface sprite);

s_information ini_player(s_information player) ;

int on_the_floor(int tab[][800/50],s_information player);
int case_bottom_floor(int tab[][800/50], s_information player);
int distance_of_floor(int tab[][800/50],s_information player);
//int distance_of_floor(int n, int tab[][n],s_information player);



int distance_wall_left(int tab[][800/50],s_information player);
int distance_wall_right(int tab[][800/50],s_information player);

s_information gravity(s_information player, int tab[][800/50]);

void control(int tab[][800/50], s_information *player_ptr);

int quit(int close);

/****************************************************************************************************/
/* CLEAN */

void free_all_sprite(s_surface sprite);

void recup_map(int map[][800/50]);


#endif
