#ifndef COMPONENTS_LCARS_BUTTON_ROUNDED_HH_
#define COMPONENTS_LCARS_BUTTON_ROUNDED_HH_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../lcars_button.hh"
#include "../paint_context.hh"


class LCARS_Button_Rounded : public LCARS_Button {

	private:
		static constexpr double c_ellipse_factor = 39.0/46.0;
		int			m_circle_radius;

		text_t *	m_button_text;

		volatile bool m_round_left;
		volatile bool m_round_right;

		bool IsInInnerRect	(int x, int y, SDL_Rect abs);
		bool IsInCircles	(int x, int y, SDL_Rect abs);
		SDL_Rect CalcInnerRect();

	public:
					LCARS_Button_Rounded()								= delete;
					LCARS_Button_Rounded(const LCARS_Button_Rounded&)	= delete;
					LCARS_Button_Rounded(int x, int y, int w, int h, TTF_Font * font, std::string text);
		virtual ~	LCARS_Button_Rounded();

		void SetRoundLeft	(bool b);
		void SetRoundRight	(bool b);

		virtual bool PointInHitbox		(int x, int y)				override;
		virtual void Paint				(PaintContext * paintctx)	override;
		virtual void HandleSDLEvent		(SDL_Event * ev)			override;
		virtual void OnUnhandledSDLEvent(SDL_Event * ev)			override;

		virtual LCARS_Button_Rounded& operator=(const LCARS_Button_Rounded&) = delete;
};

#endif /* COMPONENTS_LCARS_BUTTON_ROUNDED_HH_ */
