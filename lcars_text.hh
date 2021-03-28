#ifndef LCARS_TEXT_HH_
#define LCARS_TEXT_HH_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <cstdint>

class LCARS_Font {
	private:
		TTF_Font *	m_font;
		std::string m_font_path;
		uint16_t	m_ptsize;
		SDL_Rect	m_bounds;

		int Regenerate();

	public:
					LCARS_Font()					= delete;
					LCARS_Font(const LCARS_Font&)	= delete;
					LCARS_Font(std::string font_path, uint16_t ptsize);
		virtual ~	LCARS_Font();

		void		SetPointSize(uint16_t ptsize);
		uint16_t 	GetPointSize();

		void		SetFont(std::string font_path);
		TTF_Font *	GetFont();

		void		QuerySize(std::string text, int * w, int * h);

		virtual LCARS_Font& operator=(const LCARS_Font&) = delete;
};

class LCARS_Text {

	private:
		std::string		m_text;
		LCARS_Font *	m_font;

		SDL_Color		m_color;
		SDL_Texture *	m_texture;
		bool			m_needs_regeneration;

	public:
					LCARS_Text()					= delete;
					LCARS_Text(const LCARS_Text&)	= delete;
					LCARS_Text(std::string text, std::string font_path, uint16_t ptsize);
		virtual ~	LCARS_Text();

		void Regenerate(SDL_Renderer * renderer);

		void		SetText	(std::string text);
		std::string GetText	();

		void InsertText	(std::string text, uint16_t position);
		void Append		(std::string text);

		void PopBack	(uint16_t count);
		void PopAt		(uint16_t position, uint16_t count);

		void SetFont	(std::string path);
		void SetFontSize(uint16_t ptsize);

		void SetColor(SDL_Color color);
		
		SDL_Rect		GetBounds();
		SDL_Texture *	GetTexture();

		virtual LCARS_Text& operator=(const LCARS_Text&) = delete;
};

#endif