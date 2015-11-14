/******************************************************************/
/* game.c                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 15/11/15                                    */
/******************************************************************/

#include "constant.h"
#include "draw.h"
#include "game.h"
#include "control.h"
#include "physic.h"
#include "time.h"

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
  name[15] = '5';
  sprite.ammo_set = load(sprite.ammo_set, name, sprite.screen);
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
  player.position.x = -75;
  player.position.y = 400-50-75;
  player.jump = 0;
  player.state = 0;
  player.movement = player.position.x+13;

  return player;
}

s_time ini_time(s_time time)
{
  time.current = 0; 
  time.previous_time = 0;
  time.previous_time_ennemi = 0; 
  time.level = 0;
  time.time_max = 20000;
  time.chrono = time.time_max;

  return time;
}

s_time duration_chrono(s_information player,s_time time ,int x_max) 
{  
  // DEBUT DU CHRONO
  if (player.movement/50 == x_max-2 && time.level == 0) {
    time.level = time.current;
  }
  // DEFILER DU CHRONO
  if (time.level != 0 && time.chrono >= 0) {
    time.chrono = time.time_max - time.current + time.level;
  }

  return time;
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
/* ENEMIES */

list_ptr ennemi_spawn(s_information player,list_ptr ennemi,int nb_ennemi,int x_max, int y_max,int tab[y_max][x_max])
{
  s_information ennemi_info;
  int i;
  if (nb_ennemi > 0) {
    for (i=0 ; i<nb_ennemi ; i++) {
      ennemi_info = ini_player(ennemi_info);
      if(rand()%2==0){
	ennemi_info.position.x=-40;
	ennemi_info.rcSrc.x = 0*75;
      }else{
	ennemi_info.position.x = ((x_max)*50);
	ennemi_info.rcSrc.x = 11*75;
      }
      ennemi_info.position.y = 100;
      ennemi_info.id = 1;
      ennemi_info.movement = ennemi_info.position.x+20;
      ennemi = list_cons(ennemi, ennemi_info);
    }
    return ennemi;
  }
  return ennemi;
}


list_ptr respawn(list_ptr ennemi,int *level, s_information player,s_time *time_p,int *nb_ennemi_spawn,int *load,int x_max, int y_max,int tab[y_max][x_max])
{
  list_ptr new_ennemi=NULL;
  s_time time = *time_p;
  int nb_ennemi=nb_ennemi_update(*level);

  if(*load==1){
    if(time.current - time.previous_time_ennemi>600){
      if(*nb_ennemi_spawn!=nb_ennemi){
	new_ennemi=ennemi_spawn(player,ennemi,1,x_max,y_max,tab);
	*nb_ennemi_spawn=*nb_ennemi_spawn+1;
	time.previous_time_ennemi=time.current;
	*time_p = time;
	return new_ennemi;
      }else{
	*load=0;
	*level=*level+1;
	*nb_ennemi_spawn=0;
      }
    }
  }else{
    if(ennemi==NULL /*&& time.chrono > 0*/) {
      *load=1;
    }
  }
  *time_p = time;
  return ennemi;
}
int nb_ennemi_update(int level)
{
  return level+3*level;
}

void ennemies_moves(list_ptr ennemi, s_information player,int x_max,int y_max,int tab[y_max][x_max])
{
  int limit;
  list_ptr copy_ennemi=ennemi;

  while (copy_ennemi!=NULL){
    if (tab[(copy_ennemi->info.position.y+80)/50][(copy_ennemi->info.movement+10)/50]==1 || tab[(copy_ennemi->info.position.y+80)/50][(copy_ennemi->info.movement+40)/50]==1){
      if (copy_ennemi->info.state==0)
	copy_ennemi->info.movement-=5;
      else
	copy_ennemi->info.movement+=5;
    }else{
      if (player.position.y==copy_ennemi->info.position.y)
	limit=200;
      else
	limit=0;
      if (player.movement-limit>copy_ennemi->info.movement){
	copy_ennemi->info.movement+=5;
	copy_ennemi->info.state=1;
      }
      if (player.movement+limit<copy_ennemi->info.movement){
	copy_ennemi->info.movement-=5;
	copy_ennemi->info.state=0;
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
  int distance_down = distance_of_floor(x_max,y_max,tab,ennemi);
  int distance_up = distance_of_ceiling(x_max,y_max,tab,ennemi);

  if (distance_up < 15) {
    ennemi.jump = 0;
  }

  if (ennemi.jump > 2) {
    ennemi.position.y -= 15;
  }

  if (ennemi.jump > 0) {
    ennemi.jump -= 1;
  }


  /* si SAUT et AU SOL */
  if (player.movement>ennemi.movement){
    if (distance_wall_right(x_max,y_max,tab,ennemi)<=10 && distance_down == 0) { 
      ennemi.jump = 7;
    } 
  } else {
    if (distance_wall_left(x_max,y_max,tab,ennemi)<=10 && distance_down == 0) { 
      ennemi.jump = 7;
    } 
  }

  return ennemi;
}



/*
list_ptr ennemis_shots(list_ptr ennemis,list_ptr shots, s_information player,int x_max,int y_max,int tab[y_max][x_max])
{
  list_ptr copy_ennemis=ennemis;
  list_ptr copy_shots=shots;
  while (copy_ennemis!=NULL){
    if (copy_ennemis->info.position.y==player.position.y){
      if (distance_wall_right(x_max,y_max,tab,ennemis->info)+copy_ennemis->info.movement<player.movement){
	//y a un mur
      }else{
	//tir
      }
      if (distance_wall_left(x_max,y_max,tab,ennemis->info)+player.movement<copy_ennemis->info.movement){
	// y a un mur
      }else{
	//tir 
      }
    }
  }
}
*/

list_ptr wall_bang(list_ptr shots,int x_max,int y_max,int tab[y_max][x_max])
{
  list_ptr shots_list=shots;
  while(shots_list!=NULL){
    if(tab[shots_list->info.position.y/50][(shots_list->info.movement+25)/50]==-1 || tab[shots_list->info.position.y/50][(shots_list->info.movement-25)/50]==-1){
	shots_list->info.life=0;
      }
    shots_list=shots_list->next;
  }
  return shots;
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

void door_ennemy(int x_max, int y_max, int tab[y_max][x_max], s_information player, int load, s_time time)
{
  if (load == 0 && time.previous_time_ennemi+1000 <= time.current) {
    // pas de chargement: on ferme la porte
    tab[1][0] = -1;
    tab[1][x_max-1] = -1;
    tab[2][0] = -1;
    tab[2][x_max-1] = -1;
  } else {
    // chargement: on ouvre la porte
    tab[1][0] = 0;
    tab[1][x_max-1] = 0;
    tab[2][0] = 0;
    tab[2][x_max-1] = 0;
  }
}

void door_player(int x_max, int y_max, int tab[y_max][x_max], s_information player, s_time time)
{
  // le joueur s'écarte: on ferme la porte de gauche
  if (player.movement >= 50) {
    tab[5][0] = -1;
    tab[6][0] = -1;
  } else {
    tab[5][0] = 0;
    tab[6][0] = 0;
  }
  // fin du chrono: on ouvre la porte de droite
  if (time.chrono < 0) {
    tab[5][x_max-1] = 0;
    tab[6][x_max-1] = 0;
  } else {
    tab[5][x_max-1] = -1;
    tab[6][x_max-1] = -1;
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
