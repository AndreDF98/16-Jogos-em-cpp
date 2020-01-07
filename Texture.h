#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>

class Texture
{

public:
	void load(SDL_Renderer* renderer, const char* path);
	void loadFont(SDL_Renderer* renderer,const char* path, int size, const char* text, SDL_Color color);
	void getTexture(Uint32* x, int* y, int* w, int* h);
	void draw(SDL_Renderer* renderer);
	void draw(SDL_Renderer* renderer, double angle);
	void flipH(SDL_Renderer* renderer);
	void flipV(SDL_Renderer* renderer);
	void setColor(int r, int g, int b);
	void setAlpha(int alpha);

	SDL_Rect srcRect, destRect;

private:
	SDL_Texture* tex;

};

