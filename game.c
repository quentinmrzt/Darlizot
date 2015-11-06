/******************************************************************/
/* game.c                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 06/11/15                                    */
/******************************************************************/

#include "constant.h"
#include "draw.h"
#include "game.h"

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
  if (copy_list->info.life==0){
    if (copy_list->next==NULL){
      printf("swag\n");
      tmp=copy_list;
      free(tmp);
      return NULL;
    }else{
      printf("gottagofast\n");
      tmp=copy_list;
      list=copy_list->next;
    }
  }else{
    while (copy_list!=NULL && copy_list->next!=NULL)
      {
	if (copy_list->next->info.life==0)
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
  return sprite;
}

s_information ini_player(s_information player) 
{
  player.id = 0;
  player.rcSrc.x = 0;
  player.rcSrc.y = 0;
  player.rcSrc.w = 75;
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
      ennemi_info.position.x =  (rand()%600)+100;
      ennemi_info.id = 1;
      ennemi_info.position.x = (rand()%600)+100;
      ennemi_info.movement = ennemi_info.position.x+20;
      ennemi_info.rcSrc.x = 11*75;

      ennemi = list_cons(ennemi, ennemi_info);
    }
    return ennemi;
  }
  return ennemi;
}

int update_ennemi(int nb_ennemi,list_ptr ennemi)
{
  int res=0;
  list_ptr ennemi_list=ennemi;
  while (ennemi_list!=NULL) {
      if (ennemi_list->info.life<=0) {
	//Supprimer de la liste
      }
      ennemi_list=ennemi_list->next;
    }
  if (nb_ennemi>0) { 
    res = nb_ennemi-1;
  }

  return res;
}


/****************************************************************************************************/
/* PHYSICS */

int distance_wall_left(int x_max, int y_max, int tab[y_max][x_max], s_information player) 
{
  int i;

  for (i=player.map_x ; i>(player.movement-2*75)/50; i--) {
    if (i >= 0) {
      if (tab[(player.position.y+74)/50][i] == -1) {
	// -50 pour coin de gauche
	return (player.movement)-(i*50)-50;
      }
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

  // 
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
	return player.movement;
      } else {
	return (i*50)-(player.position.y+75);
      }
    }
  }

  return player.position.y+75;
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
}





/*********************************************/



int boundingbox(int x_max, int y_max, int tab[y_max][x_max], s_information player) 
{
  int x1 = player.position.x/50;
  int x2 = (player.position.x+75)/50;
  int y1 = player.position.y/50;
  int y2 = (player.position.y+75)/50;

  if (tab[y1][x1] != 0 || tab[y2][x2] != 0 || tab[y1][x2] != 0 || tab[y2][x1] != 0) {
    return 1;
  }
  return 0;
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
  list_ptr shot = NULL;
  list_ptr ennemis = NULL;

  list_ptr copy_shot = NULL;
  list_ptr copy_ennemis = NULL;

  shot=*shots;
  ennemis=*ennemi;

  copy_shot = shot;
  while (copy_shot != NULL) {
    copy_ennemis = ennemis;
    while (copy_ennemis != NULL) {
      if (collision_AABB(copy_shot->info,copy_ennemis->info)){
	if (copy_ennemis->info.life != 0 && copy_shot->info.life != 0) {
	  copy_ennemis->info.life = 0;
	  copy_shot->info.life = 0;
	}
      }
      copy_ennemis = copy_ennemis->next;
    }
    copy_shot = copy_shot->next;   
  }

  ennemis = list_element_delete(ennemis);
  shot = list_element_delete(shot);
  
  *ennemi=ennemis;
  *shots=shot;

}

  /* https://openclassrooms.com/courses/modifier-une-image-pixel-par-pixel */
Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
  int nbOctetsParPixel = surface->format->BytesPerPixel;
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;

  switch(nbOctetsParPixel) {
  case 1:
    return *p;
  case 2:
    return *(Uint16 *)p;
  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
      return p[0] << 16 | p[1] << 8 | p[2];
    else
      return p[0] | p[1] << 8 | p[2] << 16;
  case 4:
    return *(Uint32 *)p;
  default:
    return 0; 
  }
}



void foot_pos(int x_max, int y_max, int tab[y_max][x_max], s_information player, SDL_Surface *surface, int *pos_min, int *pos_max) 
{
  Uint8 r,g,b,a;
  int x, y, min, max;
  Uint32 pixel;
  SDL_LockSurface(surface);

  *pos_min = 0;
  *pos_max = 0;

  if (boundingbox(x_max,y_max,tab,player)) {
    min = surface->w;
    max = 0;
    y = player.rcSrc.y+75-1;
    
    for (x=player.rcSrc.x ; x<player.rcSrc.x+75 ; x++) {
      pixel = get_pixel(surface,x,y);
      SDL_GetRGBA(pixel, surface->format,&r,&g,&b,&a);
	         
      if (r != 255 || g != 0 || b != 255) {
	if (min > x) {
	  min = x;
	}
	if (max < x) {
	  max = x; 
	}
      }
    }
    *pos_min = min%75 + player.position.x;
    *pos_max = max%75 + player.position.x;
  } else {
    *pos_min = player.position.x;
    *pos_max = player.position.x+75;
  }

  SDL_UnlockSurface(surface);
}
