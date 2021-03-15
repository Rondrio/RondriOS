#ifndef PAINT_CONTEXT_HH_
#define PAINT_CONTEXT_HH_

#include <SDL2/SDL.h>

struct bezier_t {
	signed short * vx;
	signed short * vy;
	int n;
	int s;
};

class PaintContext {
	private:
		SDL_Rect	m_bounds;
		SDL_Color	m_color;

	public:
			PaintContext() = delete;
			PaintContext(SDL_Renderer * renderer, SDL_Rect bounds);
		~	PaintContext();

		void SetColor(int r, int g, int b, int a);

		void FillRect(SDL_Rect * rect);
		void DrawRect(SDL_Rect * rect);

		void FillCircle(int x, int y, int r);
		void DrawCircle(int x, int y, int r);

		void DrawBezier(bezier_t bezier_curve);
};


#endif /* PAINT_CONTEXT_HH_ */
