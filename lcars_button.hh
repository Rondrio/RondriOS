#ifndef LCARS_BUTTON_HH_
#define LCARS_BUTTON_HH_

#include "lcars_component.hh"

class LCARS_Button : public LCARS_Component {

	protected:
		bool	m_pressed;

		LCARS_Button(SDL_Rect bounds) : LCARS_Component(bounds) {
			m_pressed = false;
		}

	public:
					LCARS_Button() = delete;
		virtual ~	LCARS_Button() {}

		virtual void HandleSDLEvent(SDL_Event * ev) override;

		virtual void OnMouseDown		(SDL_MouseButtonEvent * ev);
		virtual void OnMouseUp			(SDL_MouseButtonEvent * ev);
		virtual void OnMouseMove		(SDL_MouseMotionEvent * ev);
};


#endif /* LCARS_BUTTON_HH_ */
