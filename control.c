/******************************************************************/
/* control.c                                                      */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 21/10/15                                             */
/* Last modification: 13/11/15                                    */
/******************************************************************/

#include "constant.h"
#include "game.h"
#include "draw.h"
#include "control.h"
#include "physic.h"

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


list_ptr shooting(s_information player,list_ptr shots, int *ammo,int energy,s_time *time_p)
{
  Uint8 *keystate = SDL_GetKeyState(NULL);
  s_time time = *time_p;

  if (keystate[SDLK_SPACE] && *ammo<=60 && *ammo>0 && (time.previous_time >= time.current_time+200 || time.previous_time<=time.current_time-200)) {
    time.previous_time=time.current_time;
    s_information bullet;
    *ammo-=energy;
    if (*ammo<=0){
      *ammo=0;
    }
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
    if (time.current_time-time.previous_time>=2000 && *ammo<60){
      *ammo+=15;
      time.previous_time=time.current_time;
      if (*ammo>=60){
	*ammo=60;
      }
    }
  }

  *time_p = time;
  return shots;
}

void a_and_z(int x_max, int y_max, int tab[y_max][x_max], s_information player) 
{
  Uint8 *keystate = SDL_GetKeyState(NULL);

  if (keystate[SDLK_a]) {
    if (player.id == 0) {
      printf("Joueur:  ");
    } else {
      printf("Ennemi:  ");
    }
    printf("pos_x: %d  pos_y: %d  mov: %d  left: %d  right: %d  up: %d\n",player.position.x,player.position.y,player.movement,distance_wall_left(x_max,y_max,tab,player),distance_wall_right(x_max,y_max,tab,player),distance_of_ceiling(x_max,y_max,tab,player));
  } 
  if (keystate[SDLK_z]) {
    printf("\n");
  }
}

list_ptr killing(list_ptr ennemi)
{
  list_ptr list;
  Uint8 *keystate = SDL_GetKeyState(NULL);
  if (keystate[SDLK_k]){
    list=NULL;
  }else{
    list=ennemi;
  }
  return list;
}
/****************************************************************************************************/
/* MOVE */

s_information move_right(int x_max, int y_max, int tab[y_max][x_max], s_information player) 
{
  Uint8 *keystate = SDL_GetKeyState(NULL);
  int distance, modulo;

  if (keystate[SDLK_RIGHT] && !keystate[SDLK_LEFT]) {
    if (player.state == 0) {

      modulo = 20-player.movement%20;
      distance = distance_wall_right(x_max,y_max,tab,player);
      // gere le decalage de 10px pour map impair 
      if (x_max%2 != 0 && player.movement == x_max*50-800/2-10) {
	player.position.x += 10;
	player.movement += 20;
      } else {
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
	  if ((distance >= 20 && modulo == 0) || player.movement < 0 || player.movement >= x_max*50-75+13*2) {   
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
      // gere le decalage de 10px pour map impair 
      if (x_max%2 != 0 && player.movement == x_max*50-800/2+10) {
	player.position.x -= 10;
	player.movement -= 20;
      } else {
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
	  if ((distance >= 20 && modulo == 0)) {   
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
  int distance_down = distance_of_floor(x_max,y_max,tab,player);
  int distance_up = distance_of_ceiling(x_max,y_max,tab,player);

  if (distance_up < 15) {
    player.jump = 0;
  }

  if (player.jump > 2) {
    player.position.y -= 15;
  }

  if (player.jump > 0) {
    player.jump -= 1;
  }

  /* si SAUT et AU SOL */
  if (keystate[SDLK_UP] && distance_down == 0) { 
    player.jump = 7;
  } 

  /* pour couper en vol */
  if (!keystate[SDLK_UP] && distance_down >= 65) {
    player.jump = 0;
  }


  if (keystate[SDLK_DOWN]) {
    player.position.y += 1;
  }

  return player;
}

