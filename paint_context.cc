#include "paint_context.hh"

#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <string>

#include "paint_context_ex.hh"
#include "lcars_text.hh"

PaintContext::PaintContext(SDL_Renderer * renderer, SDL_Texture * buffer, SDL_Rect bounds) {
	m_renderer	= renderer;
	m_buffer	= buffer;
	m_bounds	= bounds;
	m_color		= {0, 0, 0, 255};
	m_font		= nullptr;

	m_texture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
			bounds.w, bounds.h);

	if(!m_texture) {
		std::cerr << "FATAL ERROR: " << SDL_GetError() << std::endl;
		exit(0);
	}

	SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
	m_flip = 0;
}

PaintContext::~PaintContext() {
	//PaintScreen();
	SDL_SetRenderTarget(m_renderer, m_buffer);
	SDL_DestroyTexture(m_texture);
}

text_t * PaintContext::PrepareText(int16_t x, int16_t y, std::string * str, SDL_Surface * surf) {

	SDL_SetRenderTarget(m_renderer, nullptr);
	SDL_Texture * tex	= SDL_CreateTextureFromSurface(m_renderer, surf);

	if(surf)
		SDL_FreeSurface(surf);

	if(!tex) {
		std::cerr << "ERROR: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	int h, w;
	TTF_SizeText(m_font, str->c_str(), &w, &h);

	SDL_Rect bounds = {
			x + m_bounds.x, y + m_bounds.y, w, h
	};

	text_t * text = (text_t *) malloc(sizeof(text_t));

	if(!text)
		std::cerr << "ERROR: Could not allocate Memory." << std::endl;

	text->tex_text	= tex;
	text->str		= str;
	text->bounds	= bounds;

	return text;
}

void PaintContext::PrepareRect(SDL_Rect * rect, SDL_Rect * rect_out) {
	rect_out->x = rect->x + m_bounds.x;
	rect_out->y = rect->y + m_bounds.y;
	rect_out->w = rect->w;
	rect_out->h = rect->h;
}

SDL_Rect const * PaintContext::GetOffset() {
	return &m_bounds;
}

void PaintContext::FlipVertically() {
	m_flip |= SDL_FLIP_VERTICAL;
}

void PaintContext::FlipHorizontally() {
	m_flip |= SDL_FLIP_HORIZONTAL;
}

void PaintContext::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	m_color.r = r;
	m_color.g = g;
	m_color.b = b;
	m_color.a = a;
}

void PaintContext::SetFont(TTF_Font * font) {
	m_font = font;
}

void PaintContext::FillRect(SDL_Rect * rect) {

	if(SDL_SetRenderTarget(m_renderer, m_texture) == -1)
		std::cerr << "ERROR H: " << SDL_GetError() << std::endl;

	if(SDL_SetRenderDrawColor(m_renderer, m_color.r, m_color.g, m_color.b, m_color.a) == -1)
		std::cerr << "ERROR: " << SDL_GetError() << std::endl;

	if(SDL_RenderFillRect(m_renderer, rect) == -1)
		std::cerr << "ERROR: " << SDL_GetError() << std::endl;

	if(SDL_SetRenderTarget(m_renderer, m_buffer) == -1)
		std::cerr << "ERROR: " << SDL_GetError() << std::endl;
}

void PaintContext::DrawRect(SDL_Rect * rect) {
	SDL_SetRenderTarget(m_renderer, m_texture);
	rectangleRGBA(m_renderer, rect->x, rect->y, rect->x+rect->w, rect->y+rect->h, m_color.r, m_color.g, m_color.b, m_color.a);
	SDL_SetRenderTarget(m_renderer, m_buffer);
}

void PaintContext::FillCircle(int16_t x, int16_t y, int16_t rad) {
	SDL_SetRenderTarget(m_renderer, m_texture);
	aaFilledEllipseRGBA(m_renderer, x, y, rad, rad, m_color.r, m_color.g, m_color.b, m_color.a);
	SDL_SetRenderTarget(m_renderer, m_buffer);
}

