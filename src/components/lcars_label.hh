#ifndef COMPONENTS_LCARS_LABEL_HH_
#define COMPONENTS_LCARS_LABEL_HH_

#include <string>

#include "../lcars_component.hh"
#include "../paint_context.hh"

namespace LCARS {

	class Label : public Component {

		private:
		
			std::string	m_text_str;
			Text		m_text;

			bool		m_draw_bg;
			_color		m_bg_color;
			_color		m_fg_color;

			Rect m_original_bounds;
			Rect m_padding;

		public:
						Label()				= delete;
						Label(const Label&)	= delete;
						Label(int x, int y, _font * font, std::string text);
			virtual ~	Label();

			void		SetFontColor	(_color c);
			_color		GetFontColor	();

			_color	GetBGColor	();
			void	SetBGColor	(_color c);

			void SetFont(_font * font);
			void SetPadding(int top, int right, int bottom, int left);

			virtual bool PointInHitbox(int x, int y)		override;
			virtual void Paint(PaintContext * paintctx)		override;

			virtual void HandleSDLEvent(XEvent * ev)			override;
			virtual void OnUnhandledSDLEvent(XEvent * ev)	override;

			virtual void HandleCMPEvent(CMP_EVT_TYPE type)		override;

			virtual Label& operator=(const Label&) = delete;
	};
}

#endif /* COMPONENTS_LCARS_LABEL_HH_ */
