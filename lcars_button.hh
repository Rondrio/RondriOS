#ifndef LCARS_BUTTON_HH_
#define LCARS_BUTTON_HH_

#include "lcars_component.hh"

#include <SDL2/SDL_ttf.h>
#include <string>

#include "lcars_text.hh"
#include "simple_list.hh"

namespace LCARS {

	enum struct BTN_COLOR_TYPE {
		COLOR_IDLE,			COLOR_HOVER,		COLOR_PRESS,		COLOR_ACTIVE,
		TEXT_COLOR_IDLE,	TEXT_COLOR_HOVER,	TEXT_COLOR_PRESS,	TEXT_COLOR_ACTIVE
	};

	typedef void (*action_listener)(void);

	class Button : public Component {

		private:
			Button(SDL_Rect bounds);

		protected:
			volatile bool	m_pressed;
			Text *			m_text;

			SDL_Color *					m_colors;
			smp::list<action_listener>	m_action_listeners;

			Button(SDL_Rect bounds, Font * font, std::string text);
			Button(SDL_Rect bounds, std::string font_path, std::string text);

		public:
						Button()				= delete;
						Button(const Button&)	= delete;
			virtual ~	Button();

			virtual void HandleSDLEvent(SDL_Event * ev) override;
			virtual void HandleCMPEvent(CMP_EVT_TYPE type) override;

			virtual void OnMouseDown		(SDL_MouseButtonEvent * ev);
			virtual void OnMouseUp			(SDL_MouseButtonEvent * ev);
			virtual void OnMouseMove		(SDL_MouseMotionEvent * ev);

			void AddActionListener(action_listener listener);
			void RemActionListener(action_listener listener);

			void			SetFont(Font * font);

			void			SetText(std::string text);
			Text *			GetText();

			void		SetColor		(BTN_COLOR_TYPE colortype, SDL_Color color);
			SDL_Color 	GetColor		(BTN_COLOR_TYPE colortype);

			virtual Button& operator=(const Button& rhs) = delete;
	};
}

#endif /* LCARS_BUTTON_HH_ */
