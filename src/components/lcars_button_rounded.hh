#ifndef COMPONENTS_LCARS_BUTTON_ROUNDED_HH_
#define COMPONENTS_LCARS_BUTTON_ROUNDED_HH_

#include "../lcars_button.hh"
#include "../paint_context.hh"

namespace LCARS {

	class ButtonRounded : public Button {

		private:
			static constexpr double c_ellipse_factor = 39.0/46.0;
			
			int m_circle_radius;

			volatile bool m_round_left;
			volatile bool m_round_right;
			
			int m_off_right, m_off_left;
			Rect m_original_bounds;

			bool IsInInnerRect	(int x, int y, Rect abs);
			bool IsInCircles	(int x, int y, Rect abs);
			Rect CalcInnerRect();
			
		public:
						ButtonRounded()								= delete;
						ButtonRounded(const ButtonRounded&)			= delete;
						ButtonRounded(int x, int y, int w, int h, _font * font, std::string text);
			virtual ~	ButtonRounded();

			void SetRoundLeft	(bool b);
			void SetRoundRight	(bool b);

			void SetOffset(int left, int right);

			virtual bool PointInHitbox		(int x, int y)				override;
			virtual void Paint				(PaintContext * paintctx)	override;
			virtual void HandleSDLEvent		(XEvent * ev)			override;
			virtual void OnUnhandledSDLEvent(XEvent * ev)			override;

			virtual ButtonRounded& operator=(const ButtonRounded&) = delete;
	};
}

#endif /* COMPONENTS_LCARS_BUTTON_ROUNDED_HH_ */
