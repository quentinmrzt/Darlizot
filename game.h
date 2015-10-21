/******************************************************************/
/* game.h                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 15/10/15                                    */
/******************************************************************/

#ifndef GAME_H
#define GAME_H

/****************************************************************************************************/
/* LIST */
list_ptr list_cons(list_ptr list,int lf,SDL_Rect pos,SDL_Rect Src,int st) ;

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
s_information move_map(s_information player, int movement);
list_ptr shooting(s_information player,list_ptr shots);
list_ptr ennemi_spawn(s_information player,list_ptr ennemi,int nb_ennemi,int x_max, int y_max,int tab[y_max][x_max]);
int update_ennemi(int nb_ennemi,list_ptr ennemi);
/****************************************************************************************************/
/* PHYSICS */

int distance_wall_left(int x_max, int y_max, int tab[y_max][x_max], s_information player);
int distance_wall_right(int x_max, int y_max, int tab[y_max][x_max], s_information player);
int distance_of_floor(int x_max, int y_max, int tab[y_max][x_max], s_information player);
s_information gravity(int x_max, int y_max, int tab[y_max][x_max], s_information player);
void ennemi_gravity(int x_max,int y_max,int tab[y_max][x_max],list_ptr ennemi, s_surface sprite);

/****************************************************************************************************/
/* DRAW */

void draw(int x_max, int y_max, int tab[y_max][x_max], s_surface sprite);
void draw_shooting(s_information player, list_ptr shots, s_surface sprite);
s_information anim_right(int x_max, int y_max, int tab[y_max][x_max], s_information player);
s_information anim_left(int x_max, int y_max, int tab[y_max][x_max], s_information player);
s_information anim_jump(int x_max, int y_max, int tab[y_max][x_max], s_information player);
s_information anim_shoot(int x_max, int y_max, int tab[y_max][x_max], s_information player);
void draw_ennemis(list_ptr ennemi_ptr,s_surface sprite);

/****************************************************************************************************/
/* TAB */

void size_tab(int *x_ptr, int *y_ptr) ;
void recup_map(int x_max, int y_max, int tab[y_max][x_max]);

/****************************************************************************************************/
/* CLEAN */

void free_all_sprite(s_surface sprite);


#endif
