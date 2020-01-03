#include "Texture.h"

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

void Texture::load(SDL_Renderer* renderer, const char* path)
{
	SDL_Surface* tempSurface = IMG_Load(path);
	tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
}

void Texture::draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, tex, &srcRect, &destRect);
}

void Texture::draw(SDL_Renderer* renderer, double angle)
{
	SDL_RenderCopyEx(renderer, tex, &srcRect, &destRect, angle, NULL, SDL_FLIP_NONE);
}

void Texture::flipH(SDL_Renderer* renderer)
{
	SDL_RenderCopyEx(renderer, tex, &srcRect, &destRect, 0, NULL, SDL_FLIP_HORIZONTAL);
}

void Texture::flipV(SDL_Renderer* renderer)
{
	SDL_RenderCopyEx(renderer, tex, &srcRect, &destRect, 0, NULL, SDL_FLIP_VERTICAL);
}

void Texture::setColor(int r, int g, int b)
{
	SDL_SetTextureColorMod(tex, r, g, b);
}

void Texture::setAlpha(int alpha)
{
	SDL_SetTextureAlphaMod(tex, alpha);
}