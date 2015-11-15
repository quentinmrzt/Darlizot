/******************************************************************/
/* physic.h                                                       */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 15/11/15                                    */
/******************************************************************/

#ifndef PHYSIC_H
#define PHYSIC_H


int distance_wall_left(int x_max, int y_max, int tab[y_max][x_max], s_information player);
int distance_wall_right(int x_max, int y_max, int tab[y_max][x_max], s_information player);
int distance_of_floor(int x_max, int y_max, int tab[y_max][x_max], s_information player);
int distance_of_ceiling(int x_max, int y_max, int tab[y_max][x_max], s_information player);
s_information gravity(int x_max, int y_max, int tab[y_max][x_max], s_information player);
void ennemi_gravity(int x_max, int y_max, int tab[y_max][x_max], list_ptr ennemi,s_surface sprite);
int collision_AABB (s_information obj1, s_information obj2);
void collision_bullet_ennemi(list_ptr * shots, list_ptr * ennemi);
void collision_bullet_player(list_ptr * army_shots, s_information * player,s_time * time_p);
#endif
