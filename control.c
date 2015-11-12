/******************************************************************/
/* control.c                                                      */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 21/10/15                                             */
/* Last modification: 26/10/15                                    */
/******************************************************************/

#include "constant.h"
#include "game.h"
#include "draw.h"
#include "control.h"

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


list_ptr shooting(s_information player,list_ptr shots, int *ammo,int *previous_time,int *current_time)
{
  int reload=6;
  Uint8 *keystate = SDL_GetKeyState(NULL);
  if (keystate[SDLK_SPACE] && *ammo<=30 && *ammo>0 && (*previous_time>=*current_time+500 || *previous_time<=*current_time-500)) {
    *previous_time=*current_time;
    s_information bullet;
    bullet.life=1;
    *ammo-=reload;
    bullet.life=1;
    if (player.state == 0) {
      bullet.position.x = player.position.x+60;
      bullet.movement = player.movement-13+60;
      bullet.rcSrc.x = 0;  
    } else {
      bullet.rcSrc.x = 8;
      bullet.position.x = player.position.x+20;
      bullet.movement = player.movement-13+20;
    }
    bullet.rcSrc.y = 0;
    bullet.rcSrc.w = 8;
    bullet.rcSrc.h = 6;
    bullet.position.y = player.position.y+50;

    shots = list_cons(shots, bullet);
    if (player.state == 1) {
      shots->info.state = 1;
    } else {
      shots->info.state = 0;
    }
  }else{
    if (*current_time-*previous_time>=3000 && *ammo<30){
      *ammo+=reload;
      *previous_time=*current_time;
      if (*ammo>25 && *ammo<=35){
	*ammo=30;
      }
    }
  }
  return shots;
}

/**********************************************************/

s_information move_right(int x_max, int y_max, int tab[y_max][x_max], s_information player) 
{
  Uint8 *keystate = SDL_GetKeyState(NULL);
  int distance, modulo;

  if (keystate[SDLK_RIGHT] && !keystate[SDLK_LEFT]) {
    if (player.state == 0) {

      modulo = 20-player.movement%20;
      distance = distance_wall_right(x_max,y_max,tab,player);
      if (player.movement >= 800/2 && player.movement < (x_max*50)-(800/2)) {
	// on avance dans la map
	if (distance >= 20 && modulo == 0) {   
	  player.movement += 20;
	} else if (distance > modulo && modulo != 0) {
	  player.movement += modulo;
	} else {
	  player.movement += distance;
	}
      } else {
	// on avance dans la map + l'écran
	if (distance >= 20 && modulo == 0) {   
	  player.position.x += 20;
	  player.movement += 20;
	} else if (distance > modulo && modulo != 0) {
	  player.position.x += modulo;
	  player.movement += modulo;
	} else {
	  player.position.x += distance;
	  player.movement += distance;
	}
      }
    } else {
      player.state = 0;
    }
  }
  player.map_x = player.movement/50;

  return player;
}

s_information move_left(int x_max, int y_max, int tab[y_max][x_max], s_information player) 
{
  Uint8 *keystate = SDL_GetKeyState(NULL);
  int distance, modulo;

  if (keystate[SDLK_LEFT] && !keystate[SDLK_RIGHT]) {

    if (player.state == 1) {

      modulo = player.movement%20;
      distance = distance_wall_left(x_max,y_max,tab,player);
      if (player.movement > 800/2 && player.movement <= (x_max*50)-(800/2)) {
	// on avance dans la map + l'écran
	if (distance >= 20 && modulo == 0) {   
	  player.movement -= 20;
	} else if (distance > modulo && modulo != 0) {
	  player.movement -= modulo;
	} else {
	  player.movement -= distance;
	}
      } else {
	// on avance dans la map + l'écran
	if (distance >= 20 && modulo == 0) {   
	  player.position.x -= 20;
	  player.movement -= 20;
	} else if (distance > modulo && modulo != 0) {
	  player.position.x -= modulo;
	  player.movement -= modulo;
	} else {
	  player.position.x -= distance;
	  player.movement -= distance;
	}
      }
    } else {
      player.state=1;
    }
  }
  player.map_x = player.movement/50;

  return player;
}

s_information move_jump(int x_max, int y_max, int tab[y_max][x_max], s_information player)
{
  Uint8 *keystate = SDL_GetKeyState(NULL);
  int distance = distance_of_floor(x_max,y_max,tab,player);
  if (player.jump > 2) {
    player.position.y -= 15;
  }

  if (player.jump > 0) {
    player.jump -= 1;
  }

  /* si SAUT et AU SOL */
  if (keystate[SDLK_UP] && distance == 0) { 
    player.jump = 7;
  } 

  /* pour couper en vol */
  if (!keystate[SDLK_UP] && distance >= 65) {
    player.jump = 0;
  }


  if (keystate[SDLK_DOWN]) {
    player.position.y += 1;
  }

  return player;
}
