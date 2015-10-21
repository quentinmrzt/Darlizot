/******************************************************************/
/* game.c                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 15/10/15                                    */
/******************************************************************/

#include "constant.h"
#include "draw.h"
#include "game.h"

/****************************************************************************************************/
/* LIST */

list_ptr list_cons(list_ptr list,int lf,SDL_Rect pos,SDL_Rect Src,int st)
{
  list_ptr new = NULL;
  /* dynamic allocation */
  new = (list_ptr) malloc(sizeof(struct s_node));
  
  new->info.life = lf;
  new->info.position=pos;
  new->info.rcSrc=Src;
  new->info.state=st;
  new->info.jump=0;
  new->next = list;

  return new;
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
  /*load sprite*/
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
  return sprite;
}

s_information ini_player(s_information player) 
{
  player.rcSrc.x = 0;
  player.rcSrc.y = 0;
  player.rcSrc.w = 75;
  player.rcSrc.h = 75;
  
  player.position.x = 0;
  player.position.y = 0;
  
  player.jump = 0;

  player.state = 0;

  player.map_x = 0;
  player.movement = 0;

  return player;
}

list_ptr ennemi_spawn(s_information player,list_ptr ennemi,int nb_ennemi,int x_max, int y_max,int tab[y_max][x_max])
{
  s_information ennemi_info;
  SDL_Rect ennemi_ini_pos,ennemi_ini_rcSrc;
  if(nb_ennemi>0){
    ennemi_info=ini_player(ennemi_info);
    ennemi_ini_pos.y=0; 
    ennemi_ini_rcSrc.y=0;
    ennemi_ini_rcSrc.h=75;
    ennemi_ini_rcSrc.w=75;
    if (ennemi_ini_pos.x>player.position.x){
      ennemi_ini_rcSrc.x=11*75;
    }else{
      ennemi_ini_rcSrc.x=0;
    }
    do {
      ennemi_ini_pos.x=(rand()%600)+100;
      ennemi=list_cons(ennemi,0,ennemi_ini_pos,ennemi_ini_rcSrc,0);
    }
    while (ennemi_ini_pos.x == player.position.x && ennemi_ini_pos.y == player.position.y);
    
    
   
  }
  return ennemi;
}

int update_ennemi(int nb_ennemi,list_ptr ennemi)
{
  int res=0;
  list_ptr ennemi_list=ennemi;
  while(ennemi_list!=NULL)
    {
      if(ennemi_list->info.life<=0)
	{
	  //supprimer l'element de la liste
	}
      ennemi_list=ennemi_list->next;
    }
  if (nb_ennemi>0){
  res=nb_ennemi-1;
  }
  return res;
}


/****************************************************************************************************/
/* PHYSICS */

int distance_wall_left(int x_max, int y_max, int tab[y_max][x_max], s_information player) 
{
  int i;

  for (i=player.position.x/50 ; i>(player.position.x-2*75)/50 ; i--) {
    if (tab[(player.position.y+74)/50][i] == -1) {
      // -50 pour coin de gauche
      return (player.position.x)-(i*50)-50;
    }
  }
  return player.position.x;
}

int distance_wall_right(int x_max, int y_max, int tab[y_max][x_max], s_information player) 
{
  int i;

  for (i=(player.position.x+75)/50 ; i<(player.position.x+2*75)/50 ; i++) {
    if (tab[(player.position.y+74)/50][i] == -1) {
      // -50 pour coin de droite
      return (i*50)-(player.position.x+75);
    }
  }
  return 800-(player.position.x+75);
}

int distance_of_floor(int x_max, int y_max, int tab[y_max][x_max], s_information player)
{
  int i; 
  for (i=(player.position.y+75)/50 ; i<(player.position.y+75*5)/50 ; i++) {
    //if (tab[i][(player.position.x+37)/50] == -1) {
    if (tab[i][(player.position.x)/50] == -1 || tab[i][(player.position.x+75)/50]) {
      return (i*50)-(player.position.y+75);
    }
  }
}

s_information gravity(int x_max, int y_max, int tab[y_max][x_max], s_information player)
{
  if (player.jump == 0) {
    if (distance_of_floor(x_max,y_max,tab,player) >= 15) {
      player.position.y += 15;
    } else {
      player.position.y += distance_of_floor(x_max,y_max,tab,player); 
    }
  }

  return player;
}

void ennemi_gravity(int x_max,int y_max,int tab[y_max][x_max],list_ptr ennemi,s_surface sprite)
{
  list_ptr ennemi_list=ennemi;
  while(ennemi_list!=NULL)
    {
      ennemi_list->info=gravity(x_max,y_max,tab,ennemi_list->info);
      ennemi_list=ennemi->next;
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
