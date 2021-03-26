#ifndef LCARS_BUTTON_HH_
#define LCARS_BUTTON_HH_

#include "lcars_component.hh"
#include <SDL2/SDL_ttf.h>

enum struct BTN_COLOR_TYPE {
	COLOR_IDLE,			COLOR_HOVER,		COLOR_PRESS,		COLOR_ACTIVE,
	TEXT_COLOR_IDLE,	TEXT_COLOR_HOVER,	TEXT_COLOR_PRESS,	TEXT_COLOR_ACTIVE
};

class LCARS_Button : public LCARS_Component {

	protected:
		volatile bool	m_pressed;
		TTF_Font	*	m_font;
		char		*	m_text;

		SDL_Color * m_colors;

		LCARS_Button(SDL_Rect bounds, TTF_Font * font, char * text);

	public:
					LCARS_Button() = delete;
		virtual ~	LCARS_Button() {}

		virtual void HandleSDLEvent(SDL_Event * ev) override;
		virtual void HandleCMPEvent(CMP_EVT_TYPE type) override;

		virtual void OnMouseDown		(SDL_MouseButtonEvent * ev);
		virtual void OnMouseUp			(SDL_MouseButtonEvent * ev);
		virtual void OnMouseMove		(SDL_MouseMotionEvent * ev);

		void			SetFont(TTF_Font * font);
		TTF_Font	*	GetFont();

		void			SetText(char * text);
		char		*	GetText();

		void		SetColor		(BTN_COLOR_TYPE colortype, SDL_Color color);
		SDL_Color *	GetColor		(BTN_COLOR_TYPE colortype);
};


#endif /* LCARS_BUTTON_HH_ */
