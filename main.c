#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2\SDL.h>

SDL_Window * window;
SDL_Renderer * renderer;
SDL_Event event;
SDL_Color color;

void error(const char * errormessage){
	printf("OpenDanmaku Error: %s\n", errormessage);
	return;
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
	setcolor(255, 0, 0);
	rect(100, 100, 400, 400);
	flip();
}

int main(int argc, char ** argv){
	
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		error("COULD NOT INITIALIZE SDL");
		return 1;
	}
	
	window = SDL_CreateWindow("OpenDanmaku", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	bool quit = false;
	while(!quit){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_WINDOWEVENT){
				switch(event.window.event){
					case SDL_WINDOWEVENT_CLOSE:
						quit = true;
						break;
					default:
						break;
				}
			}
		}
		loop();
	}
	
	return 0;
}


