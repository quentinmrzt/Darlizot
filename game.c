/******************************************************************/
/* game.c                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 13/11/15                                    */
/******************************************************************/

#include "constant.h"
#include "draw.h"
#include "game.h"
#include "control.h"

/****************************************************************************************************/
/* INITIALIZE */

SDL_Surface* load(SDL_Surface *surface, char name[], SDL_Surface *screen) 
{
  SDL_Surface *temp;
  int colorkey = SDL_MapRGB(screen->format, 255, 0, 255);
  temp = SDL_LoadBMP(name);
  surface = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  SDL_SetColorKey(surface, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
  return surface;
}

s_surface load_sprite(s_surface sprite) 
{
  /* load sprite */
  char name[] = "sprite/sprite_01.bmp";
  sprite.player = load(sprite.player, name, sprite.screen);
  name[15] = '2';
  sprite.background = load(sprite.background, name, sprite.screen);
  name[15] = '3';
  sprite.block = load(sprite.block, name, sprite.screen);
  name[15] = '4';
  sprite.bullet = load(sprite.bullet, name, sprite.screen);
  name[15] = '6';
  sprite.ennemi = load(sprite.ennemi, name, sprite.screen);
  name[15] = '7';
  sprite.platform = load(sprite.platform, name, sprite.screen);
  name[15] = '8';
  sprite.ammo = load(sprite.ammo, name, sprite.screen);
  name[15] = '9';
  sprite.health = load(sprite.health, name, sprite.screen);
  return sprite;
}

s_information ini_player(s_information player) 
{
  player.id = 0;
  player.life=5;
  player.rcSrc.x = 0;
  player.rcSrc.y = 0;
  player.rcSrc.w = 75;
  player.life=5;
  player.rcSrc.h = 75;
  player.position.x = 360;
  player.position.y = 0;
  player.jump = 0;
  player.state = 0;
  player.map_x = 0;
  player.movement = player.position.x+13;

  return player;
}

list_ptr ennemi_spawn(s_information player,list_ptr ennemi,int nb_ennemi,int x_max, int y_max,int tab[y_max][x_max])
{
  s_information ennemi_info;
  int i;

  if (nb_ennemi > 0) {
    for (i=0 ; i<nb_ennemi ; i++) {
      ennemi_info = ini_player(ennemi_info);
      ennemi_info.position.x =  (rand()%1200)+100;
      ennemi_info.id = 1;
      ennemi_info.movement = ennemi_info.position.x+20;
      ennemi_info.rcSrc.x = 11*75;
      ennemi = list_cons(ennemi, ennemi_info);
    }
    return ennemi;
  }
  return ennemi;
}

/****************************************************************************************************/
/* LIST */

list_ptr list_cons(list_ptr list, s_information information) 
{
  list_ptr new = NULL;
  /* dynamic allocation */
  new = (list_ptr) malloc(sizeof(struct s_node));

  new->info = information;

  new->next = list;
  return new;
}

list_ptr list_element_delete(list_ptr list)
{ 
  if (list==NULL){
    return NULL;
  }
  list_ptr copy_list=list;
  list_ptr tmp= (list_ptr) malloc(sizeof(struct s_node));
  if (copy_list->info.life==0 || copy_list->info.position.y>450){
    if (copy_list->next==NULL){
      tmp=copy_list;
      free(tmp);
      return NULL;
    }else{
      tmp=copy_list;
      list=copy_list->next;
    }
  }else{
    while (copy_list!=NULL && copy_list->next!=NULL)
      {
	if (copy_list->next->info.life==0 || copy_list->next->info.position.y>450)
	  {	  
	    tmp=copy_list->next;
	    copy_list->next=copy_list->next->next;
	  }
	else 
	  copy_list=copy_list->next;
      }
  }
  free(tmp);
  return list;  
}

int list_size(list_ptr list)
{
  int i=0;
  while (list!=NULL)
    {
      i+=1;
      list=list->next;
      
    }
  return i;
}

/****************************************************************************************************/
/* PHYSICS */

int distance_wall_left(int x_max, int y_max, int tab[y_max][x_max], s_information player) 
{
  int i;
  for (i=(player.movement/50) ; i>(player.movement-2*75)/50; i--) {
    if (tab[(player.position.y+74)/50][i] == -1) {
      // -50 pour coin de gauche
      return (player.movement)-(i*50)-50;
    }   
  }
  return player.movement;
}

int distance_wall_right(int x_max, int y_max, int tab[y_max][x_max], s_information player) 
{
  int i;

  for (i=(player.movement+(75-13*2))/50 ; i<(player.movement+(75-13*2)+2*75)/50 ; i++) {
    if (tab[(player.position.y+74)/50][i] == -1) {
      // -13 car pos du pied droit *2 pour pos pied gauche
      return (i*50)-(player.movement+75-13*2);
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
  int i, j, head; 
  head = player.position.y+31;
  j = head;

  /*while (j > head-(50*3)) {
    if (tab[j/50][(player.movement-13+75/3)/50] == -1 || tab[j/50][(player.movement-13+75/3*2)/50] == -1) {
      printf("%d  %d\n",j, head);
      //return head-(j/50)*50;
      return head-(j-j%50);
    }
    j -= 50;
    }*/

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

/****************************************************************************************************/
/* ENEMIES */

list_ptr respawn(list_ptr ennemi,int level, s_information player,int x_max, int y_max,int tab[y_max][x_max])
{
  list_ptr new_ennemi=NULL;
  if(ennemi==NULL){
    new_ennemi=ennemi_spawn(player,ennemi,level*level,x_max,y_max,tab);
    return new_ennemi;
  }
  return ennemi;
}

list_ptr ennemies_moves(list_ptr ennemi, s_information player)
{
  list_ptr copy_ennemi=ennemi;
  while (copy_ennemi!=NULL){
    if (){
      if (player.movement-100>copy_ennemi->info.movement)
	{
	  copy_ennemi->info.movement+=5;
	}
      if (player.movement+100<copy_ennemi->info.movement){
	copy_ennemi->info.movement-=5;
      }
    }
    copy_ennemi=copy_ennemi->next;
  }
}

void ennemis_jump(int x_max,int y_max,int tab[y_max][x_max],list_ptr ennemi,s_information player)
{
  list_ptr ennemi_list=ennemi;
  
  while (ennemi_list != NULL) {
    ennemi_list->info = jump(x_max,y_max,tab,ennemi_list->info,player);
    ennemi_list = ennemi_list->next;
  }
}

s_information jump(int x_max,int y_max,int tab[y_max][x_max],s_information ennemi,s_information player)
{
  int distance = distance_of_floor(x_max,y_max,tab,ennemi);
  if (ennemi.jump > 2) {
    ennemi.position.y -= 15;
  }

  if (ennemi.jump > 0) {
    ennemi.jump -= 1;
  }

  /* si SAUT et AU SOL */
  if (player.movement>ennemi.movement){
    if (distance_wall_right(x_max,y_max,tab,ennemi)<=10 && distance == 0) { 
      ennemi.jump = 7;
    } 
  } else {
    if (distance_wall_left(x_max,y_max,tab,ennemi)<=10 && distance == 0) { 
      ennemi.jump = 7;
    } 
  }

  return ennemi;
}
/****************************************************************************************************/
/* TAB */

void size_tab(int *x_ptr, int *y_ptr) 
{
  FILE* recuperation;
  int number, end, x;
  *x_ptr = 0;
  *y_ptr = 0;
  recuperation = NULL;
  x = 0;
  number=0;
  /* mode read */
  recuperation = fopen("data/map_1", "r");

  if (recuperation != NULL) {

    // on se met au début 
    rewind(recuperation);

    while (number != 148) {
      fscanf(recuperation, "%d",&number);

      // pour les y 
      if (number == 147) {
	*y_ptr = *y_ptr+1;
      }
      // pour les x
      if (number == 147) {
	if (x > *x_ptr) {
	  *x_ptr = x;
	}
	x = 0;
      } else {
	x++;
      }
    }
    fclose(recuperation);
  }
}

void recup_map(int x_max, int y_max, int tab[y_max][x_max])
{
  FILE* recuperation;
  int x, y,number;
  number = 0;
  recuperation = NULL;
  
  /* mode read */
  recuperation = fopen("data/map_1", "r");

  if (recuperation != NULL) {

    /* on se met au début */
    rewind(recuperation);

    y = 0;
    while (y < y_max && number != 148) {
      x = 0;
      number = 0;

      /* x < taille du tableau ET pas retour */
      while (x < x_max && number != 147) {
	fscanf(recuperation, "%d", &number);
	if (number != 147 && number != 148) {
	  tab[y][x] = number;
	}
	x++;
      }

      if (x < x_max) {
	printf("on rentre\n");
      }

      /* tab < map: on va jusqu'à la fin de la ligne */
      while (number != 147 && number != 148) {
	fscanf(recuperation, "%d", &number);
      }
      y++;
    }
    fclose(recuperation);
  }
}

/****************************************************************************************************/
/* CLEAN */

void free_all_sprite(s_surface sprite) 
{
  SDL_FreeSurface(sprite.screen);
  SDL_FreeSurface(sprite.background);
  SDL_FreeSurface(sprite.player);
  SDL_FreeSurface(sprite.block);
  SDL_FreeSurface(sprite.bullet);
  SDL_FreeSurface(sprite.ennemi);
  SDL_FreeSurface(sprite.ammo);
  SDL_FreeSurface(sprite.platform);
  SDL_FreeSurface(sprite.health);

}
