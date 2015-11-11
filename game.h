/******************************************************************/
/* game.h                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 15/10/15                                    */
/******************************************************************/

#ifndef GAME_H
#define GAME_H


/****************************************************************************************************/
/* INITIALIZE */

SDL_Surface* load(SDL_Surface *surface, char name[], SDL_Surface *screen);
s_surface load_sprite(s_surface sprite);
s_information ini_player(s_information player);
list_ptr ennemi_spawn(s_information player,list_ptr ennemi,int nb_ennemi,int x_max, int y_max,int tab[y_max][x_max]);




/****************************************************************************************************/
/* LIST */

list_ptr list_cons(list_ptr list, s_information information);
list_ptr list_element_delete(list_ptr list);
int list_size(list_ptr list);
/****************************************************************************************************/
/* PHYSICS */

int distance_wall_left(int x_max, int y_max, int tab[y_max][x_max], s_information player);
int distance_wall_right(int x_max, int y_max, int tab[y_max][x_max], s_information player);
int distance_of_floor(int x_max, int y_max, int tab[y_max][x_max], s_information player);
s_information gravity(int x_max, int y_max, int tab[y_max][x_max], s_information player);
void ennemi_gravity(int x_max, int y_max, int tab[y_max][x_max], list_ptr ennemi,s_surface sprite);
int collision_AABB (s_information obj1, s_information obj2);
void collision_bullet_ennemi(list_ptr * shots, list_ptr * ennemi);
/****************************************************************************************************/
/* TAB */

void size_tab(int *x_ptr, int *y_ptr) ;
void recup_map(int x_max, int y_max, int tab[y_max][x_max]);

/****************************************************************************************************/
/* CLEAN */

void free_all_sprite(s_surface sprite);

int boundingbox(int x_max, int y_max, int tab[y_max][x_max], s_information player);
void foot_pos(int x_max, int y_max, int tab[y_max][x_max], s_information player, SDL_Surface *surface, int *pos_min, int *pos_max);
Uint32 get_pixel(SDL_Surface *surface, int x, int y);


#endif
