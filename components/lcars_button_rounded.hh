#ifndef COMPONENTS_LCARS_BUTTON_ROUNDED_HH_
#define COMPONENTS_LCARS_BUTTON_ROUNDED_HH_

#include <SDL2/SDL.h>

#include "../lcars_button.hh"


class LCARS_Button_Rounded : public LCARS_Button {

	private:
		int m_circle_radius;

	public:
		LCARS_Button_Rounded(int x, int y, int w, int h);

		virtual bool PointInHitbox		(int x, int y)				override;
		virtual void Paint				(SDL_Renderer * renderer)	override;
		virtual void HandleSDLEvent		(SDL_Event * ev)			override;
		virtual void OnUnhandledEvent	(SDL_Event * ev)			override;
};

#endif /* COMPONENTS_LCARS_BUTTON_ROUNDED_HH_ */
