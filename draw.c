/******************************************************************/
/* draw.c                                                         */
/* Victor DARMOIS Loic MOLINA Quentin MORIZOT                     */
/* Creation: 21/10/15                                             */
/* Last modification: 17/11/15                                    */
/******************************************************************/

#include "constant.h"
#include "game.h"
#include "draw.h"
#include "physic.h"


/****************************************************************************************************/
/* DRAW */

void draw(int x_max, int y_max, int tab[y_max][x_max], s_surface sprite, s_information player)
{
  int x, y, a, b, map_x, modulo;
  SDL_Rect pos_sprite, pos_screen;

  pos_sprite.x = 0;
  pos_sprite.y = 0;
  pos_sprite.w = 50;
  pos_sprite.h = 50;
  modulo = 0;

  if (player.movement > 800/2 && player.movement < x_max*50 - 800/2) {
    // camera bouge: début d'affichage selon la pos du player
    map_x = player.movement/50-(800/50)/2;
    modulo = player.movement%50;
  } else if (player.movement <= 800/2) {
    map_x = 0;
  } else {
    map_x = x_max-800/50;
  }

  a = 0;
  for (y=0 ; y<y_max ; y++) {
    b = 0;
    for (x=map_x ; x<=map_x+800/50 ; x++) {
      pos_screen.x = b*50-modulo;
      pos_screen.y = a*50;
      draw_element(x_max,y_max,tab,x,y,sprite,pos_sprite,pos_screen);
      b++;
    }
    a++;
  }
}

void draw_player(s_information player,s_surface sprite,s_time* time_p,int map)
{
  
  // tampon car BlitSurface remet a 0 si nega
  SDL_Rect position = player.position;
  if(player.life>=0)
    {
  if (time_p->current-time_p->previous_time_hit<2500 && map!=0 && player.life>0){
    if (time_p->current%2==0)
      SDL_BlitSurface(sprite.player, &player.rcSrc, sprite.screen, &position);
  }else{
    SDL_BlitSurface(sprite.player, &player.rcSrc, sprite.screen, &position);
  }
  }
}

void draw_element(int x_max, int y_max, int tab[y_max][x_max], int x, int y, s_surface sprite, SDL_Rect pos_sprite, SDL_Rect pos_screen) 
{
  if (tab[y][x] == 0) {
    SDL_BlitSurface(sprite.background, &pos_sprite, sprite.screen, &pos_screen);
  }
  
  if (tab[y][x] == -1) {
    SDL_BlitSurface(sprite.block, &pos_sprite, sprite.screen, &pos_screen);
  } 

  if (tab[y][x] == 1) {
    SDL_BlitSurface(sprite.platform, &pos_sprite, sprite.screen, &pos_screen);
  } 

  if (tab[y][x] == 2) {
    SDL_BlitSurface(sprite.locked, &pos_sprite, sprite.screen, &pos_screen);
  } 

  if (tab[y][x] == 3) {
    SDL_BlitSurface(sprite.download, &pos_sprite, sprite.screen, &pos_screen);
  } 
  if (tab[y][x] == 4) {
    SDL_BlitSurface(sprite.unlocked, &pos_sprite, sprite.screen, &pos_screen);
  } 
}

