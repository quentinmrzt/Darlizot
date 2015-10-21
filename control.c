/******************************************************************/
/* control.c                                                      */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 21/10/15                                             */
/* Last modification: 21/10/15                                    */
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

list_ptr shooting(s_information player,list_ptr shots)
{
  Uint8 *keystate = SDL_GetKeyState(NULL);
  if (keystate[SDLK_SPACE])
    {
      SDL_Rect bullet_ini_pos,bullet_ini_rcSrc;
      if (player.state==0)
	{
	  bullet_ini_pos.x=player.position.x+60;
	  bullet_ini_rcSrc.x=0;  
	}
      else
	{
	  bullet_ini_rcSrc.x=8;
	  bullet_ini_pos.x=player.position.x+20;
	}
      bullet_ini_rcSrc.y=0;
      bullet_ini_rcSrc.w=8;
      bullet_ini_rcSrc.h=6;
      bullet_ini_pos.y=player.position.y+50;
      shots=list_cons(shots,0,bullet_ini_pos,bullet_ini_rcSrc,0);
      if (player.state==1)
	shots->info.state=1;
    }
  return shots;
}

/**********************************************************/

s_information move_right(int x_max, int y_max, int tab[y_max][x_max], s_information player) 
{
  Uint8 *keystate = SDL_GetKeyState(NULL);

  if (keystate[SDLK_RIGHT] && !keystate[SDLK_LEFT]) {
    if (player.state == 0) {
      if (distance_wall_right(x_max,y_max,tab,player) >= 20) {   
	player.position.x+=20;
	player = move_map(player,20);
      } else {
	player.position.x+=distance_wall_right(x_max,y_max,tab,player);
	player = move_map(player,distance_wall_right(x_max,y_max,tab,player));
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
	player.position.x-=distance_wall_left(x_max,y_max,tab,player);
	player = move_map(player,-(distance_wall_left(x_max,y_max,tab,player)));
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
