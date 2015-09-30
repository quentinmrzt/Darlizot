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
  /*load sprite starship 1 */
  char name[] = "sprite/sprite_01.bmp";
  sprite.player = load(sprite.player, name, sprite.screen);
  name[15] = '2';
  sprite.background = load(sprite.background, name, sprite.screen);
  name[15] = '3';
  sprite.plateform = load(sprite.plateform, name, sprite.screen);
  return sprite;
}

/****************************************************************************************************/
/* KEYBOARD AND MOUSE */


void control(SDL_Rect *p1, SDL_Rect *r1,int *s1, int *sol1)
{
  SDL_Rect position=*p1;
  SDL_Rect rcSrc=*r1;
  int state=*s1;
  int saut=0;
  
  Uint8 *keystate = SDL_GetKeyState(NULL);
  if (keystate[SDLK_LEFT]){
  
    if (state!=1)
      state=1;
    position.x-=20;
    if (rcSrc.x<=7*95 || rcSrc.x==13*95)
      rcSrc.x=8*95;
    else
      rcSrc.x+=95;
  }
  else
    {
      if (state==1)
	rcSrc.x=7*95;
    }

  if (keystate[SDLK_RIGHT]){
    if (state!=0)
      state=0;
    position.x+=20;
    if (rcSrc.x==0 || rcSrc.x>=6*95)
      rcSrc.x=95;
    else
      rcSrc.x+=95;
  }
  else 
    {
      if (state==0)
	rcSrc.x=state;
    }
	 
  if (keystate[SDLK_SPACE]){
    
  }

  if (keystate[SDLK_DOWN]){

  }
  *p1=position;
  *r1=rcSrc;
  *s1=state;
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