list_ptr anim_ennemis(list_ptr ennemis,s_information player,int x_max,int y_max,int tab[y_max][x_max])
{
  list_ptr copy_ennemi=ennemis;
  if (copy_ennemi!=NULL)
    while (copy_ennemi!=NULL){
      int limit;
      copy_ennemi->info.rcSrc.y=0;
      if (copy_ennemi->info.life>0){
	if (player.position.y==copy_ennemi->info.position.y)
	  limit=copy_ennemi->info.limit;
	else
	  limit=0;
	if (copy_ennemi->info.state==0){
	  if (copy_ennemi->info.rcSrc.x>=2*75 && copy_ennemi->info.rcSrc.x<10*75 )
	    copy_ennemi->info.rcSrc.x+=75;
	  else
	    copy_ennemi->info.rcSrc.x=2*75;
	}else{
	  if (copy_ennemi->info.rcSrc.x>=12*75 && copy_ennemi->info.rcSrc.x<20*75 )
	    copy_ennemi->info.rcSrc.x+=75;
	  else
	    copy_ennemi->info.rcSrc.x=12*75;
	}
	if (player.movement>=copy_ennemi->info.movement &&  player.movement-limit<=copy_ennemi->info.movement ){
	  copy_ennemi->info.rcSrc.x=0;
	  if (player.position.y==copy_ennemi->info.position.y)
	    copy_ennemi->info.rcSrc.y=75;
	}
	if (player.movement<=copy_ennemi->info.movement &&  player.movement+limit>=copy_ennemi->info.movement){
	  copy_ennemi->info.rcSrc.x=11*75;
	  if (player.position.y==copy_ennemi->info.position.y)
	    copy_ennemi->info.rcSrc.y=75;
	}
      }
      copy_ennemi=copy_ennemi->next;
    }
  return ennemis;

}

s_information death(s_information player)
{
  if (player.life==0)
    {
      player.rcSrc.y=3*75;
      if (player.dying==0){
	if (player.state==0){
	  player.rcSrc.x=0;
	}else{
	  player.rcSrc.x=11*75;
	}
	player.dying=1;
      }
      if ((player.state==0 && player.rcSrc.x<4*75) ||  (player.state==1 && player.rcSrc.x<15*75) ){
	player.rcSrc.x+=75;
      }else{
	player.life=-1;
      }
        
    }
  return player;
}

list_ptr ennemis_death(list_ptr ennemis)
{
  list_ptr copy_ennemis=ennemis;
  int a =0;
  while (copy_ennemis!=NULL)
    {
      if (copy_ennemis->info.life==0){
	copy_ennemis->info.rcSrc.y=75;
	if (copy_ennemis->info.dying==0){
	  if (copy_ennemis->info.state==0){
	    copy_ennemis->info.rcSrc.x=3*75;
	  }else{
	    copy_ennemis->info.rcSrc.x=15*75;
	  }
	}
	copy_ennemis->info.dying=1;
	if (copy_ennemis->info.rcSrc.x<6*75 && copy_ennemis->info.state==0){
	  copy_ennemis->info.rcSrc.x+=75;
	}else{
	  if (copy_ennemis->info.rcSrc.x<17*75 && copy_ennemis->info.state==1){
	    copy_ennemis->info.rcSrc.x+=75;
	  }else{
	    copy_ennemis->info.life=-1;
	  }
	}	  
      }      
      copy_ennemis=copy_ennemis->next;
    }

  return ennemis;
}

void draw_shooting(s_information player, list_ptr shots, s_surface sprite)
{
  list_ptr shots_copy=shots;
  while (shots_copy!=NULL)
    {
      if (shots_copy->info.state==0){
	shots_copy->info.position.x+=24;
	shots_copy->info.movement+=24;
      
      } else{ 
	shots_copy->info.position.x-=24;
	shots_copy->info.movement-=24;
      }
      SDL_BlitSurface(sprite.bullet,&shots_copy->info.rcSrc,sprite.screen,&shots_copy->info.position);
      shots_copy=shots_copy->next;
    }
}

void draw_ennemis_shooting(list_ptr army_shots,s_surface sprite, s_information player,int x_max )
{
  list_ptr copy_shots=army_shots;
  SDL_Rect pos;
  while (copy_shots!=NULL){
    if (copy_shots->info.state==0){
      copy_shots->info.movement+=24;
    }else{
      copy_shots->info.movement-=24;
    }
    
    if (copy_shots->info.movement<=0 || copy_shots->info.movement>x_max*50)
      copy_shots->info.life=-1;
    pos.y=copy_shots->info.position.y;
    pos.x=copy_shots->info.movement-20-(player.movement-13-player.position.x);
    SDL_BlitSurface(sprite.bullet,&copy_shots->info.rcSrc,sprite.screen,&pos);
    copy_shots=copy_shots->next;
  }
}

