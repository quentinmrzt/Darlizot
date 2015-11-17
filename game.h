/******************************************************************/
/* game.h                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 16/11/15                                    */
/******************************************************************/

#ifndef GAME_H
#define GAME_H


/****************************************************************************************************/
/* INITIALIZE */

SDL_Surface* load(SDL_Surface *surface, char name[], SDL_Surface *screen);
s_surface load_sprite(s_surface sprite);
s_information ini_player(s_information player);
s_time ini_time(s_time time);
list_ptr ennemi_spawn(s_information player,list_ptr ennemi,int nb_ennemi,int x_max, int y_max,int tab[y_max][x_max]);
s_time duration_chrono(s_information player, s_time time , int x_max);
void change_map(int *map_ptr, int *previous_map_ptr);
void change_lvl(s_information *player_ptr, s_time *time_ptr, list_ptr *shots_ptr, list_ptr *ennemi_ptr, list_ptr *army_shots_ptr, int *load_ptr, int *level,int *x_max, int *y_max, int map, int tab[*y_max][*x_max]);

/****************************************************************************************************/
/* LIST */

list_ptr list_cons(list_ptr list, s_information information);
list_ptr list_element_delete(list_ptr list);
int list_size(list_ptr list);

/****************************************************************************************************/
/* PHYSICS */
list_ptr wall_bang(list_ptr shots,int x_max,int y_max,int tab[y_max][x_max]);

/****************************************************************************************************/
/* ENEMIES */
void ennemies_moves(list_ptr ennemi, s_information player,int x_max,int y_max,int tab[y_max][x_max]);
int nb_ennemi_update(int level);
list_ptr respawn(list_ptr ennemi,int* level, s_information player,s_time *time_p,int *nb_ennemi_spawn,int *load,int x_max, int y_max,int tab[y_max][x_max], int map);
void ennemis_jump(int x_max,int y_max,int tab[y_max][x_max],list_ptr ennemi,s_information player);
s_information jump(int x_max,int y_max,int tab[y_max][x_max],s_information ennemi,s_information player);
list_ptr ennemis_shots(list_ptr ennemis,list_ptr army_shots, s_information player,int x_max,int y_max,int tab[y_max][x_max],s_time *time_p);
list_ptr kill_all(list_ptr ennemis);

/****************************************************************************************************/
/* TAB */
void size_tab(int *x_ptr, int *y_ptr, int map);
int nb_map(void);
void recup_map(int x_max, int y_max, int tab[y_max][x_max], int map);
void door_ennemy(int x_max, int y_max, int tab[y_max][x_max], s_information player, int load, s_time time, int map);
void door_player(int x_max, int y_max, int tab[y_max][x_max], s_information player, s_time time);

/****************************************************************************************************/
/* CLEAN */

void free_all_sprite(s_surface sprite);
void free_list(list_ptr list);

#endif
