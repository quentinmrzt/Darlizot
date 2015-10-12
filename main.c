/******************************************************************/
/* main.c                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Date creation: 20/09/15                                        */
/* Derniere modification: 20/09/15                                */
/******************************************************************/

#include "constant.h"
#include "game.h"
#include "time.h"


int main(int argc, char* argv[])
{
  int close,i,previous_plateform_time=0,current_time=0,saut=0,state=2, sol=75;
  int NB_plateform=0;
  s_information player;
  s_surface sprite;
  SDL_Rect position,rcSrc, pos, test, plat, pos_sprite,pos_screen;
  int tab[400/50][800/50] = {0}; // taille de l'écran, carre de 50px
  int x,y;
  list_info enemies=NULL;
  list_info shots=NULL;

  /****************************************************************************************************/
  /* INITIALIZE */

  /* initialize video system */
  SDL_Init(SDL_INIT_VIDEO);
  /* initialize ttf */
  TTF_Init();
  /* set the title bar */
  SDL_WM_SetCaption("S3", NULL);
  /* create window */
  sprite.screen = SDL_SetVideoMode(800, 400, 0, 0);
  /* load sprite */
  sprite = load_sprite(sprite);

  /* initialize variable */
  close = 0;

  /* pos in sprite: player */
  rcSrc.x = 0;
  rcSrc.y = 0;
  rcSrc.w = 75;
  rcSrc.h = 75;
  //player.width = 75;
  //player.height = 75;
  player.rcSrc.x = 0;
  player.rcSrc.y = 0;
  player.rcSrc.w = 75;
  player.rcSrc.h = 75;
  /* pos in screen: player */
  position.x = 0;
  position.y = 305-sol;
  //player.pos_screenX = 0;
  //player.pos_screenY = 305-sol;

  player.position.x = 0;
  player.position.y = 0;

  /* pos in sprite: background */
  test.x = 0;
  test.y = 0;
  test.w = 800;
  test.h = 400;
  /* pos in screen: background */
  pos.x = 0;
  pos.y = 0;

  /* pos in sprite: block */
  pos_sprite.x = 0;
  pos_sprite.y = 0;
  pos_sprite.w = 50;
  pos_sprite.h = 50;

  /* pos in screen: plateform */
  plat.x = 20;
  plat.y = 320+rand()%40;




  /* remplissage tableau manuel */
  for (x=0;x<800/50;x++) {
    tab[7][x] = -1;
  }
  tab[6][0] = -1;
  tab[5][0] = -1;
  tab[6][1] = -1;
  /*
  tab[4][21] = -1;
  tab[4][20] = -1;
  tab[3][20] = -1;
  tab[3][21] = -1;


  tab[4][22] = -1;
  tab[4][23] = -1;
  tab[3][22] = -1;
  tab[3][23] = -1;*/


  tab[6][12] = -1;
  tab[6][13] = -1;
  tab[6][14] = -1;
  tab[6][15] = -1;

  /* affichage du tableau */
  for (y=0;y<400/50;y++) {
    for (x=0;x<800/50;x++) {
      //printf("%d ",tab[y][x]);
    }
    //printf("\n");
  }

  player.jump = 0;





  /* loop for game */
  while (!close)
    {
      /****************************************************************************************************/
      /* TIME */
      current_time=SDL_GetTicks();
      
      /* KEYBOARD AND MOUSE */
      /* croix ou échap */
      close = quit(close);
      /* clavier */
      control(tab,&player);
      
      /* draw bakground */
      SDL_BlitSurface(sprite.background, &test, sprite.screen, &pos);

      /* tableau */
      /* draw floor */
      for (y=0;y<400/50;y++) {
	for (x=0;x<800/50;x++) {
	  pos_screen.x = x*50;
	  pos_screen.y = y*50;

	  if (tab[y][x] == -1) {
	    SDL_BlitSurface(sprite.block, &pos_sprite, sprite.screen, &pos_screen);
	  }
	}
      }


      SDL_BlitSurface(sprite.player, &player.rcSrc, sprite.screen, &player.position);

      //printf("%d ",on_the_floor(tab,player));
      //printf("%d ",distance_of_floor(tab,player));

      /* nb sprite x et y
	printf("%d %d\n",player.rcSrc.x/75,player.rcSrc.y/75);*/

      /* distance entre joueur et obstacle gauche 
	 printf("%d\n",distance_wall_left(tab,player));*/

      /* distance entre joueur et obstacle droite */
      //printf("%d\n",distance_wall_right(tab,player));

      /* pos case x et y
	 printf("%d %d\n", player.position.x/50,(player.position.y+(75/2))/50);*/
      
      /* GRAVITY */
      if (player.jump == 0)
	gravity(&player,tab);

      /****************************************************************************************************/
      /* OTHER */
      
      /* update the screen */
      SDL_UpdateRect(sprite.screen,0,0,0,0);

      /* fps */
      SDL_Delay(80);
    }

  /****************************************************************************************************/
  /* CLEAN */

  free_all_sprite(sprite);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
