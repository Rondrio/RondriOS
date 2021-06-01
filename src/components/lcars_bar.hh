#ifndef LCARS_BAR_HH_
#define LCARS_BAR_HH_

#include "../lcars_component.hh"
#include "../lcars_button.hh"

struct width_color_pair {
	double width;
	_color color;
};

namespace LCARS {

	class Bar : public Component {

		private:
			static constexpr double c_ellipse_factor	= 39.0/46.0;
		
			_color m_bg_color;
			
			bool m_round_left;
			bool m_round_right;

			int m_blockgrid_gap_x, m_blockgrid_gap_y;
			
			width_color_pair * m_blockgrid_top;
			width_color_pair * m_blockgrid_bottom;
			
			int m_blockgrid_top_len, m_bockgrid_bottom_len;

			smp::list<Button *> m_buttons;
			int m_btn_gapsize;
			
			_color m_title_color;
			Text * m_title;

			void	PaintBlockgrid(PaintContext * paintctx, width_color_pair * pairs, int pairs_len, int breating_space, int end_width, int y, int h);
			int		GetEndBreathingSpace();

		public:
						Bar()					= delete;
						Bar(const Bar&)	= delete;
						Bar(int x, int y, int w, int h, _color color);
			virtual ~	Bar();

			void		SetBackgroundColor(_color color);
			_color *	GetBackgroundColor();

			void SetRoundLeft	(bool b);
			void SetRoundRight	(bool b);

			void SetBlockgridGapsize	(int gapsize_x, int gapsize_y);
			void SetBlockgridTop		(width_color_pair * pairs, int len);
			void SetBlockgridBottom		(width_color_pair * pairs, int len);
			
			void SetTitle(std::string title, std::string fontname = "");
			void SetTitleColor(_color color);
			
			void AddButton(Button * button);
			
			virtual bool PointInHitbox(int x, int y)		override;
			virtual void Paint(PaintContext * paintctx)		override;

			virtual void HandleSDLEvent(XEvent * ev)		override;
			virtual void OnUnhandledSDLEvent(XEvent * ev)	override;

			virtual void HandleCMPEvent(CMP_EVT_TYPE type)	override;

			virtual Bar& operator=(const Bar&) = delete;
	};
}

#endif