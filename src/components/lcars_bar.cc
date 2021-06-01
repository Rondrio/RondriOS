#include "lcars_bar.hh"

#include "../lcars_component.hh"
#include "../lcars_colors.hh"
#include "../lcars_font.hh"
#include "../lcars_error.hh"

void LCARS::Bar::PaintBlockgrid(PaintContext * paintctx, width_color_pair * pairs, int pairs_len, int breating_space, int end_width, int y, int h) {
	
		int x_progression	= m_round_left*(c_ellipse_factor*m_bounds.h/2 + breating_space + m_blockgrid_gap_x/2);
		int gap_count		= pairs_len - 1;
		int useable_width	= m_bounds.w - m_round_left*end_width - m_round_right*end_width - gap_count*m_blockgrid_gap_x;

		int sgn = 0;

		for(int i = 0; i < pairs_len; ++i) {
			
			_color	c		= pairs[i].color;
			double	width	= pairs[i].width;
			
			double wpx	= (useable_width)*(width/100.0);
			
			double alt = (wpx - (int)wpx);
			int wpxi = wpx + sgn*alt - !sgn*alt;
			
			sgn = !sgn;
			
			paintctx->SetColor(c.r, c.g, c.b, c.a);
			paintctx->FillRect(x_progression + m_blockgrid_gap_x/2, y, wpxi, h);
			
			x_progression += wpxi;
			x_progression += m_blockgrid_gap_x;
		}
}

int LCARS::Bar::GetEndBreathingSpace() {
	return m_bounds.h*2;
}

LCARS::Bar::Bar(int x, int y, int w, int h, _color color) : Component((Rect) {x, y, w, h}) {
	m_bg_color = color;
	
	m_round_left	= false;
	m_round_right	= false;
	
	m_blockgrid_gap_x = 0;
	m_blockgrid_gap_y = 0;
	
	m_btn_gapsize = 5;
	
	m_blockgrid_top		= nullptr;
	m_blockgrid_bottom	= nullptr;
	
	m_title = nullptr;
	m_title_color = COL_GOLD;
	
	m_blockgrid_top_len		= 0;
	m_bockgrid_bottom_len	= 0;
}

LCARS::Bar::~Bar() {
	if(m_blockgrid_top)
		free(m_blockgrid_top);
	
	if(m_blockgrid_bottom)
		free(m_blockgrid_bottom);
		
	if(m_title) {
		free(m_title->str);
		LCARS::DestroyFont(m_title->font);
		free(m_title);
	}
}

void LCARS::Bar::SetBackgroundColor(_color color) {
	m_bg_color = color;
	SetNeedsRepaint(true);
}

_color * LCARS::Bar::GetBackgroundColor() {
	return &m_bg_color;
}

void LCARS::Bar::SetRoundLeft(bool b) {
	m_round_left = b;
	SetNeedsRepaint(true);
}

void LCARS::Bar::SetRoundRight(bool b) {
	m_round_right = b;
	SetNeedsRepaint(true);
}

void LCARS::Bar::SetBlockgridGapsize(int gapsize_x, int gapsize_y) {
	m_blockgrid_gap_x = gapsize_x - gapsize_x%2;
	m_blockgrid_gap_y = gapsize_y;
	
	SetNeedsRepaint(true);
}

void LCARS::Bar::SetBlockgridTop(width_color_pair * pairs, int len) {
	if(m_blockgrid_top)
		free(m_blockgrid_top);
	
	m_blockgrid_top = (width_color_pair *) malloc(sizeof(width_color_pair) * len);
	memcpy(m_blockgrid_top, pairs, len*sizeof(width_color_pair));
	
	m_blockgrid_top_len = len;
	
	SetNeedsRepaint(true);
}

void LCARS::Bar::SetBlockgridBottom(width_color_pair * pairs, int len) {
	if(m_blockgrid_bottom)
		free(m_blockgrid_bottom);
	
	m_blockgrid_bottom = (width_color_pair *) malloc(sizeof(width_color_pair) * len);
	memcpy(m_blockgrid_bottom, pairs, len*sizeof(width_color_pair));
	
	m_bockgrid_bottom_len = len;
	
	SetNeedsRepaint(true);
}

void LCARS::Bar::AddButton(LCARS::Button * button) {
	
	m_buttons += button;
	AddChild(button);
	
	const int breathing_gap = GetEndBreathingSpace();
	const int max_width = 200;
	
	int x_progression = breathing_gap;
	
	for(int i = 0; i < m_buttons.Size(); ++i) {
		
		LCARS::Button * btn = m_buttons[i];
		btn->SetPosX(x_progression);
		btn->SetPosY(0);
		
		btn->SetHeight(m_bounds.h);
		
		if(btn->GetWidth() > max_width) {
			btn->SetWidth(max_width);
		}
		
		x_progression += btn->GetWidth() + m_btn_gapsize;
	}
	
	SetNeedsRepaint(true);
}

