/******************************************************************/
/* physic.c                                                       */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 18/11/15                                    */
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

  for (i=(player.movement/50) ; i>=(player.movement-8*50)/50; i--) {
    if (tab[(player.position.y+74)/50][i] == -1 || tab[y/50][i] == -1) {
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

  for (i=(player.movement+(75-13*2))/50 ; i<(x+8*50)/50 ; i++) {
    if (tab[(player.position.y+74)/50][i] == -1 || tab[y/50][i] == -1) {
      return (i*50)-x;
    }
  }

  return player.movement+75-13*2;
}

int distance_of_floor(int x_max, int y_max, int tab[y_max][x_max], s_information player)
{
  int i;

  for (i=(player.position.y+75)/50 ; i<(player.position.y+75*5)/50 ; i++) {
    if (tab[i][(player.movement-13+75/3)/50] == -1 || tab[i][(player.movement-13+75/3*2)/50] == -1) {
      return (i*50)-(player.position.y+75);
    }
    if (tab[i][(player.movement-13+75/3)/50] == 1 || tab[i][(player.movement-13+75/3*2)/50] == 1) {
      if ((i*50)-(player.position.y+75) < 0) {
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
  if(obj1.movement  <= obj2.movement +5
     || obj1.movement >= obj2.movement +30
     || obj1.position.y <= obj2.position.y +35
     || obj1.position.y >= obj2.position.y +75){
    return 0;
  }
  return 1;
}

void collision_bullet_ennemi(list_ptr *shots, list_ptr *ennemi,int *score,int level)
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
	if (copy_copy_ennemi->info.life > 0 && copy_copy_shots->info.life != -1) {
	  *score=scoring(*score,level);
	  copy_copy_ennemi->info.life = 0;
	  copy_copy_shots->info.life = -1;
	}
      }
      copy_copy_ennemi = copy_copy_ennemi->next;
    }
    if (copy_copy_shots->info.position.x>800 || copy_copy_shots->info.position.x<=0){
      copy_copy_shots->info.life=-1;
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

list_ptr collision_bullet_player(list_ptr army_shots, s_information * player, s_time * time_p)
{
  list_ptr copy_shots= army_shots;
  int touch = 0;

  if(time_p->current-time_p->previous_time_hit>2500){  
    while (copy_shots != NULL && !touch){
      if(collision_AABB(copy_shots->info,*player)){
	player->life=(player->life)-1;
	if (player->life < 0) {
	  player->life = 0;
	}
	copy_shots->info.life=-1;
	time_p->previous_time_hit=time_p->current;
	touch = 1;
      }
      copy_shots=copy_shots->next;
    }
  }
  army_shots=list_element_delete(army_shots);
  return army_shots;
}


list_ptr wall_bang(list_ptr shots,int x_max,int y_max,int tab[y_max][x_max])
{
  list_ptr shots_list=shots;
  while(shots_list!=NULL){
    if(tab[shots_list->info.position.y/50][(shots_list->info.movement+25)/50]==-1 || tab[shots_list->info.position.y/50][(shots_list->info.movement-25)/50]==-1){
	shots_list->info.life=-1;
      }
    shots_list=shots_list->next;
  }
  return shots;
}
