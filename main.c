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

struct thing{
	vec2 pos;
	vec2 acc;
	struct thing * next;	
};

typedef struct thing bullet;

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
	dt = ((double)SDL_GetTicks() - (double)oldtime) / 1000;
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
	clear();
	static double xacc;
	static double yacc;
	if(init){
		xacc = (double)screenwidth / 3.0;
		yacc = (double)screenheight / 3.0;
	}
	
	setcolor(255, 0, 0);
	
	if(move.left){
		px -= xacc * dt;
	}
	if(move.right){
		px += xacc * dt;
	}
	if(move.up){
		py -= yacc * dt;
	}
	if(move.down){
		py += yacc * dt;
	}
	
	rect(px, py, screenwidth/40, screenheight/20);
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
		init = false;
	}
	return 0;
}