void draw_ammo(s_surface sprite,int ammo, int map, s_information player)
{ 
  if (map!=0 && player.life>0){
    SDL_Rect rcSrc_set,rcSrc;
    SDL_Rect position_set,position;

    position_set.x=760;
    position_set.y=10;
    position.x=764;
    position.y=14+(60-ammo);
    rcSrc_set.w=14;
    rcSrc_set.h=80;
    rcSrc_set.x=0;
    rcSrc_set.y=0;
    rcSrc.w=8;
    rcSrc.h=ammo;
    rcSrc.x=0;
    rcSrc.y=0;
    SDL_BlitSurface(sprite.ammo_set, &rcSrc_set, sprite.screen, &position_set);
    SDL_BlitSurface(sprite.ammo, &rcSrc, sprite.screen, &position);
  }
}

void draw_health(s_information player,s_surface sprite,int map)
{ 
  if (map!=0 && player.life>0){
    SDL_Rect rcSrc;
    SDL_Rect position;
    rcSrc.w=200;
    rcSrc.h=50;
    rcSrc.y=0;
    rcSrc.x=1000-(player.life*200);
    position.x=5;
    position.y = 10;
    SDL_BlitSurface(sprite.health, &rcSrc, sprite.screen, &position);
  }
}

void draw_ennemis(list_ptr ennemi_ptr, s_surface sprite, s_information player)
{
  list_ptr ennemi_list = ennemi_ptr;
  SDL_Rect position;

  while (ennemi_list != NULL) {
    position.y = ennemi_list->info.position.y;
    position.x = ennemi_list->info.movement-20-(player.movement-13-player.position.x);

    SDL_BlitSurface(sprite.ennemi,&ennemi_list->info.rcSrc,sprite.screen,&position);
    ennemi_list = ennemi_list->next;
  }
}

void draw_tab(int x_max, int y_max, int tab[y_max][x_max]) 
{
  int x,y;

  for (y=0;y<y_max;y++) {
    for (x=0;x<x_max;x++) {
      if (tab[y][x] >= 0) {
	printf(" %d ",tab[y][x]);
      } else {
	printf("%d ",tab[y][x]);
      }
    }
    printf("\n");
  }
}

void draw_chrono(SDL_Surface *screen, TTF_Font *font, s_time time, int map) 
{
  char sentence[20] = "";
  SDL_Rect position;
  SDL_Color black_color = {0,0,0,0};
  SDL_Surface *text;

  if (map >= 2) { 
    if (time.chrono < 0) {
      time.chrono = 0;
    }
    
    sprintf(sentence, "Time: %d.%d",time.chrono/1000,(time.chrono%1000)/10);
    
    text = TTF_RenderText_Blended(font, sentence, black_color);
    position.x = 400-text->w/2;
    position.y = 35;
    SDL_BlitSurface(text, NULL, screen, &position);
    SDL_FreeSurface(text);
  }
}

void draw_level(SDL_Surface *screen, TTF_Font *font, int level, int map, int score) 
{
  char sentence[20] = "";
  SDL_Rect position;
  SDL_Color black_color = {0,0,0,0};
  SDL_Surface *text;

  if (map >= 2) { 
    sprintf(sentence, "Level: %d    Score: %d",level,score);
  
    text = TTF_RenderText_Blended(font, sentence, black_color);
    position.x = 400-text->w/2;
    position.y = 10;
    SDL_BlitSurface(text, NULL, screen, &position);
    SDL_FreeSurface(text);
  }
}

