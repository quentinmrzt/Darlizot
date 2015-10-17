/******************************************************************/
/* game.c                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 20/09/15                                             */
/* Last modification: 15/10/15                                    */
/******************************************************************/

#include "constant.h"
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
  /*name[15] = '4';
    sprite.bullet = load(sprite.bullet, name, sprite.screen);*/
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

/****************************************************************************************************/
/* KEYBOARD AND MOUSE */

int quit(int close) 
{
  SDL_Event event;
  /* look for an event */
  if (SDL_PollEvent(&event)) {
    /* an event was found */
    switch (event.type) {
      /* close button clicked */
    case SDL_QUIT:
      close = 1;
      break;	 
      /* handle the keyboard */     
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
	close = 1;
	break;
      }
      break;
    }
  }
  return close;
}

s_information move_map(s_information player, int movement) 
{
  player.movement += movement;

  if (player.movement >= 50) {
    player.movement -= 50;
    player.map_x += 1;
  }
  if (player.movement <= -50) {
    player.movement += 50;
    player.map_x -= 1;
  }

  return player;
}

s_information move_right(int x_max, int y_max, int tab[y_max][x_max], s_information player) 
{
  Uint8 *keystate = SDL_GetKeyState(NULL);

  if (keystate[SDLK_RIGHT] && !keystate[SDLK_LEFT]) {
    if (player.state == 0) {
      if (distance_wall_right(x_max,y_max,tab,player) >= 20) {   
	player.position.x+=20;
	player = move_map(player,20);
      } else {
	player.position.x+=distance_wall_right(x_max,y_max,tab,player)+13;
	player = move_map(player,distance_wall_right(x_max,y_max,tab,player)+13);
      }
    } else {
      player.state = 0;
    }
  }

  return player;
}

s_information move_left(int x_max, int y_max, int tab[y_max][x_max], s_information player) 
{
  Uint8 *keystate = SDL_GetKeyState(NULL);

  if (keystate[SDLK_LEFT] && !keystate[SDLK_RIGHT]) {

    if (player.state == 1) {
      if (distance_wall_left(x_max,y_max,tab,player) >= 20) { 
	player.position.x-=20;
	player = move_map(player,-20);
      } else {
	player.position.x-=distance_wall_left(x_max,y_max,tab,player)+13;
	player = move_map(player,-(distance_wall_left(x_max,y_max,tab,player)+13));
      }      
    } else {
      /* sprite à gauche */
      player.state=1;
    }
  }

  return player;
}

s_information move_jump(int x_max, int y_max, int tab[y_max][x_max], s_information player)
{
  Uint8 *keystate = SDL_GetKeyState(NULL);

  if (player.jump > 2) {
    player.position.y -= 15;
  }

  if (player.jump > 0) {
    player.jump -= 1;
  }

  /* si SAUT et AU SOL */
  if (keystate[SDLK_UP] && distance_of_floor(x_max,y_max,tab,player) == 0) { 
    player.jump = 7;
  } 

  /* pour couper en vol */
  if (!keystate[SDLK_UP] && distance_of_floor(x_max,y_max,tab,player) >= 65) {
    player.jump = 0;
  }

  return player;
}

list_ptr ennemi_spawn(s_information player,list_ptr ennemi,int nb_ennemi,int x_max, int y_max,int tab[y_max][x_max])
{
  if(nb_ennemi<10){
    s_information ennemi_info;
    SDL_Rect ennemi_ini_pos,ennemi_ini_rcSrc;
    ennemi_info=ini_player(ennemi_info);
    do {
      ennemi_ini_pos.x=rand()%800;
      ennemi_ini_pos.y=0; 
    }
    while (ennemi_ini_pos.x == player.position.x && ennemi_ini_pos.y == player.position.y);
    if (ennemi_ini_pos.x>player.position.x){
      ennemi_ini_rcSrc.x=11*75;
    }else{
      ennemi_ini_rcSrc.x=0;
    }
    ennemi_ini_rcSrc.y=0;
    ennemi_ini_rcSrc.h=75;
    ennemi_ini_rcSrc.w=75;
    ennemi=list_cons(ennemi,0,ennemi_ini_pos,ennemi_ini_rcSrc,0);	
  }
  return ennemi;
}

void print_ennemi(list_ptr ennemi, int nb_ennemi)
{
}
list_ptr shooting(s_information player,list_ptr shots)
{
  Uint8 *keystate = SDL_GetKeyState(NULL);
  if (keystate[SDLK_SPACE])
    {
      SDL_Rect bullet_ini_pos,bullet_ini_rcSrc;
      if (player.state==0)
	bullet_ini_rcSrc.x=0;  
      else
	bullet_ini_rcSrc.x=8;
      bullet_ini_rcSrc.y=0;
      bullet_ini_pos.x=player.position.x+40;
      bullet_ini_pos.y=player.position.y+30;
      shots=list_cons(shots,0,bullet_ini_pos,bullet_ini_rcSrc,0);
    }
  return shots;
}

