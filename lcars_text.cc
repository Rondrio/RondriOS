#include "lcars_text.hh"

#include <iostream>
#include <string>
#include <cstdint>
#include <SDL2/SDL.h>

#include "lcars_error.hh"

LCARS::Font::Font(std::string font_path, uint16_t ptsize) {
	m_font_path	= font_path;
	m_ptsize	= ptsize;
	m_font		= nullptr;

	Regenerate();
}

LCARS::Font::~Font() {
	if(m_font)
		TTF_CloseFont(m_font);
}

int LCARS::Font::Regenerate() {

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

void LCARS::Font::SetPointSize(uint16_t ptsize) {
	m_ptsize = ptsize;
	Regenerate();
}

uint16_t LCARS::Font::GetPointSize() {
	return m_ptsize;
}

void LCARS::Font::SetFont(std::string font_path) {
	m_font_path = font_path;
	Regenerate();
}

TTF_Font * LCARS::Font::GetFont() {
	return m_font;
}

void LCARS::Font::QuerySize(std::string text, int * w, int * h) {
	TTF_SizeText(m_font, text.c_str(), w, h);
}
// -------------------- LCARS_TEXT --------------------------

LCARS::Text::Text(std::string text, std::string font_path, uint16_t ptsize) : Text(text, ptsize) {
	m_font = new Font(font_path, ptsize);
}

LCARS::Text::Text(std::string text, Font * font) : Text(text, font->GetPointSize()) {
	m_font = font;
}

LCARS::Text::Text(std::string text, uint16_t ptsize) {
	m_text					= text;
	m_color					= {255, 255, 255, 255};
	m_texture				= nullptr;
	m_surface				= nullptr;
	m_needs_regeneration	= true;
}

LCARS::Text::~Text() {
	if(m_texture)
		SDL_DestroyTexture(m_texture);
	delete m_font;
}

void LCARS::Text::Regenerate(SDL_Renderer * renderer) {
	if(m_needs_regeneration) {
	
		if(!m_font) {
			LOG("LCARS::Text::Regenerate()", "Could not generate Text, as there is no LCARS::Font attached to this text.");
			
			m_needs_regeneration = false;
			return;
		}

		/* Delete active Surface to make way for the new Surface. */
		if(m_surface) {
			SDL_FreeSurface(m_surface);
			m_surface = nullptr;
		}

		/* Make new Surface. */
		m_surface = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_color);
		
		/* If the Surface couldn't be created, print error message. */
		if(!m_surface) {
			LOGEX("ERROR in LCARS_Text::Regenerate()", "Could not generate Text", TTF_GetError());
			
			m_needs_regeneration = false;
			return;
		}

		/* Delete active Texture to make way for the new Texture. */
		if(m_texture) {
			SDL_DestroyTexture(m_texture);
			m_texture = nullptr;
		}
		
		/* Create new Texture. */
		m_texture = SDL_CreateTextureFromSurface(renderer, m_surface);

		/* If the Texture couldn't be created, print error message. */
		if(!m_texture)
			LOGEX("ERROR in LCARS_Text::Regenerate()", "Could not create Texture", SDL_GetError());

		m_needs_regeneration = false;
	}
}

void LCARS::Text::SetText(std::string text) {
	m_text = text;
	m_needs_regeneration = true;
}

std::string LCARS::Text::GetText() {
	return m_text;
}

void LCARS::Text::InsertText(std::string text, uint16_t position) {
	std::string pre		= m_text.substr(0, position);
	std::string post	= m_text.substr(position);

	pre   += text;
	m_text = pre + post;
	m_needs_regeneration = true;
}

void LCARS::Text::Append(std::string text) {
	m_text += text;
	m_needs_regeneration = true;
}

void LCARS::Text::PopBack(uint16_t count) {
	for(int i = 0; i < count; ++i)
		m_text.pop_back();
	m_needs_regeneration = true;
}

void LCARS::Text::PopAt(uint16_t position, uint16_t count) {
	std::string pre		= m_text.substr(0, position);
	std::string post	= m_text.substr(position);

	for(int i = 0; i < count; ++i)
		if(pre.size() > 0)
			pre.pop_back();

	m_text = pre + post;
	m_needs_regeneration = true;
}

void LCARS::Text::SetFont(std::string path) {
	m_font->SetFont(path);
	m_needs_regeneration = true;
}

void LCARS::Text::SetFont(LCARS::Font * font) {
	m_font = font;
	m_needs_regeneration;
}

void LCARS::Text::SetFontSize(uint16_t ptsize) {
	m_font->SetPointSize(ptsize);
	m_needs_regeneration = true;
}

void LCARS::Text::SetColor(SDL_Color color) {
	if(m_color.r != color.r || m_color.g != color.g || m_color.b != color.b || m_color.a != color.a) {
		m_color = color;
		m_needs_regeneration = true;
	}
}

int LCARS::Text::Length() {
	return m_text.size();
}

SDL_Texture * LCARS::Text::GetTexture() {
	return m_texture;
}

SDL_Rect LCARS::Text::GetBounds() {
	int w, h;
	m_font->QuerySize(m_text, &w, &h);
	return {0, 0, w, h};
}

LCARS::Text& LCARS::Text::operator+(std::string& str) {
	Append(str);
	return *this;
}

LCARS::Text& LCARS::Text::operator+(const char * str) {
	Append(str);
	return *this;
}

LCARS::Text& LCARS::Text::operator+=(std::string& str) {
	Append(str);
	return *this;
}

LCARS::Text& LCARS::Text::operator+=(const char * str) {
	Append(str);
	return *this;
}

LCARS::Text& LCARS::Text::operator=(std::string str) {
	SetText(str);
	return *this;
}