void draw_menu(s_surface sprite, s_time time) 
{
  SDL_Color black_color = {255,255,255,0};
  SDL_Surface *text;
  TTF_Font *test = TTF_OpenFont("pixelmix.ttf", 46);
  SDL_Rect position, pos_sprite, pos_screen;
  int i, j;

  pos_sprite.x = 0;
  pos_sprite.y = 0;
  pos_sprite.w = 50;
  pos_sprite.h = 50;

  text = TTF_RenderText_Blended(test, "Accueil", black_color);
  position.x = 800/2 - text->w/2;
  position.y = 400/5-35 - text->h/2;
  SDL_BlitSurface(text, NULL, sprite.screen, &position);

  text = TTF_RenderText_Blended(test, "Jouer", black_color);
  position.x = 800/2 - text->w/2;
  position.y = 400/5+400/5*1-10 - text->h/2;
  SDL_BlitSurface(text, NULL, sprite.screen, &position);

  text = TTF_RenderText_Blended(test, "Classement", black_color);
  position.x = 800/2 - text->w/2;
  position.y = 400/5+400/5*2-10 - text->h/2;
  SDL_BlitSurface(text, NULL, sprite.screen, &position);

  text = TTF_RenderText_Blended(test, "Quitter", black_color);
  position.x = 800/2 - text->w/2;
  position.y = 400/5+400/5*3-10 - text->h/2;
  SDL_BlitSurface(text, NULL, sprite.screen, &position);

  TTF_CloseFont(test);
}

void draw_outline(s_surface sprite, int choice)
{
  SDL_Rect pos_sprite, pos_screen;

  pos_sprite.x = 0;
  pos_sprite.y = 0;
  pos_sprite.w = 350;
  pos_sprite.h = 50;

  pos_screen.x = 220;
  pos_screen.y = 400/5+400/5*(choice+1)-10 - 22;
  SDL_BlitSurface(sprite.outline, &pos_sprite, sprite.screen, &pos_screen);
}

/****************************************************************************************************/
/* ANIM */

s_information anim_right(int x_max, int y_max, int tab[y_max][x_max], s_information player, int automatic) 
{
  Uint8 *keystate = SDL_GetKeyState(NULL);

  if ((keystate[SDLK_RIGHT] && !keystate[SDLK_LEFT]) || automatic) {
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

s_information anim_left(int x_max, int y_max, int tab[y_max][x_max], s_information player, int automatic) 
{
  Uint8 *keystate = SDL_GetKeyState(NULL);

  if ((keystate[SDLK_LEFT] && !keystate[SDLK_RIGHT]) || automatic){
    /* AU SOL */
    if (distance_of_floor(x_max,y_max,tab,player) == 0) {
      /* direction droite ou au bout des sprites */
      player.rcSrc.y=0;
      if (player.rcSrc.x<12*75 || player.rcSrc.x>=21*75) {
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
  Uint8 *keystate= SDL_GetKeyState(NULL);
  /* JUMP */ 
  /* si EN SAUT  */
  if (player.jump > 0) {
    //deuxieme ligne de sprite 
    
    if (!keystate[SDLK_SPACE]){
      player.rcSrc.y=75;
      if (player.state==0) {
	player.rcSrc.x=75;
      } else {
	player.rcSrc.x=3*75;
      }
    }
  }
  /* si PAS DE SAUT mais PAS AU SOL */
  if (player.jump == 0 && distance_of_floor(x_max,y_max,tab,player) != 0) {
    if (!keystate[SDLK_SPACE]){
      player.rcSrc.y = 75;
      if (player.state == 0) {
	player.rcSrc.x = 75*2;
      } else {
	player.rcSrc.x = 75*5;
      }   
    }
  }
  return player;
}

s_information anim_shoot(int x_max, int y_max, int tab[y_max][x_max], s_information player)
{
  Uint8 *keystate = SDL_GetKeyState(NULL);
  
  if (keystate[SDLK_SPACE]) {
    player.rcSrc.y=75*2;
    if (distance_of_floor(x_max,y_max,tab,player) !=0) {
      if (player.state==0) {
	player.rcSrc.x=22*75;
      } else {
	player.rcSrc.x=23*75;
      }
    }
  }

  return player;
}

