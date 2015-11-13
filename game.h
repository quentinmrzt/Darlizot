/******************************************************************/
/* game.h                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 13/11/15                                    */
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


/****************************************************************************************************/
/* ENEMIES */

list_ptr respawn(list_ptr ennemi,int level, s_information player,int x_max, int y_max,int tab[y_max][x_max]);
list_ptr ennemies_moves(list_ptr ennemi, s_information player);
void ennemis_jump(int x_max,int y_max,int tab[y_max][x_max],list_ptr ennemi,s_information player);
s_information jump(int x_max,int y_max,int tab[y_max][x_max],s_information ennemi,s_information player);

/****************************************************************************************************/
/* TAB */

void size_tab(int *x_ptr, int *y_ptr) ;
void recup_map(int x_max, int y_max, int tab[y_max][x_max]);

/****************************************************************************************************/
/* CLEAN */

void free_all_sprite(s_surface sprite);

#endif