void PaintContext::DrawCircle(int16_t x, int16_t y, int16_t rad) {
	SDL_SetRenderTarget(m_renderer, m_texture);
	aacircleRGBA(m_renderer, x, y, rad, m_color.r, m_color.g, m_color.b, m_color.a);
	SDL_SetRenderTarget(m_renderer, m_buffer);
}

void PaintContext::FillEllipse(int16_t x, int16_t y, int16_t rx, int16_t ry) {
	SDL_SetRenderTarget(m_renderer, m_texture);
	aaFilledEllipseRGBA(m_renderer, x, y, rx, ry, m_color.r, m_color.g, m_color.b, m_color.a);
	SDL_SetRenderTarget(m_renderer, m_buffer);
}

void PaintContext::DrawEllipse(int16_t x, int16_t y, int16_t rx, int16_t ry) {
	SDL_SetRenderTarget(m_renderer, m_texture);
	aaellipseRGBA(m_renderer, x, y, rx, ry, m_color.r, m_color.g, m_color.b, m_color.a);
	SDL_SetRenderTarget(m_renderer, m_buffer);
}

void PaintContext::DrawImage(int x, int y, int w, int h, SDL_Surface * img) {
	SDL_SetRenderTarget(m_renderer, m_texture);

	SDL_Texture * tex = SDL_CreateTextureFromSurface(m_renderer, img);

	SDL_Rect src = {0, 0, img->w, img->h};
	SDL_Rect dst = {x, y, w, h};

	SDL_RenderCopy(m_renderer, tex, &src, &dst);

	SDL_DestroyTexture(tex);
	SDL_SetRenderTarget(m_renderer, m_buffer);
}

text_t * PaintContext::PrepareBlendedText(int16_t x, int16_t y, std::string * str) {
	SDL_Surface * surf	= TTF_RenderText_Blended(m_font, str->c_str(), m_color);

	if(!surf)
		std::cerr << "ERROR: " << SDL_GetError() << std::endl;

	return PrepareText(x, y, str, surf);
}

text_t * PaintContext::PrepareSolidText(int16_t x, int16_t y, std::string * str) {
	SDL_Surface * surf	= TTF_RenderText_Solid(m_font, str->c_str(), m_color);

	if(!surf)
		std::cerr << "ERROR: " << SDL_GetError() << std::endl;

	return PrepareText(x, y, str, surf);
}

void PaintContext::PrepareText(LCARS_Text * text) {
	text->Regenerate(m_renderer);
}

void PaintContext::DestroyText(text_t * text) {
	if(text->tex_text)
		SDL_DestroyTexture(text->tex_text);
	free(text);
}

void PaintContext::DrawText(text_t * text, SDL_Rect * src, SDL_Rect * dst) {
	SDL_Rect dst_modded = {dst->x, dst->y, dst->w, dst->h};

	SDL_SetRenderTarget(m_renderer, m_texture);
	if(SDL_RenderCopy(m_renderer, text->tex_text, src, &dst_modded) == -1)
		std::cerr << "ERROR I: " << SDL_GetError() << std::endl;

	SDL_SetRenderTarget(m_renderer, m_buffer);
}

void PaintContext::DrawText(int x, int y, LCARS_Text * text) {

	SDL_Rect text_bounds = text->GetBounds();

	SDL_Rect modded_dst = {
		x, y, text_bounds.w, text_bounds.h
	};

	SDL_Rect modded_src = {
		0, 0, text_bounds.w, text_bounds.h
	};

	SDL_SetRenderTarget(m_renderer, m_texture);
	if(SDL_RenderCopy(m_renderer, text->GetTexture(), &modded_src, &modded_dst) == -1)
		std::cerr << "ERROR I: " << SDL_GetError() << std::endl;

	SDL_SetRenderTarget(m_renderer, m_buffer);
}

void PaintContext::PaintScreen() {
	SDL_SetRenderTarget(m_renderer, m_buffer);

	SDL_Rect src = {0, 0, m_bounds.w, m_bounds.h};
	SDL_Rect dst = {m_bounds.x, m_bounds.y, m_bounds.w, m_bounds.h};

	SDL_RenderCopyEx(m_renderer, m_texture, &src, &dst, 0, NULL, (SDL_RendererFlip) m_flip);
}

SDL_Texture * PaintContext::GetTexture() {
	return m_texture;
}
