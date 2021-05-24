#ifndef LCARS_TEXT_HH_
#define LCARS_TEXT_HH_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <cstdint>

namespace LCARS {

	class Font {
		private:
			TTF_Font *	m_font;
			std::string m_font_path;
			uint16_t	m_ptsize;
			SDL_Rect	m_bounds;

			int Regenerate();

			
		public:
						Font()					= delete;
						Font(const Font&)	= delete;
						Font(std::string font_path, uint16_t ptsize);
			virtual ~	Font();

			void		SetPointSize(uint16_t ptsize);
			uint16_t 	GetPointSize();

			void		SetFont(std::string font_path);
			TTF_Font *	GetFont();

			void		QuerySize(std::string text, int * w, int * h);

			virtual Font& operator=(const Font&) = delete;
	};

	class Text {

		private:
			std::string		m_text;
			Font *			m_font;

			SDL_Color		m_color;
			SDL_Texture *	m_texture;
			SDL_Surface *	m_surface;
			bool			m_needs_regeneration;

			Text(std::string text, uint16_t ptsize);

		public:
						Text()				= delete;
						Text(const Text&)	= delete;
						Text(std::string text, std::string font_path, uint16_t ptsize);
						Text(std::string text, Font * font);
			virtual ~	Text();

			void Regenerate(SDL_Renderer * renderer);

			void		SetText	(std::string text);
			std::string GetText	();

			void InsertText	(std::string text, uint16_t position);
			void Append		(std::string text);

			void PopBack	(uint16_t count);
			void PopAt		(uint16_t position, uint16_t count);

			void SetFont	(std::string path);
			void SetFont	(Font * font);
			void SetFontSize(uint16_t ptsize);

			void SetColor(SDL_Color color);

			int Length();
			
			SDL_Rect		GetBounds();
			SDL_Texture *	GetTexture();


			Text& operator+=(std::string& str);
			Text& operator+=(const char * str);
			
			Text& operator+(std::string& str);
			Text& operator+(const char * str);

			Text& operator=(std::string str);
			
			virtual Text& operator=(const Text&) = delete;
	};
}

#endif