s_information control(int x_max, int y_max, int tab[y_max][x_max], s_information player)
{
  /* animation */
  player = anim_right(x_max,y_max,tab,player);  
  player = anim_left(x_max,y_max,tab,player);
  player = anim_jump(x_max,y_max,tab,player);
  player = anim_shoot(x_max,y_max,tab,player);

  /* move */
  player = move_right(x_max,y_max,tab,player);
  player = move_left(x_max,y_max,tab,player);
  player = move_jump(x_max,y_max,tab,player);

  return player;
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
    if (tab[i][(player.position.x+37)/50] == -1) {
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

  /****************************************************************************************************/
  /* DRAW */

void draw(int x_max, int y_max, int tab[y_max][x_max], s_surface sprite) 
{
  int x,y;
  SDL_Rect pos_sprite,pos_screen;

  pos_sprite.x = 0;
  pos_sprite.y = 0;
  pos_sprite.w = 50;
  pos_sprite.h = 50;

  for (y=0 ; y<y_max ; y++) {
    for (x=0;x<x_max;x++) {
      pos_screen.x = x*50;
      pos_screen.y = y*50;

      if (tab[y][x] == 0) {
	SDL_BlitSurface(sprite.background, &pos_sprite, sprite.screen, &pos_screen);
      }

      if (tab[y][x] == -1) {
	SDL_BlitSurface(sprite.block, &pos_sprite, sprite.screen, &pos_screen);
      }
    }
  }
}

s_information anim_right(int x_max, int y_max, int tab[y_max][x_max], s_information player) 
{
  Uint8 *keystate = SDL_GetKeyState(NULL);

  if (keystate[SDLK_RIGHT] && !keystate[SDLK_LEFT]) {
    /* AU SOL */
    if (distance_of_floor(x_max,y_max,tab,player) == 0) {
      player.rcSrc.y=0;
      if (player.rcSrc.x==0 || player.rcSrc.x>=10*75) {
	player.rcSrc.x=75;
      } else {
	player.rcSrc.x+=75;
      }
    } else {	
      player.rcSrc.y=0;
      player.rcSrc.x=3*75;
    }
  } else {
    /* PAS DE TOUCHE: STATIC */
    if (player.state==0) {
      player.rcSrc.x=0;
      player.rcSrc.y=0;
    }
  }

  return player;
}

s_information anim_left(int x_max, int y_max, int tab[y_max][x_max], s_information player) 
{
  Uint8 *keystate = SDL_GetKeyState(NULL);

  if (keystate[SDLK_LEFT] && !keystate[SDLK_RIGHT]){
    /* AU SOL */
    if (distance_of_floor(x_max,y_max,tab,player) == 0) {
      /* direction droite ou au bout des sprites */
      player.rcSrc.y=0;
      if (player.rcSrc.x<12*75 || player.rcSrc.x==21*75) {
	player.rcSrc.x=12*75;
      } else {
	player.rcSrc.x+=75;
      }
    } else {
      player.rcSrc.y=0;
      player.rcSrc.x=14*75;
    }
  } else {
    /* PAS DE TOUCHE: STATIC */
    if (player.state==1) {
      player.rcSrc.x=11*75;
      player.rcSrc.y=0;
    }
  }

  return player;
}

s_information anim_jump(int x_max, int y_max, int tab[y_max][x_max], s_information player)
{
  /* JUMP */ 
  /* si EN SAUT  */
  if (player.jump > 0) {
    //deuxieme ligne de sprite 
    player.rcSrc.y=75;
    if (player.state==0) {
      player.rcSrc.x=75;
    } else {
      player.rcSrc.x=3*75;
    }
  }

  /* si PAS DE SAUT mais PAS AU SOL */
  if (player.jump == 0 && distance_of_floor(x_max,y_max,tab,player) != 0) {
    player.rcSrc.y = 75;
    if (player.state == 0) {
      player.rcSrc.x = 75*2;
    } else {
      player.rcSrc.x = 75*5;
    }   
  }

  return player;
}

s_information anim_shoot(int x_max, int y_max, int tab[y_max][x_max], s_information player)
{
  Uint8 *keystate = SDL_GetKeyState(NULL);
  
  if (keystate[SDLK_SPACE]) {
    player.rcSrc.y=75*2;
    if (player.jump==1) {
      if (player.state==0) {
	player.rcSrc.x=23*75;
      } else {
	player.rcSrc.x=22*75;
      }
    }
  }

  return player;
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
}
