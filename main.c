#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2\SDL.h>

typedef struct{
	double x;
	double y;
}vec2;

struct clock{
	double timeleft;
	struct clock * next;
};
typedef struct clock timer;
timer * timerhandle = NULL;

typedef struct{
	bool left;
	bool right;
	bool up;
	bool down;
}keys;

struct projectile{
	vec2 pos;
	vec2 acc;
	struct projectile * next;	
};
typedef struct projectile bullet;
bullet * bullethandle = NULL;

typedef struct{
	vec2 pos;
	vec2 acc;
}entity;

size_t screenwidth = 800;
size_t screenheight = 600;
double px = 400.0;
double py = 300.0;
double dt = 1.0;
bool init = true;
SDL_Window * window;
SDL_Renderer * renderer;
SDL_Event event;
SDL_Color color;

keys move;

void error(const char * errormessage){
	printf("OpenDanmaku Error: %s\n", errormessage);
}

void setdt(void){
	static size_t oldtime;
	dt = ((double)SDL_GetTicks() - (double)oldtime) / 1000.0;
	oldtime = SDL_GetTicks();
}

void addtimer(timer * newtimer){
	if(timerhandle == NULL){
		timerhandle = newtimer;
		timerhandle->next = timerhandle;
	}else{
		newtimer->next = timerhandle->next;
		timerhandle->next = newtimer;
	}
}

void updatetimers(void){
	if(timerhandle == NULL){
		return;
	}else{
		timer * temp = timerhandle;
		do{
			if(temp->timeleft > 0){
				temp->timeleft -= dt;
			}
			temp = temp->next;
		}while(temp != timerhandle);
	}
}

void addbullet(double x, double y, double sx, double sy){
	bullet * temp = bullethandle;
	bullethandle = (bullet *)malloc(sizeof(bullet));
	bullethandle->pos.x = x;
	bullethandle->pos.y = y;
	bullethandle->acc.x = sx;
	bullethandle->acc.y = sy;
	bullethandle->next = temp;
}

void deletebullet(bullet * remove){
	bullet * temp = remove->next;
	remove->pos = remove->next->pos;
	remove->acc = remove->next->acc;
	remove->next = remove->next->next;
	free(temp);
}

void updatebullets(void){
	if(bullethandle == NULL){
		return;
	}else{
		bullet * temp = bullethandle;
		do{
			temp->pos.x += temp->acc.x * dt;
			temp->pos.y += temp->acc.y * dt;
			rect((int)temp->pos.x, (int)temp->pos.y, 10, 10);
			temp = temp->next;
		}while(temp != NULL);
	}
}

bool checktimer(timer check){
	return (check.timeleft <= 0.0);
}

void clear(void){
	setcolor(255, 255, 255);
	SDL_RenderClear(renderer);
}

void flip(void){
	SDL_RenderPresent(renderer);
}

void setcolor(Uint8 r, Uint8 g, Uint8 b){
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = 255;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void setalpha(Uint8 a){
	color.a = a;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void rect(int x, int y, int w, int h){
	SDL_Rect temp = (SDL_Rect) { .x = x, .y = y, .w = w, .h = h};
	SDL_RenderFillRect(renderer, &temp);
}

void loop(void){
	
	static double acc;
	static entity pr;
	static timer bullettimer;
	
	if(init){
		pr.pos.x = (double)screenwidth / 2;
		pr.pos.y = (double)screenheight / 2;
		pr.acc.x = (double)screenwidth / 3.0;
		pr.acc.y = pr.acc.x;
		bullettimer.timeleft = .1;
		addtimer(&bullettimer);
		init = false;
	}
	
	clear();
	
	setcolor(255, 0, 0);
	updatebullets();
	
	if(move.left){
		pr.pos.x -= pr.acc.x * dt;
	}
	if(move.right){
		pr.pos.x += pr.acc.x * dt;
	}
	if(move.up){
		pr.pos.y -= pr.acc.y * dt;
	}
	if(move.down){
		pr.pos.y += pr.acc.y * dt;
	}
	
	if(checktimer(bullettimer)){
		addbullet(pr.pos.x, pr.pos.y, .0, -500.0);
		bullettimer.timeleft = .1;
	}
	rect(pr.pos.x, pr.pos.y, screenwidth/40, screenheight/20);
	
	flip();
}

int main(int argc, char ** argv){
	
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		error("COULD NOT INITIALIZE SDL");
		return 1;
	}
	
	window = SDL_CreateWindow("OpenDanmaku", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenwidth, screenheight, 0);
	if(window == NULL){
		error("Could not create SDL Window");
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL){
		error("Could not create SDL Renderer");
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	setdt();
	
	bool quit = false;
	while(!quit){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_WINDOWEVENT){
				switch(event.window.event){
					case SDL_WINDOWEVENT_CLOSE:
						quit = true;
						break;
				}
			}else if(event.type == SDL_KEYDOWN){
				switch(event.key.keysym.sym){
					case SDLK_LEFT:
					case SDLK_a:
						move.left = true;
						break;
					case SDLK_RIGHT:
					case SDLK_d:
						move.right = true;
						break;
					case SDLK_UP:
					case SDLK_w:
						move.up = true;
						break;
					case SDLK_DOWN:
					case SDLK_s:
						move.down = true;
						break;
				}
			}else if(event.type == SDL_KEYUP){
				switch(event.key.keysym.sym){
					case SDLK_LEFT:
					case SDLK_a:
						move.left = false;
						break;
					case SDLK_RIGHT:
					case SDLK_d:
						move.right = false;
						break;
					case SDLK_UP:
					case SDLK_w:
						move.up = false;
						break;
					case SDLK_DOWN:
					case SDLK_s:
						move.down = false;
						break;
				}
			}
		}
		setdt();
		updatetimers();
		loop();
	}
	return 0;
}


