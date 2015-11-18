/******************************************************************/
/* game.c                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 17/11/15                                    */
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
  name[14] = '1';
  name[15] = '0';
  sprite.outline = load(sprite.outline, name, sprite.screen);
  name[15] = '1';
  sprite.locked = load(sprite.locked, name, sprite.screen);
  name[15] = '2';
  sprite.download = load(sprite.download, name, sprite.screen);
  name[15] = '3';
  sprite.unlocked = load(sprite.unlocked, name, sprite.screen);
  name[15] = '4';
  sprite.black = load(sprite.black, name, sprite.screen);
  return sprite;
}

s_information ini_player(s_information player) 
{
  player.id = 0;
  player.rcSrc.x = 0;
  player.rcSrc.y = 0;
  player.rcSrc.w = 75;
  player.life=5;
  player.rcSrc.h = 75;
  player.position.x = -75;
  player.position.y = 400-50-75;
  player.jump = 0;
  player.dying=0;
  player.state = 0;
  player.movement = player.position.x+13;

  return player;
}

s_time ini_time(s_time time, int map)
{
  time.current = 0; 
  time.previous_time = 0;
  time.previous_time_hit=0;
  time.previous_time_ennemi_hit=0;
  time.previous_time_ennemi = -1000;
  time.level = 0;
  time.time_max = 10000;
  time.chrono = time.time_max;
  time.menu = 0;
  time.dead = 0;

  if (map==0 || map==1) {
    time.time_max = 0;
    time.chrono = 0;
  }

  if (map == 0) {
    time.birth = 0;
  }

  return time;
}

s_time duration_chrono(s_information player,s_time time ,int x_max,int y_max,int tab[y_max][x_max],int map) 
{  
  // DEBUT DU CHRONO
  if (player.movement/50 == x_max-2 && time.level == 0) {
    time.level = time.current;
    if (map!=0 && map!=1){
      tab[y_max-2][x_max-2]=3;
    }
  }
  // DEFILER DU CHRONO
  if (time.level != 0 && time.chrono >= 0) {
    time.chrono = time.time_max - time.current + time.level;
  }

  return time;
}

void change_map(int *map_ptr, int *previous_map_ptr)
{
  int map = *map_ptr;
  int previous_map = *previous_map_ptr;

  /* choix de la map */
  if (previous_map == 0 && map == 0) {
    map = 2;
  } else if (map == 1) {
    map = previous_map+1;
  } else {
    previous_map = map;
    map = 1;
  }

  if (map > nb_map()-1) {
    map = 2;
    previous_map = 0;
  }

  *map_ptr = map;
  *previous_map_ptr = previous_map;
}

void change_lvl(s_information *player_ptr, s_time *time_ptr, list_ptr *shots_ptr, list_ptr *ennemi_ptr, list_ptr *army_shots_ptr, int *load_ptr, int *level,int *x_max, int *y_max, int map, int tab[*y_max][*x_max])
{
  s_information player = *player_ptr;
  s_time time = *time_ptr;

  player = ini_player(player);

  free_list(ennemi_ptr);
  free_list(shots_ptr);
  free_list(army_shots_ptr);
    
  *load_ptr = 0;
  /* lvl up */
  if (map > 1) {
    *level = *level+1;
  }
  size_tab(x_max,y_max,map);
  recup_map(*x_max,*y_max,tab,map);
  time = ini_time(time,map);

  *player_ptr = player;
  *time_ptr = time;
}

void set_menu(int map,int *action_ptr,int *choice_ptr,s_information *player_ptr,int x_max,int y_max,int tab[y_max][x_max],s_time *time_ptr) 
{
  s_information player = *player_ptr;
  s_time time = *time_ptr;

  if (map == 0) {
    if (*action_ptr == 0 || *action_ptr == 2) {
      // LANCEMENT
      if (player.movement < x_max*50/2-10) {
	player = control_auto(x_max,y_max,tab,player,(x_max*50)/2-(x_max*50/2)%2);
      } else {
	player.rcSrc.x = 0;
	time = control_menu(x_max,choice_ptr,action_ptr,time);
      }
    } else if (*action_ptr == 1) {
      // JOUER
      player = control_auto(x_max,y_max,tab,player,x_max*50);
    } else if (*action_ptr == 2) {
      // RIEN
      //*action_ptr = 0;
    } else {
      // QUITTER
      player = control_auto(x_max,y_max,tab,player,0);
    }
  }

  *player_ptr = player;
  *time_ptr = time;
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
  if (copy_list->info.life==-1 || copy_list->info.position.y>450){
    if (copy_list->next==NULL){
      tmp=copy_list;
      return NULL;
    }else{
      tmp=copy_list;
      list=copy_list->next;
    }
  }else{
    while (copy_list!=NULL && copy_list->next!=NULL)
      {
	if (copy_list->next->info.life==-1 || copy_list->next->info.position.y>450)
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
      ennemi_info.limit=rand()%100+250;
      ennemi_info.position.y = 100;
      ennemi_info.id = 1;
      ennemi_info.movement = ennemi_info.position.x+20;
      ennemi = list_cons(ennemi, ennemi_info);
    }
    return ennemi;
  }
  return ennemi;
}


