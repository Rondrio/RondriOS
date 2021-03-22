#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

int pixel(SDL_Renderer *renderer, Sint16 x, Sint16 y)
{
	return SDL_RenderDrawPoint(renderer, x, y);
}

int vline(SDL_Renderer * renderer, Sint16 x, Sint16 y1, Sint16 y2)
{
	return SDL_RenderDrawLine(renderer, x, y1, x, y2);;
}

int hline(SDL_Renderer * renderer, Sint16 x1, Sint16 x2, Sint16 y)
{
	return SDL_RenderDrawLine(renderer, x1, y, x2, y);;
}

int pixelRGBAWeight(SDL_Renderer * renderer, Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint32 weight)
{
	/*
	* Modify Alpha by weight
	*/
	Uint32 ax = a;
	ax = ((ax * weight) >> 8);
	if (ax > 255) {
		a = 255;
	} else {
		a = (Uint8)(ax & 0x000000ff);
	}

	return pixelRGBA(renderer, x, y, r, g, b, a);
}

static int renderdrawline(SDL_Renderer *renderer, int x1, int y1, int x2, int y2)
{
	int result ;
#ifndef __EMSCRIPTEN__
	if ((x1 == x2) && (y1 == y2))
		result = SDL_RenderDrawPoint (renderer, x1, y1) ;
	else if (y1 == y2)
	    {
		int x ;
		if (x1 > x2) { x = x1 ; x1 = x2 ; x2 = x ; }
		SDL_Point *points = (SDL_Point*) malloc ((x2 - x1 + 1) * sizeof(SDL_Point)) ;
		if (points == NULL) return -1 ;
		for (x = x1; x <= x2; x++)
		    {
			points[x - x1].x = x ;
			points[x - x1].y = y1 ;
		    }
		result = SDL_RenderDrawPoints (renderer, points, x2 - x1 + 1) ;
		free (points) ;
	    }
	else if (x1 == x2)
	    {
		int y ;
		if (y1 > y2) { y = y1 ; y1 = y2 ; y2 = y ; }
		SDL_Point *points = (SDL_Point*) malloc ((y2 - y1 + 1) * sizeof(SDL_Point)) ;
		if (points == NULL) return -1 ;
		for (y = y1; y <= y2; y++)
		    {
			points[y - y1].x = x1 ;
			points[y - y1].y = y ;
		    }
		result = SDL_RenderDrawPoints (renderer, points, y2 - y1 + 1) ;
		free (points) ;
	    }
	else
#endif
		result = SDL_RenderDrawLine (renderer, x1, y1, x2, y2) ;
	return result ;
}

int aaFilledEllipseRGBA(SDL_Renderer * renderer, float cx, float cy, float rx, float ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	int n, xi, yi, result = 0 ;
	double s, v, x, y, dx, dy ;

	if ((rx <= 0.0) || (ry <= 0.0))
		return -1 ;

	result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) ;
	if (rx >= ry)
	    {
		n = ry + 1 ;
		for (yi = cy - n - 1; yi <= cy + n + 1; yi++)
		    {
			if (yi < (cy - 0.5))
				y = yi ;
			else
				y = yi + 1 ;
			s = (y - cy) / ry ;
			s = s * s ;
			x = 0.5 ;
			if (s < 1.0)
			    {
				x = rx * sqrt(1.0 - s) ;
				if (x >= 0.5)
				    {
					result |= SDL_SetRenderDrawColor (renderer, r, g, b, a ) ;
					result |= renderdrawline (renderer, cx - x + 1, yi, cx + x - 1, yi) ;
				    }
			    }
			s = 8 * ry * ry ;
			dy = fabs(y - cy) - 1.0 ;
			xi = cx - x ; // left
			while (1)
			    {
				dx = (cx - xi - 1) * ry / rx ;
				v = s - 4 * (dx - dy) * (dx - dy) ;
				if (v < 0) break ;
				v = (sqrt(v) - 2 * (dx + dy)) / 4 ;
				if (v < 0) break ;
				if (v > 1.0) v = 1.0 ;
				result |= SDL_SetRenderDrawColor (renderer, r, g, b, (double)a * v) ;
				result |= SDL_RenderDrawPoint (renderer, xi, yi) ;
				xi -= 1 ;
			    }
			xi = cx + x ; // right
			while (1)
			    {
				dx = (xi - cx) * ry / rx ;
				v = s - 4 * (dx - dy) * (dx - dy) ;
				if (v < 0) break ;
				v = (sqrt(v) - 2 * (dx + dy)) / 4 ;
				if (v < 0) break ;
				if (v > 1.0) v = 1.0 ;
				result |= SDL_SetRenderDrawColor (renderer, r, g, b, (double)a * v) ;
				result |= SDL_RenderDrawPoint (renderer, xi, yi) ;
				xi += 1 ;
			    }
		    }
	    }
	else
	    {
		n = rx + 1 ;
		for (xi = cx - n - 1; xi <= cx + n + 1; xi++)
		    {
			if (xi < (cx - 0.5))
				x = xi ;
			else
				x = xi + 1 ;
			s = (x - cx) / rx ;
			s = s * s ;
			y = 0.5 ;
			if (s < 1.0)
			    {
				y = ry * sqrt(1.0 - s) ;
				if (y >= 0.5)
				    {
					result |= SDL_SetRenderDrawColor (renderer, r, g, b, a ) ;
					result |= renderdrawline (renderer, xi, cy - y + 1, xi, cy + y - 1) ;
				    }
			    }
			s = 8 * rx * rx ;
			dx = fabs(x - cx) - 1.0 ;
			yi = cy - y ; // top
			while (1)
			    {
				dy = (cy - yi - 1) * rx / ry ;
				v = s - 4 * (dy - dx) * (dy - dx) ;
				if (v < 0) break ;
				v = (sqrt(v) - 2 * (dy + dx)) / 4 ;
				if (v < 0) break ;
				if (v > 1.0) v = 1.0 ;
				result |= SDL_SetRenderDrawColor (renderer, r, g, b, (double)a * v) ;
				result |= SDL_RenderDrawPoint (renderer, xi, yi) ;
				yi -= 1 ;
			    }
			yi = cy + y ; // bottom
			while (1)
			    {
				dy = (yi - cy) * rx / ry ;
				v = s - 4 * (dy - dx) * (dy - dx) ;
				if (v < 0) break ;
				v = (sqrt(v) - 2 * (dy + dx)) / 4 ;
				if (v < 0) break ;
				if (v > 1.0) v = 1.0 ;
				result |= SDL_SetRenderDrawColor (renderer, r, g, b, (double)a * v) ;
				result |= SDL_RenderDrawPoint (renderer, xi, yi) ;
				yi += 1 ;
			    }
		    }
	    }
	return result ;
}
