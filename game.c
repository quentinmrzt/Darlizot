/******************************************************************/
/* game.c                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Date creation: 20/09/15                                        */
/* Derniere modification: 20/09/15                                */
/******************************************************************/

#include "constant.h"
#include "game.h"


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
  sprite.plateform = load(sprite.plateform, name, sprite.screen);
  name[15] = '5';
  sprite.block = load(sprite.block, name, sprite.screen);
  return sprite;
}

/****************************************************************************************************/
/* TAB OF SPRITE */

/****************************************************************************************************/
/* PHYSICS */



int distance_wall_left(int tab[][800/50],s_information player) 
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

int distance_wall_right(int tab[][800/50],s_information player) 
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

int distance_of_floor(int tab[][800/50],s_information player) 
{
  int i; 
  for (i=(player.position.y+75)/50 ; i<(player.position.y+75*5)/50 ; i++) {
    if (tab[i][(player.position.x+37)/50] == -1) {
      return (i*50)-(player.position.y+75);
    }
  }
}
void gravity(s_information *player_ptr, int tab[][800/50])
{
  s_information player = *player_ptr;
  
  if (distance_of_floor(tab,player) >= 10) {
    player.position.y += 15;
  } else {
    player.position.y += distance_of_floor(tab,player); 
  }

  *player_ptr = player;
}

/****************************************************************************************************/
/* KEYBOARD AND MOUSE */

void control(int tab[][800/50], s_information *player_ptr)
{
  s_information player = *player_ptr;  
  Uint8 *keystate = SDL_GetKeyState(NULL);

  /* ANIMATIONS */

   if (keystate[SDLK_RIGHT] && !keystate[SDLK_LEFT]) {
    /* AU SOL */
    if (distance_of_floor(tab,player) == 0) {   /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
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

 if (keystate[SDLK_LEFT] && !keystate[SDLK_RIGHT]){
    /* AU SOL */
    if (distance_of_floor(tab,player) == 0) {   
      /* direction droite ou au bout des sprites */
      player.rcSrc.y=0;
      if (player.rcSrc.x<12*75 || player.rcSrc.x==20*75) {
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

    /********************************************************************************************/

 /* JUMP */
 
  /* si EN SAUT  */
  if (player.jump > 0) {
    //deuxieme ligne de sprite 
    player.rcSrc.y=75;
    if (player.state==0) {
      // 0: droite
      player.rcSrc.x=75;
    } else {
      // 1: gauche
      player.rcSrc.x=3*75;
    }
     
    player.position.y-=10;
    player.jump-=10;   
  }

  /* si SAUT et AU SOL */
  if (keystate[SDLK_UP] && distance_of_floor(tab,player) == 0) { /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
    player.jump = 70;
  }

  /* si PAS DE SAUT mais PAS AU SOL */
  if (player.jump == 0 && distance_of_floor(tab,player) != 0) { /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
    player.rcSrc.y = 75;
    if (player.state==0) {
      player.rcSrc.x = 75*2;
    } else {
      player.rcSrc.x = 75*5;
    }   
  }

  /********************************************************************************************/

  if (keystate[SDLK_LEFT] && !keystate[SDLK_RIGHT]){

    if (distance_wall_left(tab,player) >= 20) { 
      player.position.x-=20;
    } else {
      player.position.x-=distance_wall_left(tab,player)+13; 
    }

    /* sprite à gauche */
    player.state=1;
  }

  /********************************************************************************************/

  if (keystate[SDLK_RIGHT] && !keystate[SDLK_LEFT]) {

    if (distance_wall_right(tab,player) >= 20) {     /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
      player.position.x+=20;
    } else {
      player.position.x+=distance_wall_right(tab,player)+13;
    }

    /* sprite a droite */
    player.state = 0;
  }
 

  /********************************************************************************************/
	 
  if (keystate[SDLK_SPACE]) {
    
  }


  
  /********************************************************************************************/

  *player_ptr=player;
}



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


/****************************************************************************************************/
/* CLEAN */

void free_all_sprite(s_surface sprite) 
{
  SDL_FreeSurface(sprite.player);
}
