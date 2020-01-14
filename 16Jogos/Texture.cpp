#include "Texture.h"

void Texture::load(SDL_Renderer* renderer, const char* path)
{
	SDL_Surface* tempSurface = IMG_Load(path);
	tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
}

void Texture::loadFont(SDL_Renderer* renderer, const char* path, int size,const char* text, SDL_Color color)
{
	TTF_Font* font = TTF_OpenFont(path, size);
	SDL_Surface* tempSurface = TTF_RenderText_Solid(font, text, color);
	tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	int texW = 0, texH = 0;
	getTexture(NULL, NULL, &texW, &texH);
	srcRect.w = destRect.w = texW;
	srcRect.h = destRect.h = texH;
}

void Texture::getTexture(Uint32* x, int* y, int* w, int* h)
{
	SDL_QueryTexture(tex, x, y, w, h);
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