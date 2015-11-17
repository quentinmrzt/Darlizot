/******************************************************************/
/* draw.h                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 21/10/15                                             */
/* Last modification: 17/11/15                                    */
/******************************************************************/

#ifndef DRAW_H
#define DRAW_H

/****************************************************************************************************/
/* DRAW */

void draw(int x_max, int y_max, int tab[y_max][x_max], s_surface sprite, s_information player);
void draw_player(s_information player,s_surface sprite,s_time* time_p,int map);
void draw_element(int x_max, int y_max, int tab[y_max][x_max], int x, int y, s_surface sprite, SDL_Rect pos_sprite, SDL_Rect pos_screen);
void draw_ennemis(list_ptr ennemi_ptr,s_surface sprite, s_information player);
void draw_tab(int x_max, int y_max, int tab[y_max][x_max]);
list_ptr anim_ennemis(list_ptr ennemis,s_information player,int x_max,int y_max,int tab[y_max][x_max]);
void draw_shooting(s_information player, list_ptr shots, s_surface sprite);
void draw_ennemis_shooting(list_ptr army_shots,s_surface sprite,s_information player);
void draw_ammo(s_surface sprite,int ammo);
void draw_health(s_information player,s_surface sprite);
void draw_ammo(s_surface sprite,int ammo);

void draw_chrono(SDL_Surface *screen, TTF_Font *font, s_time chrono, int map);
void draw_level(SDL_Surface *screen, TTF_Font *font, int level, int map);
void draw_menu(s_surface sprite, s_time time);
void draw_outline(s_surface sprite, int choice);

/****************************************************************************************************/
/* ANIM */

s_information anim_right(int x_max, int y_max, int tab[y_max][x_max], s_information player, int automatic);
s_information anim_left(int x_max, int y_max, int tab[y_max][x_max], s_information player, int automatic);
s_information anim_jump(int x_max, int y_max, int tab[y_max][x_max], s_information player);
s_information anim_shoot(int x_max, int y_max, int tab[y_max][x_max], s_information player);

#endif
