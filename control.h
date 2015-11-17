/******************************************************************/
/* control.h                                                      */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 21/10/15                                             */
/* Last modification: 17/11/15                                    */
/******************************************************************/

#ifndef CONTROL_H
#define CONTROL_H

/****************************************************************************************************/
/* KEYBOARD AND MOUSE */

int quit(int close);
void control(int x_max,int y_max,int tab[y_max][x_max],int map,s_information *player_ptr,list_ptr *shots_ptr,s_time *time_ptr,int *ammo_ptr,int energy);
s_information control_manual(int x_max, int y_max, int tab[y_max][x_max], s_information player);
s_information control_auto(int x_max, int y_max, int tab[y_max][x_max], s_information player, int x);
s_time control_menu(int x_max,int *choice_ptr, int *action_ptr, s_time time);
list_ptr shooting(s_information player,list_ptr shots, int *ammo,int energy,s_time *time_p);
void a_and_z(int x_max, int y_max, int tab[y_max][x_max], s_information player);
void killing(list_ptr *ennemi);

/****************************************************************************************************/
/* MOVE */

s_information move_right(int x_max, int y_max, int tab[y_max][x_max], s_information player, int automatic);
s_information move_left(int x_max, int y_max, int tab[y_max][x_max], s_information player, int automatic);
s_information move_jump(int x_max, int y_max, int tab[y_max][x_max], s_information player);



#endif
