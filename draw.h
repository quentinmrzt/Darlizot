/******************************************************************/
/* draw.h                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 21/10/15                                             */
/* Last modification: 21/10/15                                    */
/******************************************************************/

#ifndef DRAW_H
#define DRAW_H

/****************************************************************************************************/
/* DRAW */

void draw(int x_max, int y_max, int tab[y_max][x_max], s_surface sprite);
void draw_shooting(s_information player, list_ptr shots, s_surface sprite);
void draw_ennemis(list_ptr ennemi_ptr,s_surface sprite);
void draw_tab(int x_max, int y_max, int tab[y_max][x_max]);

/****************************************************************************************************/
/* ANIM */

s_information anim_right(int x_max, int y_max, int tab[y_max][x_max], s_information player);
s_information anim_left(int x_max, int y_max, int tab[y_max][x_max], s_information player);
s_information anim_jump(int x_max, int y_max, int tab[y_max][x_max], s_information player);
s_information anim_shoot(int x_max, int y_max, int tab[y_max][x_max], s_information player);

#endif
