#ifndef LCARS_BUTTON_HH_
#define LCARS_BUTTON_HH_

#include <string>

#include "simple_list.hh"
#include "lcars_component.hh"

#include "paint_context.hh"

namespace LCARS {

	enum struct BTN_COLOR_TYPE {
		COLOR_IDLE,			COLOR_HOVER,		COLOR_PRESS,		COLOR_ACTIVE,
		TEXT_COLOR_IDLE,	TEXT_COLOR_HOVER,	TEXT_COLOR_PRESS,	TEXT_COLOR_ACTIVE
	};

	typedef void (*action_listener)(void);

	class Button : public Component {

		protected:
			static constexpr double c_ellipse_factor = 39.0/46.0;
			
			volatile bool	m_pressed;

			Text *	m_text;

			_color *					m_colors;
			smp::list<action_listener>	m_action_listeners;
			
			Rect m_base_bounds;
			
			bool m_capped_left;
			bool m_capped_right;
			
			int m_cap_offset_left;
			int m_cap_offset_right;
			
			int CalcCapRadius();
			void Rebound();
			
		public:
						Button()				= delete;
						Button(const Button&)	= delete;
						Button(int x, int y, int w, int h, _font * font, std::string text);
			virtual ~	Button();

			virtual void OnMouseDown		(XButtonPressedEvent * ev);
			virtual void OnMouseUp			(XButtonReleasedEvent * ev);
			virtual void OnMouseMove		(XMotionEvent * ev);

			void AddActionListener (action_listener listener);
			void RemActionListener (action_listener listener);

			void SetFont(_font * font);

			void	SetText(std::string text);
			Text *	GetText();

			void	SetColor (BTN_COLOR_TYPE colortype, _color color);
			_color 	GetColor (BTN_COLOR_TYPE colortype);
			
			void SetCappedLeft	(bool b);
			void SetCappedRight	(bool b);
			
			void SetCapOffsetLeft	(int amount);
			void SetCapOffsetRight	(int amount);
			
			void	SetPosX(int x)	override;
			int		GetPosX()		override;
			
			void	SetPosY(int y)	override;
			int		GetPosY()		override;
			
			void	SetWidth(int width)	override;
			int		GetWidth()			override;
			
			virtual bool PointInHitbox		(int x, int y)			override;
			virtual void Paint				(PaintContext * ctx)	override;
			virtual void OnUnhandledSDLEvent(XEvent * ev)			override;
			virtual void HandleSDLEvent		(XEvent * ev)			override;
			virtual void HandleCMPEvent		(CMP_EVT_TYPE type)		override;

			virtual Button& operator=(const Button& rhs) = delete;
	};
}

#endif /* LCARS_BUTTON_HH_ */