list_ptr respawn(list_ptr ennemi,int *level, s_information player,s_time *time_p,int *nb_ennemi_spawn,int *load,int x_max, int y_max,int tab[y_max][x_max], int map)
{
  if (map >= 2) {
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
	  //*level=*level+1;
	  *nb_ennemi_spawn=0;
	}
      }
    }else{
      if(ennemi==NULL /*&& time.chrono > 0*/) {
	*load=1;
      }
    }
    *time_p = time;
  }
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
    if (copy_ennemi->info.life>0){
      if (player.position.y==copy_ennemi->info.position.y)
	limit=copy_ennemi->info.limit;
      else
	limit=0;
      copy_ennemi->info.rcSrc.y=0;
      if (tab[(copy_ennemi->info.position.y+80)/50][(copy_ennemi->info.movement+10)/50]==1 || tab[(copy_ennemi->info.position.y+80)/50][(copy_ennemi->info.movement+40)/50]==1){
	if (copy_ennemi->info.state==0){
	  copy_ennemi->info.movement+=5;
	  copy_ennemi->info.state=0;
	} else{
	  copy_ennemi->info.movement-=5;
	  copy_ennemi->info.state=1;
	}
      }else{
	if (player.movement>=copy_ennemi->info.movement ){
	  copy_ennemi->info.state=0;
	  if (player.movement-limit>=copy_ennemi->info.movement)
	    copy_ennemi->info.movement+=5;
	}else{
	  if (player.movement<=copy_ennemi->info.movement){
	    copy_ennemi->info.state=1;
	    if (player.movement+limit<=copy_ennemi->info.movement)
	      copy_ennemi->info.movement-=5;
	  }
	}
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
    if (distance_wall_right(x_max,y_max,tab,ennemi)<=10 && distance_down == 0 && player.movement!=ennemi.movement) { 
      ennemi.jump = 7;
    } 
  } else {
    if (distance_wall_left(x_max,y_max,tab,ennemi)<=10 && distance_down == 0  && player.movement!=ennemi.movement) { 
      ennemi.jump = 7;
    } 
  }

  return ennemi;
}