void LCARS::Bar::SetTitle(std::string title, std::string fontname) {
	
	Text * text = (Text *) malloc(sizeof(Text));
	bool keep_font = false;
	
	if(fontname.length() > 0) {
		
		/* Create new Font */
		text->font = CreateFontPX(fontname, m_bounds.h - m_bounds.h/4);
		
	} else if(m_title) {
		
		/* If there was no fontname given, take the existing font. */
		text->font = m_title->font;
		keep_font = true;
		
	} else {
		
		/* Neither an old nor a new font were available, return. */
		LOG("LCARS::SolidDeco::SetLabel()", "No Font set.");
		return;
	}
	
	text->str = (char *) malloc(title.length());
	memcpy(text->str, title.c_str(), title.length());
	
	LCARS::TextSize(text);
	
	if(m_title) {
		free(m_title->str);
		
		/* The existing font was passed on, do not delete. */
		if(!keep_font)
			LCARS::DestroyFont(m_title->font);
		
		free(m_title);
	}
	
	m_title = text;
	SetNeedsRepaint(true);
}

void LCARS::Bar::SetTitleColor(_color color) {
	m_title_color = color;
	SetNeedsRepaint(true);
}

// ------- overrides ---------

bool LCARS::Bar::PointInHitbox(int x, int y) {
	Rect abs = GetAbsoluteBounds();
	return x >= abs.x && x <= abs.x + m_bounds.w && y >= abs.y && y <= abs.y + m_bounds.h;
}

void LCARS::Bar::Paint(PaintContext * paintctx) {

	/* Paint Background black */
	paintctx->SetColor(0, 0, 0, 255);
	paintctx->FillRect(0, 0, m_bounds.w, m_bounds.h);

	const int roundingds_breathing_space = GetEndBreathingSpace();

	int rounding_x = m_round_left*c_ellipse_factor*m_bounds.h/2;
	int rounding_y = 0;
	
	int inner_rect_w = m_bounds.w - rounding_x*2;
	
	paintctx->SetColor(m_bg_color.r, m_bg_color.g, m_bg_color.b, m_bg_color.a);
	
	if(m_round_left) {
		paintctx->FillEllipse(rounding_x, m_bounds.h/2, m_bounds.h/2*c_ellipse_factor, m_bounds.h/2);
		paintctx->FillRect(rounding_x, 0, roundingds_breathing_space, m_bounds.h);
	}
	
	if(m_round_right) {
		paintctx->FillEllipse(m_bounds.w - rounding_x, m_bounds.h/2, m_bounds.h/2*c_ellipse_factor, m_bounds.h/2);
		paintctx->FillRect(m_bounds.w - rounding_x - roundingds_breathing_space, 0, roundingds_breathing_space, m_bounds.h);
	}
	
	int erase_x = (roundingds_breathing_space + rounding_x)*m_round_left;
	int erase_w = m_bounds.w - 2*(roundingds_breathing_space + rounding_x)*m_round_right;
	
	/* Erase the inner halfs of the drawn Ellipses */
	paintctx->SetColor(0, 0, 0, 255);
	paintctx->FillRect(erase_x, 0, erase_w, m_bounds.h);
	
	int blockgrid_row_h		= m_bounds.h*(m_bockgrid_bottom_len <= 0) + (m_bounds.h - m_blockgrid_gap_y)/2*(m_bockgrid_bottom_len > 0);
	int end_extension_width	= c_ellipse_factor*m_bounds.h/2 + roundingds_breathing_space + m_blockgrid_gap_x;

	if(!m_blockgrid_top && !m_blockgrid_bottom) {
		paintctx->SetColor(m_bg_color.r, m_bg_color.g, m_bg_color.b, m_bg_color.a);
		paintctx->FillRect(rounding_x, rounding_y, inner_rect_w, blockgrid_row_h);
		
	} else {
		if(m_blockgrid_top) {
			PaintBlockgrid(paintctx, m_blockgrid_top, m_blockgrid_top_len, roundingds_breathing_space, end_extension_width, 0, blockgrid_row_h);
		}
		
		if(m_blockgrid_bottom) {
			PaintBlockgrid(paintctx, m_blockgrid_bottom, m_bockgrid_bottom_len, roundingds_breathing_space, end_extension_width, blockgrid_row_h + m_blockgrid_gap_y, blockgrid_row_h);
		}
	}
	
	/* Paint Background behind the Buttons black. */
	if(m_buttons.Size() > 0) {
		
		int x = roundingds_breathing_space - m_btn_gapsize;
		int w = m_btn_gapsize;
		
		for(int i = 0; i < m_buttons.Size(); ++i) {
			w += m_buttons[i]->GetWidth();
			w += m_btn_gapsize;
		}
		
		paintctx->SetColor(0, 0, 0, 255);
		paintctx->FillRect(x, 0, w, m_bounds.h);
	}
	
	/* Draw Title */
	if(m_title) {
		
		const int text_gap = 5;
		
		int x = m_bounds.w - m_title->w - roundingds_breathing_space;
		int y = (m_bounds.h - m_title->h)/2;
		
		/* Blacken Background */
		paintctx->SetColor(0, 0, 0, 255);
		paintctx->FillRect(x - text_gap, 0, m_title->w + 2*text_gap, m_bounds.h);
		
		/* Draw Text centered */
		paintctx->SetColor(m_title_color.r, m_title_color.g, m_title_color.b, m_title_color.a);
		paintctx->DrawText(x, y, m_title);
	}
}

void LCARS::Bar::HandleSDLEvent(XEvent * ev) {
	
}

void LCARS::Bar::OnUnhandledSDLEvent(XEvent * ev) {

}

void LCARS::Bar::HandleCMPEvent(CMP_EVT_TYPE type) {

}
