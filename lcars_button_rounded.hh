#ifndef COMPONENTS_LCARS_BUTTON_ROUNDED_HH_
#define COMPONENTS_LCARS_BUTTON_ROUNDED_HH_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../lcars_button.hh"
#include "../paint_context.hh"

namespace LCARS {

	class ButtonRounded : public Button {

		private:
			static constexpr double c_ellipse_factor = 39.0/46.0;
			
			int m_circle_radius;

			volatile bool m_round_left;
			volatile bool m_round_right;

			bool IsInInnerRect	(int x, int y, SDL_Rect abs);
			bool IsInCircles	(int x, int y, SDL_Rect abs);
			SDL_Rect CalcInnerRect();

			void Init();

		public:
						ButtonRounded()								= delete;
						ButtonRounded(const ButtonRounded&)			= delete;
						ButtonRounded(int x, int y, int w, int h, Font * font, std::string text);
						ButtonRounded(int x, int y, int w, int h, std::string fontpath, std::string text);
			virtual ~	ButtonRounded();

			void SetRoundLeft	(bool b);
			void SetRoundRight	(bool b);

			virtual bool PointInHitbox		(int x, int y)				override;
			virtual void Paint				(PaintContext * paintctx)	override;
			virtual void HandleSDLEvent		(SDL_Event * ev)			override;
			virtual void OnUnhandledSDLEvent(SDL_Event * ev)			override;

			virtual ButtonRounded& operator=(const ButtonRounded&) = delete;
	};
}

#endif /* COMPONENTS_LCARS_BUTTON_ROUNDED_HH_ */