list_ptr ennemis_shots(list_ptr ennemis,list_ptr army_shots, s_information player,int x_max,int y_max,int tab[y_max][x_max],s_time *time_p)
{
  list_ptr copy_ennemis=ennemis;
  list_ptr copy_shots=army_shots;
  int limit;
  s_information bullet;
  s_time time=*time_p;
  while (copy_ennemis!=NULL){
    if (player.position.y==copy_ennemis->info.position.y)
      limit=copy_ennemis->info.limit;
    else
      limit=0;
    if (copy_ennemis->info.position.y==player.position.y && player.life>0 &&  time.previous_time_ennemi_hit<=time.current-1200 && copy_ennemis->info.life>0){
      time_p->previous_time_ennemi_hit=time_p->current;
      bullet.life=1;
      bullet.rcSrc.y = 0;
      bullet.rcSrc.w = 8;
      bullet.rcSrc.h = 6;
      bullet.position.y=copy_ennemis->info.position.y+50;
      if (distance_wall_right(x_max,y_max,tab,ennemis->info)>player.movement-copy_ennemis->info.movement -2
	  && player.movement-limit<=copy_ennemis->info.movement 
	  && copy_ennemis->info.state==0){
	bullet.movement=copy_ennemis->info.movement+60-20;
	bullet.position.x=copy_ennemis->info.position.x+60;
	bullet.rcSrc.x=16;
	bullet.state=0;
	army_shots=list_cons(army_shots,bullet);
      }
      if (copy_ennemis->info.movement - distance_wall_left(x_max,y_max,tab,ennemis->info)<player.movement +2
	  && player.movement+limit>=copy_ennemis->info.movement
	  && copy_ennemis->info.state==1){
	bullet.movement=copy_ennemis->info.movement+20;
	bullet.position.x=copy_ennemis->info.position.x+20;
	bullet.rcSrc.x=24;
	bullet.state=1;
	army_shots=list_cons(army_shots,bullet);
      }
    }
    copy_ennemis=copy_ennemis->next;
  }
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

list_ptr kill_all(list_ptr ennemis)
{
  list_ptr copy_ennemis=ennemis;

  while (copy_ennemis!=NULL) {
    free(ennemis);
    copy_ennemis = copy_ennemis->next;
  }

  return ennemis;
}

/****************************************************************************************************/
/* TAB */

void size_tab(int *x_ptr, int *y_ptr, int map) 
{
  FILE* recuperation;
  int number, end, x;
  *x_ptr = 0;
  *y_ptr = 0;
  recuperation = NULL;
  x = 0;
  number=0;

  char name[15] = "data/map_0";
  char tmp[15] = "";
  sprintf (tmp,"%d",map);
  name[9] = tmp[0];
  name[10] = tmp[1];

  /* mode read */
  recuperation = fopen(name, "r");

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

int nb_map(void) 
{
  int compteur=0, fin=0;  
  char name[15] = "data/map_0";
  char tmp[15] = "";

  while (fin == 0) {
    sprintf (tmp, "%d", compteur) ;
    name[9] = tmp[0];
    name[10] = tmp[1];
    if (fopen(name, "r") != NULL) {
      compteur++;
    } else {
      fin = 1;
    }
  }

  return compteur;
}

void recup_map(int x_max, int y_max, int tab[y_max][x_max], int map)
{
  FILE* recuperation;
  int x, y,number;
  number = 0;
  recuperation = NULL;
  
  char name[15] = "data/map_0";
  char tmp[15] = "";
  sprintf (tmp,"%d",map);
  name[9] = tmp[0];
  name[10] = tmp[1];

  /* mode read */
  recuperation = fopen(name, "r");

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

void door_ennemy(int x_max, int y_max, int tab[y_max][x_max], s_information player, int load, s_time time, int map)
{
  if ((load == 0 && time.previous_time_ennemi+1000 <= time.current && player.movement >=49) || map <= 1) {
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

void door_player(int x_max, int y_max, int tab[y_max][x_max], s_information player, s_time time, int map)
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
  if (time.chrono <= 0) {
    tab[5][x_max-1] = 0;
    tab[6][x_max-1] = 0;
    if (map!=0 && map!=1){
      tab[y_max-2][x_max-2]= 4;
    }
  } else {
    tab[5][x_max-1] = -1;
    tab[6][x_max-1] = -1;
  }

  // cas pour le menu
  if (map == 0 && player.movement <= 50) {
    tab[5][0] = 0;
    tab[6][0] = 0;
  }
}

int nb_one(int tab_end[400/50][800/50])
{
  int x,y,one;

  one = 0;
  for (y=0 ; y<400/50 ; y++) {
    for (x=0 ; x<800/50 ; x++) {
      if (tab_end[y][x] == 1) {
	one++;
      }
    }
  }
  return one;
}

void put_zero(int tab_end[400/50][800/50],int nb)
{
  int one = 0;
  int x,y;

  while (one <= nb && nb_one(tab_end) != 0) {
    x = rand()%(800/50);
    y = rand()%(400/50);

    if (tab_end[y][x] != 0) {
      tab_end[y][x] = 0;
      one++; ;
    }
  }
}

void all_one(int tab_end[400/50][800/50])
{
  int x,y;
  for (y=0 ; y<400/50 ; y++) {
    for (x=0 ; x<800/50 ; x++) {
      tab_end[y][x] = 1;
    }
  }
}

/****************************************************************************************************/
/* SCORING */

int scoring(int score, int level)
{
  score+=10*level;
  return score;
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
  SDL_FreeSurface(sprite.ammo_set);
  SDL_FreeSurface(sprite.platform);
  SDL_FreeSurface(sprite.health);
  SDL_FreeSurface(sprite.locked);
  SDL_FreeSurface(sprite.download);
  SDL_FreeSurface(sprite.unlocked);
  SDL_FreeSurface(sprite.outline);
  SDL_FreeSurface(sprite.black);
}

list_ptr list_tail(list_ptr list) 
{
  if (list==NULL || list->next==NULL) {
    return NULL;
  }
  return list->next;
}

void free_list(list_ptr *list) 
{
  list_ptr tmp;
  while (*list!=NULL) {
    tmp = *list;
    *list= list_tail(*list);
    free(tmp);
  }
}

/********************************************************************************************************************/



void recuperation(int ranking[]) 
{
  FILE* recuperation;
  int i = 0;
  recuperation = NULL;
  /* mode read */
  recuperation = fopen("data/ranking.txt", "r");
  /* if file does not exist */
  if (recuperation == NULL) {
    create_ranking();
    recuperation = fopen("data/ranking.txt", "r+");
  }

  if (recuperation != NULL) {
    /* travel result */
    rewind(recuperation);
    while (i < 10) {
      fscanf(recuperation, "%d", &ranking[i]);
      i++;
    }
    fclose(recuperation);	
  }
}

void save(int score)
{
  int i, over, j;
  FILE* saving;
  int ranking[10] = {0};
  saving = NULL;
  /* mode read/write */
  saving = fopen("data/ranking.txt", "r+");
  /* if file does not exist */
  if (saving == NULL) {
    create_ranking();
    saving = fopen("data/ranking.txt", "r+");
  }

  if (saving != NULL) {
    i = 0;
    j = 9;
    over = 0; 
    /* recovery scores */
    recuperation(ranking);
    /* sort.. */
    i = 0;
    while(i < 10 && !over) {
      if (ranking[i] < score) {
	while (j > i) {
	  ranking[j] = ranking[j-1];
	  j--;
	}
	ranking[i] = score;
	over = 1;
      }
      i++;
    }
    /* ..and prints */
    if (over) {
      i = 0;
      rewind(saving);
      while(i < 10) {
	fprintf(saving,"%d\n",ranking[i]);
	i++;
      }
    }
    fclose(saving);
  }
}

void create_ranking(void)
{
  FILE* ranking;
  ranking = fopen("data/ranking.txt", "w");
  int i = 0;
  while(i < 10) {
    fprintf(ranking,"0\n");
    i++;
  }
  rewind(ranking);
}
