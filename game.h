/******************************************************************/
/* game.h                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 15/10/15                                    */
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

int quit(int close);
s_information control(int x_max, int y_max, int tab[y_max][x_max], s_information player);
s_information move_right(int x_max, int y_max, int tab[y_max][x_max], s_information player);
s_information move_left(int x_max, int y_max, int tab[y_max][x_max], s_information player);
s_information move_jump(int x_max, int y_max, int tab[y_max][x_max], s_information player);

/****************************************************************************************************/
/* PHYSICS */

int distance_wall_left(int x_max, int y_max, int tab[y_max][x_max], s_information player);
int distance_wall_right(int x_max, int y_max, int tab[y_max][x_max], s_information player);
int distance_of_floor(int x_max, int y_max, int tab[y_max][x_max], s_information player);
s_information gravity(int x_max, int y_max, int tab[y_max][x_max], s_information player);

/****************************************************************************************************/
/* DRAW */

void draw(int x_max, int y_max, int tab[y_max][x_max], s_surface sprite);
s_information anim_right(int x_max, int y_max, int tab[y_max][x_max], s_information player);
s_information anim_left(int x_max, int y_max, int tab[y_max][x_max], s_information player);
s_information anim_jump(int x_max, int y_max, int tab[y_max][x_max], s_information player);
s_information anim_shoot(int x_max, int y_max, int tab[y_max][x_max], s_information player);

/****************************************************************************************************/
/* TAB */

void size_tab(int *x_ptr, int *y_ptr) ;
void recup_map(int x_max, int y_max, int tab[y_max][x_max]);

/****************************************************************************************************/
/* CLEAN */

void free_all_sprite(s_surface sprite);


#endif
