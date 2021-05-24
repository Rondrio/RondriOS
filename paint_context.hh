#ifndef PAINT_CONTEXT_HH_
#define PAINT_CONTEXT_HH_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>

#include "lcars_text.hh"

struct bezier_t {
	signed short * vx;
	signed short * vy;
	int n;
	int s;
};

struct text_t {
	SDL_Texture *	tex_text;
	SDL_Surface *	sur_text;
	std::string	*	str;
	SDL_Rect		bounds;
};

class PaintContext {
	private:
		SDL_Rect	m_bounds;
		SDL_Color	m_color;

		TTF_Font *	m_font;

		SDL_Renderer *	m_renderer;
		SDL_Texture *	m_buffer;
		SDL_Texture *	m_texture;

		int m_flip;

		text_t *	PrepareText(int16_t x, int16_t y, std::string * str, SDL_Surface * surf);
		void		PrepareRect(SDL_Rect * rect, SDL_Rect * rect_out);

	public:
			PaintContext()						= delete;
			PaintContext(const PaintContext&)	= delete;
			PaintContext(SDL_Renderer * renderer, SDL_Texture * buffer, SDL_Rect bounds);
		~	PaintContext();

		SDL_Rect const * GetOffset();

		void FlipVertically();
		void FlipHorizontally();

		void SetColor	(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void SetFont	(TTF_Font * font);

		void FillRect(SDL_Rect * rect);
		void DrawRect(SDL_Rect * rect);

		void FillCircle(int16_t x, int16_t y, int16_t rad);
		void DrawCircle(int16_t x, int16_t y, int16_t rad);

		void FillEllipse(int16_t x, int16_t y, int16_t rx, int16_t ry);
		void DrawEllipse(int16_t x, int16_t y, int16_t rx, int16_t ry);

		void DrawImage(int x, int y, int w, int h, SDL_Surface * img);

		void PaintScreen();
		SDL_Texture * GetTexture();

		/**
		 * For every Text, this Method should only be called once. As it is
		 * very expensive.
		 * */
				text_t *	PrepareBlendedText(int16_t x, int16_t y, std::string * str);
				text_t *	PrepareSolidText(int16_t x, int16_t y, std::string * str);
		static	void		DestroyText(text_t * text);

		void DrawText(text_t * text, SDL_Rect * src, SDL_Rect * dst);
		void DrawText(int x, int y, LCARS::Text * text);

		PaintContext& operator=(const PaintContext&) = delete;
};


#endif /* PAINT_CONTEXT_HH_ */
