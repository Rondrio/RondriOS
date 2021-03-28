#include "lcars_text.hh"

#include <iostream>
#include <string>
#include <cstdint>
#include <SDL2/SDL.h>

#include "lcars_error.hh"

LCARS_Font::LCARS_Font(std::string font_path, uint16_t ptsize) {
	m_font_path	= font_path;
	m_ptsize	= ptsize;
	m_font		= nullptr;

	Regenerate();
}

LCARS_Font::~LCARS_Font() {
	if(m_font)
		TTF_CloseFont(m_font);
}

int LCARS_Font::Regenerate() {

	if(m_font)
		TTF_CloseFont(m_font);

	TTF_Font * font = TTF_OpenFont(m_font_path.c_str(), m_ptsize);

	if(!font) {
		LOGEX("ERROR in LCARS_Font::Regenerate()", "Could not open Font", TTF_GetError());
		return 0;
	}

	m_font = font;
	return 1;
}

void LCARS_Font::SetPointSize(uint16_t ptsize) {
	m_ptsize = ptsize;
	Regenerate();
}

uint16_t LCARS_Font::GetPointSize() {
	return m_ptsize;
}

void LCARS_Font::SetFont(std::string font_path) {
	m_font_path = font_path;
	Regenerate();
}

TTF_Font * LCARS_Font::GetFont() {
	return m_font;
}

void LCARS_Font::QuerySize(std::string text, int * w, int * h) {
	TTF_SizeText(m_font, text.c_str(), w, h);
}
// -------------------- LCARS_TEXT --------------------------

LCARS_Text::LCARS_Text(std::string text, std::string font_path, uint16_t ptsize) {
	m_text					= text;
	m_font					= new LCARS_Font(font_path, ptsize);
	m_color					= {255, 255, 255, 255};

	m_texture				= nullptr;
	m_needs_regeneration	= true;
}

LCARS_Text::~LCARS_Text() {
	if(m_texture)
		SDL_DestroyTexture(m_texture);
	delete m_font;
}

void LCARS_Text::Regenerate(SDL_Renderer * renderer) {
	if(m_needs_regeneration) {
		
		if(m_texture)
			SDL_DestroyTexture(m_texture);

		SDL_Surface * surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_color);

		if(!surf) {
			LOGEX("ERROR in LCARS_Text::Regenerate()", "Could not generate Text", TTF_GetError());
			return;

		} else {
			SDL_SetRenderTarget(renderer, nullptr);
			m_texture = SDL_CreateTextureFromSurface(renderer, surf);

			if(!m_texture) {
				LOGEX("ERROR in LCARS_Text::Regenerate()", "Could not create Texture", SDL_GetError());
				return;
			}

			SDL_FreeSurface(surf);
		}

		m_needs_regeneration = false;
	}
}

void LCARS_Text::SetText(std::string text) {
	m_text = text;
	m_needs_regeneration = true;
}

std::string LCARS_Text::GetText() {
	return m_text;
}

void LCARS_Text::InsertText(std::string text, uint16_t position) {
	std::string pre		= m_text.substr(0, position);
	std::string post	= m_text.substr(position);

	pre   += text;
	m_text = pre + post;
	m_needs_regeneration = true;
}

void LCARS_Text::Append(std::string text) {
	m_text += text;
	m_needs_regeneration = true;
}

void LCARS_Text::PopBack(uint16_t count) {
	for(int i = 0; i < count; ++i)
		m_text.pop_back();
	m_needs_regeneration = true;
}

void LCARS_Text::PopAt(uint16_t position, uint16_t count) {
	std::string pre		= m_text.substr(0, position);
	std::string post	= m_text.substr(position);

	for(int i = 0; i < count; ++i)
		if(pre.size() > 0)
			pre.pop_back();

	m_text = pre + post;
	m_needs_regeneration = true;
}

void LCARS_Text::SetFont(std::string path) {
	m_font->SetFont(path);
	m_needs_regeneration = true;
}

void LCARS_Text::SetFontSize(uint16_t ptsize) {
	m_font->SetPointSize(ptsize);
	m_needs_regeneration = true;
}

void LCARS_Text::SetColor(SDL_Color color) {
	m_color = color;
	m_needs_regeneration = true;
}

SDL_Texture * LCARS_Text::GetTexture() {
	return m_texture;
}

SDL_Rect LCARS_Text::GetBounds() {
	int w, h;
	m_font->QuerySize(m_text, &w, &h);
	return {0, 0, w, h};
}