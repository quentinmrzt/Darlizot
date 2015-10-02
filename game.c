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
  return sprite;
}

/****************************************************************************************************/
/* TAB OF SPRITE */

/****************************************************************************************************/
/* PHYSICS */

void gravity(SDL_Rect *s1, int *s2)
{
  SDL_Rect sprite=*s1;
  int sol=*s2;
  if (sprite.y< (305-sol))
    sprite.y+=10;
  *s1=sprite;
  *s2=sol;
}

/****************************************************************************************************/
/* KEYBOARD AND MOUSE */

void control(SDL_Rect *p1, SDL_Rect *r1,int *s1, int *s2, int *f1)
{
  SDL_Rect position=*p1;
  SDL_Rect rcSrc=*r1;
  int state=*s1;
  int jump=*s2;
  int sol=*f1;
  int sol2=305-sol;
  
  Uint8 *keystate = SDL_GetKeyState(NULL);

  if (keystate[SDLK_LEFT]){
    position.x-=20;
    if (state!=1)
      state=1;
    if (jump==0 && position.y==sol2)
      {
	if (rcSrc.x<12*75 || rcSrc.x==20*75)
	  rcSrc.x=12*75;
	else{
	  rcSrc.x+=75;
	  rcSrc.y=0;
	}
      }
    else
      {
	rcSrc.y=0;
	rcSrc.x=14*75;
      }
  }
  else
    {
      if (state==1){
	rcSrc.x=11*75;
	rcSrc.y=0;
      }
    }

  if (keystate[SDLK_RIGHT]){
    position.x+=20;
    if (state!=0)
      state=0;
    if (jump==0 && position.y==sol2)
      {
	if (rcSrc.x==0 || rcSrc.x>=10*75)
	  rcSrc.x=75;
	else{
	  rcSrc.x+=75;
	  rcSrc.y=0;
	}
      }
    else
      {
	
	rcSrc.y=0;
	rcSrc.x=3*75;
      }
  }
  else 
    {
      if (state==0){
	rcSrc.x=0;
	rcSrc.y=75;
      }
    }
	 
  if (keystate[SDLK_SPACE]){
  }

  if (keystate[SDLK_UP] && position.y==sol2 && jump==0)
    {
      jump=1;
      rcSrc.y=75;
      position.y-=10; 
    }
  if (position.y>170 && jump==1)
    {
      rcSrc.y=75;
      if (state==0)
	rcSrc.x=75;
      else
	rcSrc.x=3*75; 
      position.y-=10;
      
    }
  else
    {
      rcSrc.y=0;
      jump=0;
    } 
  if (jump==0 && position.y>170 && position.y<sol2 )
    {
      rcSrc.y=75;
      if (state==0)
	rcSrc.x=150;
      else
	rcSrc.x=375;
    }

  *s2=jump;
  *p1=position;
  *r1=rcSrc;
  *s1=state;
  *f1=sol;
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
