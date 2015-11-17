/******************************************************************/
/* control.h                                                      */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 21/10/15                                             */
/* Last modification: 15/11/15                                    */
/******************************************************************/

#ifndef CONTROL_H
#define CONTROL_H

/****************************************************************************************************/
/* KEYBOARD AND MOUSE */

int quit(int close);
s_information control(int x_max, int y_max, int tab[y_max][x_max], s_information player);
s_information control_auto(int x_max, int y_max, int tab[y_max][x_max], s_information player, int x);
list_ptr shooting(s_information player,list_ptr shots, int *ammo,int energy,s_time *time_p);
void a_and_z(int x_max, int y_max, int tab[y_max][x_max], s_information player);
list_ptr killing(list_ptr ennemi);

/****************************************************************************************************/
/* MOVE */

s_information move_right(int x_max, int y_max, int tab[y_max][x_max], s_information player, int automatic);
s_information move_left(int x_max, int y_max, int tab[y_max][x_max], s_information player, int automatic);
s_information move_jump(int x_max, int y_max, int tab[y_max][x_max], s_information player);



#endif
