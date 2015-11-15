/******************************************************************/
/* physic.c                                                       */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 15/11/15                                    */
/******************************************************************/

#include "constant.h"
#include "draw.h"
#include "game.h"
#include "control.h"
#include "physic.h"


int distance_wall_left(int x_max, int y_max, int tab[y_max][x_max], s_information player) 
{
  int i, y;

  if (player.id == 0) {
    y = player.position.y+23;
  } else {
    y = player.position.y+35;
  }

  for (i=(player.movement/50) ; i>=(player.movement-2*50)/50; i--) {
    if (tab[(player.position.y+74)/50][i] == -1 || tab[y/50][i] == -1) {
      // -50 pour coin de gauche
      return (player.movement)-(i*50)-50;
    }   
  }

  return player.movement;
}

int distance_wall_right(int x_max, int y_max, int tab[y_max][x_max], s_information player) 
{
  int i,y,x;

  if (player.id == 0) {
    y = player.position.y+23;
  } else {
    y = player.position.y+35;
  }
  x = player.movement+75-13*2;

  for (i=(player.movement+(75-13*2))/50 ; i<(x+2*50)/50 ; i++) {
    if (tab[(player.position.y+74)/50][i] == -1 || tab[y/50][i] == -1) {
      // -13 car pos du pied droit *2 pour pos pied gauche
      return (i*50)-x;
    }
  }

  return player.movement+75-13*2;
}

int distance_of_floor(int x_max, int y_max, int tab[y_max][x_max], s_information player)
{
  int i;

  for (i=(player.position.y+75)/50 ; i<(player.position.y+75*5)/50 ; i++) {
    /* [1/3;2/3] */
    if (tab[i][(player.movement-13+75/3)/50] == -1 || tab[i][(player.movement-13+75/3*2)/50] == -1) {
      return (i*50)-(player.position.y+75);
    }

    if (tab[i][(player.movement-13+75/3)/50] == 1 || tab[i][(player.movement-13+75/3*2)/50] == 1) {
      if ((i*50)-(player.position.y+75) < 0) {
	//A REGARDER
	return player.movement;
      } else {
	return (i*50)-(player.position.y+75);
      }
    }
  }

  return player.position.y+75;
}

int distance_of_ceiling(int x_max, int y_max, int tab[y_max][x_max], s_information player)
{
  int i, head;

  if (player.id == 0) {
    head = player.position.y+23;
  } else {
    head = player.position.y+35;
  }

  for (i=head/50 ; i>=head/50-3 ; i--) {
    if (tab[i][(player.movement-13+75/3)/50] == -1 || tab[i][(player.movement-13+75/3*2)/50] == -1) {
      //-50 car 0 à i donc manque 1
      return head-(i*50)-50;
    }
  }

  return head;
}

s_information gravity(int x_max, int y_max, int tab[y_max][x_max], s_information player)
{
  int distance = distance_of_floor(x_max,y_max,tab,player);

  if (player.jump == 0) {
    if (distance >= 15) {
      player.position.y += 15;
    } else {
      player.position.y += distance; 
    }
  }

  return player;
}

void ennemi_gravity(int x_max,int y_max,int tab[y_max][x_max],list_ptr ennemi,s_surface sprite)
{
  list_ptr ennemi_list=ennemi;
  
  while(ennemi_list != NULL) {
    ennemi_list->info = gravity(x_max,y_max,tab,ennemi_list->info);
    ennemi_list = ennemi_list->next;
  }
}

int collision_AABB(s_information obj1, s_information obj2)
{
  //printf("1: %d  2: %d\n",obj1.movement, obj2.movement);
  if(obj1.movement  <= obj2.movement +5
     || obj1.movement >= obj2.movement +30
     || obj1.position.y <= obj2.position.y +35
     || obj1.position.y >= obj2.position.y +75){
    return 0;
  }
  return 1;
}

void collision_bullet_ennemi(list_ptr *shots, list_ptr *ennemi)
{
  list_ptr copy_shots = NULL;
  list_ptr copy_ennemi = NULL;

  list_ptr copy_copy_shots = NULL;
  list_ptr copy_copy_ennemi = NULL;

  copy_shots=*shots;
  copy_ennemi=*ennemi;

  copy_copy_shots = copy_shots;
  while (copy_copy_shots != NULL) {
    copy_copy_ennemi = copy_ennemi;
    while (copy_copy_ennemi != NULL) {
      if (collision_AABB(copy_copy_shots->info,copy_copy_ennemi->info)){
	if (copy_copy_ennemi->info.life != 0 && copy_copy_shots->info.life != 0) {
	  copy_copy_ennemi->info.life = 0;
	  copy_copy_shots->info.life = 0;
	}
      }
      copy_copy_ennemi = copy_copy_ennemi->next;
    }
    if (copy_copy_shots->info.position.x>800 || copy_copy_shots->info.position.x<=0){
      copy_copy_shots->info.life=0;
    }
    copy_copy_shots = copy_copy_shots->next;   
  }

  copy_ennemi = list_element_delete(copy_ennemi);
  copy_shots = list_element_delete(copy_shots);
  
  *ennemi=copy_ennemi;
  *shots=copy_shots;

  free(copy_copy_shots);
  free(copy_copy_ennemi);
}

void collision_bullet_player(list_ptr * army_shots, s_information * player, s_time * time_p)
{
  list_ptr copy_shots= *army_shots;
  s_information copy_player=*player;
  s_time time=*time_p;
  if(time_p->previous_time_hit-time_p->current>2500){
    time.previous_time_hit=time.current;
    while (copy_shots != NULL){
      if(collision_AABB(copy_shots->info,copy_player)){
	copy_player.life=copy_player.life-10;
	copy_shots->info.life=0;
      }
      copy_shots=copy_shots->next;
    }
  }
  *player=copy_player;
  *army_shots=copy_shots;
  *time_p=time;
}
