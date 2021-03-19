#include "paint_context.hh"

#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

PaintContext::PaintContext(SDL_Renderer * renderer, SDL_Rect bounds) {
	m_renderer	= renderer;
	m_bounds	= bounds;
	m_color		= {0, 0, 0, 255};
	m_font		= nullptr;
}

PaintContext::~PaintContext() {

}

text_t * PaintContext::PrepareText(int16_t x, int16_t y, char * str, SDL_Surface * surf) {

	SDL_Texture * tex	= SDL_CreateTextureFromSurface(m_renderer, surf);

	if(surf)
		SDL_FreeSurface(surf);

	if(!tex)
		return nullptr;

	int h, w;
	TTF_SizeText(m_font, str, &w, &h);

	SDL_Rect bounds = {
			x + m_bounds.x, y + m_bounds.y, w, h
	};

	text_t * text = (text_t *) malloc(sizeof(text_t));

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
	SDL_Rect r;
	PrepareRect(rect, &r);

	SDL_SetRenderDrawColor(m_renderer, m_color.r, m_color.g, m_color.b, m_color.a);
	SDL_RenderFillRect(m_renderer, &r);
}

void PaintContext::DrawRect(SDL_Rect * rect) {
	SDL_Rect r;
	PrepareRect(rect, &r);

	rectangleRGBA(m_renderer, r.x, r.y, r.x+r.w, r.y+r.h, m_color.r, m_color.g, m_color.b, m_color.a);

	//SDL_SetRenderDrawColor(m_renderer, m_color.r, m_color.g, m_color.b, m_color.a);
	//SDL_RenderDrawRect(m_renderer, &r);
}

void PaintContext::FillCircle(int16_t x, int16_t y, int16_t rad) {

	x += m_bounds.x;
	y += m_bounds.y;

	filledCircleRGBA(m_renderer, x, y, rad, m_color.r, m_color.g, m_color.b, m_color.a);
}

void PaintContext::DrawCircle(int16_t x, int16_t y, int16_t rad) {

	x += m_bounds.x;
	y += m_bounds.y;

	aacircleRGBA(m_renderer, x, y, rad, m_color.r, m_color.g, m_color.b, m_color.a);
}

void PaintContext::FillEllipse(int16_t x, int16_t y, int16_t rx, int16_t ry) {

	x += m_bounds.x;
	y += m_bounds.y;

	filledEllipseRGBA(m_renderer, x, y, rx, ry, m_color.r, m_color.g, m_color.b, m_color.a);
}

void PaintContext::DrawEllipse(int16_t x, int16_t y, int16_t rx, int16_t ry) {

	x += m_bounds.x;
	y += m_bounds.y;

	aaellipseRGBA(m_renderer, x, y, rx, ry, m_color.r, m_color.g, m_color.b, m_color.a);
}

text_t * PaintContext::PrepareBlendedText(int16_t x, int16_t y, char * str) {
	SDL_Surface * surf	= TTF_RenderText_Blended(m_font, str, m_color);
	return PrepareText(x, y, str, surf);
}

text_t * PaintContext::PrepareSolidText(int16_t x, int16_t y, char * str) {
	SDL_Surface * surf	= TTF_RenderText_Solid(m_font, str, m_color);
	return PrepareText(x, y, str, surf);
}

void PaintContext::DestroyText(text_t * text) {
	if(text->tex_text)
		SDL_DestroyTexture(text->tex_text);
}

void PaintContext::DrawText(text_t * text, SDL_Rect * src, SDL_Rect * dst) {
	//SDL_Rect src = {0, 0, (&text->bounds)->w, (&text->bounds)->h};
	//SDL_RenderCopy(m_renderer, text->tex_text, &src, &(text->bounds));

	SDL_Rect dst_modded = {m_bounds.x + dst->x, m_bounds.y + dst->y, dst->w, dst->h};
	SDL_RenderCopy(m_renderer, text->tex_text, src, &dst_modded);
}
