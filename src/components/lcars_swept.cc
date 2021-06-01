#include "lcars_swept.hh"

#include "../lcars_colors.hh"
#include "../lcars_font.hh"

LCARS::Swept::Swept(int x, int y, int bar_height, int bar_width, int arm_height, int arm_width, int shoulder_ry, int armpit_ry, SWEPT_ORIENTATION orientation) : Component((Rect) {x, y, bar_width, (arm_height - shoulder_ry) > bar_height + armpit_ry ? arm_height : bar_height + armpit_ry + shoulder_ry}) {
	m_arm_height	= arm_height;
	m_arm_width		= arm_width;

	m_bar_height	= bar_height;
	m_bar_width		= bar_width;

	m_shoulder_ry	= shoulder_ry;
	m_shoulder_rx	= c_arc_factor*shoulder_ry;

	m_armpit_ry	= armpit_ry;
	m_armpit_rx	= c_arc_factor*armpit_ry;

	m_ext_arc_ry = m_bar_height/2;
	m_ext_arc_rx = m_ext_arc_ry*c_ellipse_factor;

	m_titles[(int) SWEPT_ADDPOS::IN_ARM]	= nullptr;
	m_titles[(int) SWEPT_ADDPOS::IN_BAR]	= nullptr;

	m_button_margin	= 5;
	m_label_margin	= 5;

	m_orientation = orientation;
	m_color = COL_GOLD;

	m_extension_type = SWEPT_EXTENSION::ROUNDED;
}

LCARS::Swept::~Swept() {
	for(int i = 0; i < 2; ++i) {
		Text * active_label = m_titles[i];
		if(active_label) {
			LCARS::DestroyFont(active_label->font);
			free(active_label->str);
			free(active_label);
		}
	}
}

Rect LCARS::Swept::CalcBase() {

	int width = 0;

	switch(m_extension_type) {
		case SWEPT_EXTENSION::FLAT: {
			width = m_bar_width-m_shoulder_rx;
			break;
		}

		case SWEPT_EXTENSION::ROUNDED: {
			width = m_bar_width-m_shoulder_rx-m_ext_arc_rx;
			break;
		}

		case SWEPT_EXTENSION::SHARP: {
			width = m_bar_width - m_shoulder_rx - m_bar_height/c_ellipse_factor;
			break;
		}
	}

	return {m_shoulder_rx, 0, width, m_bar_height};
}

Rect LCARS::Swept::CalcUnderMainArc() {
	int height = 2*m_shoulder_ry-m_arm_height;
	height = height < 0 ? 0 : height;

	return {0, m_arm_height, m_arm_width, height};
}

Rect LCARS::Swept::CalcUnderMainArc2() {
	return {m_arm_width, m_bar_height, m_shoulder_rx*2-m_arm_width, m_shoulder_ry*2+m_bar_height};
}

Rect LCARS::Swept::CalcMainArc() {
	return {m_shoulder_rx, m_shoulder_ry, m_shoulder_rx, m_shoulder_ry};
}

Rect LCARS::Swept::CalcArm() {
	int height = m_arm_height - m_shoulder_ry;
	height = height > m_bar_height + m_armpit_ry ? height : m_bar_height + m_armpit_ry;

	return {0, m_shoulder_ry, m_arm_width, height};
}

Rect LCARS::Swept::CalcUnderSmallArc() {
	int arm_main_arc_diff_x = m_arm_width - m_shoulder_rx;
	int main_arc_base_diff_y = m_shoulder_ry - m_bar_height;

	arm_main_arc_diff_x = arm_main_arc_diff_x > 0 ? arm_main_arc_diff_x : 0;
	main_arc_base_diff_y = main_arc_base_diff_y > 0 ? main_arc_base_diff_y : 0;

	int x = m_arm_width < m_shoulder_rx ? m_arm_width : m_shoulder_rx;

	return {x, m_bar_height, arm_main_arc_diff_x + m_armpit_rx, main_arc_base_diff_y + m_armpit_ry};
}

Rect LCARS::Swept::CalcUnderSmallArc2() {
	return {m_arm_width, m_bar_height+m_armpit_ry, m_armpit_rx*2, m_armpit_ry*2 + m_bar_height};
}

Rect LCARS::Swept::CalcExtensionArc(PaintContext * paintctx) {

	paintctx->SetColor(m_color.r, m_color.g, m_color.b, m_color.a);

	Rect rect = {0, 0, 0, 0};

	switch(m_extension_type) {
		case SWEPT_EXTENSION::ROUNDED: {
			paintctx->FillEllipse(m_bar_width-m_ext_arc_rx, m_bar_height/2, m_ext_arc_rx, m_ext_arc_ry);
			rect = {0, 0, m_ext_arc_rx*2, m_ext_arc_ry};
			break;
		}

		case SWEPT_EXTENSION::FLAT: {
			rect = {0, 0, m_ext_arc_rx*2, m_ext_arc_ry};
			break;
		}

		case SWEPT_EXTENSION::SHARP: {

			int rx = (int)(m_bar_height/c_ellipse_factor);

			paintctx->FillEllipse(m_bar_width - rx, m_bar_height, rx, m_bar_height);

			rect = {0, 0, rx, m_bar_height};

			paintctx->SetColor(0, 0, 0, 255);
			paintctx->FillRect(m_bar_width - rx*2, m_bar_height, rx*2, rx);
		}
	}

	return rect;
}

Rect LCARS::Swept::CalcSmallArc() {
	return {m_arm_width + m_armpit_rx, m_bar_height+m_armpit_ry, m_armpit_rx, m_armpit_ry};
}

void LCARS::Swept::Paint(PaintContext * paintctx) {

	paintctx->SetColor(0, 0, 0, 255);
	paintctx->FillRect(0, 0, m_bounds.w, m_bounds.h);

	paintctx->SetColor(m_color.r, m_color.g, m_color.b, m_color.a);

	Rect main_base = CalcBase();
	paintctx->FillRect(main_base.x, main_base.y, main_base.w, main_base.h);

	Rect main_arc = CalcMainArc();
	paintctx->FillEllipse(main_arc.x, main_arc.y, main_arc.w, main_arc.h);

	paintctx->SetColor(m_color.r, m_color.g, m_color.b, m_color.a);

	Rect arm_rect = CalcArm();
	paintctx->FillRect(arm_rect.x, arm_rect.y, arm_rect.w, arm_rect.h);

	Rect fill_small_arc = CalcUnderSmallArc();
	paintctx->FillRect(fill_small_arc.x, fill_small_arc.y, fill_small_arc.w, fill_small_arc.h);

	paintctx->SetColor(0, 0, 0, 255);
	Rect fill_small_arc2 = CalcUnderSmallArc2();
	paintctx->FillRect(fill_small_arc2.x, fill_small_arc2.y, fill_small_arc2.w, fill_small_arc2.h);

	Rect small_arc = CalcSmallArc();
	paintctx->FillEllipse(small_arc.x, small_arc.y, small_arc.w, small_arc.h);

	/* Misleading Function-Name. CalcExtensionArc does not only Calc, it draws aswell. */
	Rect ext_arc = CalcExtensionArc(paintctx);

	/* Draw black Backgrounds for the Buttons that are attached to this BigDeco */
	paintctx->SetColor(0, 0, 0, 255);
	
	for(int i = 0; i < m_buttons_bar.Size(); ++i) {
		
		Button * btn = m_buttons_bar[i];
		int x;
		
		if(m_orientation == SWEPT_ORIENTATION::ARM_DOWN_LEFT || m_orientation == SWEPT_ORIENTATION::ARM_UP_LEFT)
			x = btn->GetPosX() - m_button_margin;
		else
			x = m_bar_width - btn->GetPosX() - btn->GetWidth() - m_button_margin;
		
		paintctx->FillRect(x, 0, btn->GetWidth() + m_button_margin*2, btn->GetHeight());
	}
	
	for(int i = 0; i < m_buttons_arm.Size(); ++i) {
		
		Button * btn = m_buttons_arm[i];
		int y;
		
		if(m_orientation == SWEPT_ORIENTATION::ARM_DOWN_LEFT || m_orientation == SWEPT_ORIENTATION::ARM_DOWN_RIGHT)
			y = btn->GetPosY() - m_button_margin;
		else
			y = m_arm_height - btn->GetPosY() - btn->GetHeight() - m_button_margin;
		
		paintctx->FillRect(0, y, m_arm_width, btn->GetHeight() + m_button_margin*2);
	}
	
	/* Draw any Label in the Bar of the Swept */
	if(m_titles[(int) LCARS::SWEPT_ADDPOS::IN_BAR]) {
		Text * label	= m_titles[(int) LCARS::SWEPT_ADDPOS::IN_BAR];
		_color c		= m_title_colors[(int) LCARS::SWEPT_ADDPOS::IN_BAR];
		
		int x = m_bar_width - label->w - ext_arc.w;
		int y = (m_bar_height - label->h)/2;
		int w = label->w;
		int h = m_bar_height;
		
		int padding = 5;
		
		paintctx->SetColor(0, 0, 0, 255);
		paintctx->FillRect(x - padding, 0, w + padding*2, h);

		paintctx->SetColor(c.r, c.g, c.b, c.a);
		paintctx->DrawText(x, y, label);
	}

	/* Draw any Title in the arm of the Swept */
	if(m_titles[(int) LCARS::SWEPT_ADDPOS::IN_ARM]) {
		Text * label	= m_titles[(int) LCARS::SWEPT_ADDPOS::IN_ARM];
		_color c		= m_title_colors[(int) LCARS::SWEPT_ADDPOS::IN_ARM];

		int padding = 5;
		int x = m_arm_width - label->w - padding;
		int y = m_arm_height - label->h - padding;

		paintctx->SetColor(c.r, c.g, c.b, c.a);
		paintctx->DrawText(x, y, label);
		
	}

	/* Flip according to the BigDeco's Orientation */
	switch(m_orientation) {
		case SWEPT_ORIENTATION::ARM_DOWN_RIGHT: {
			paintctx->FlipX();
			break;
		}
		case SWEPT_ORIENTATION::ARM_UP_RIGHT: {
			paintctx->FlipX();
			paintctx->FlipY();
			break;
		}
		case SWEPT_ORIENTATION::ARM_UP_LEFT: {
			paintctx->FlipY();
			break;
		}
		case SWEPT_ORIENTATION::ARM_DOWN_LEFT:
			break;
	}
}

void LCARS::Swept::SetOrientation(SWEPT_ORIENTATION o) {
	m_orientation = o;
	SetNeedsRepaint(true);
}

void LCARS::Swept::SetTitle(std::string text, std::string fontname, SWEPT_ADDPOS label_pos, _color c) {

	Text * t = (Text *) malloc(sizeof(Text));
	t->font	= CreateFontPX(fontname, m_bar_height - m_bar_height/4);
	t->str	= (char *) malloc(text.length());
		
	strcpy(t->str, text.c_str());
	LCARS::TextSize(t);
	
	/* Free existing Label */
	Text * active_label = m_titles[(int) label_pos];
	if(active_label) {
		LCARS::DestroyFont(active_label->font);
		free(active_label->str);
		free(active_label);
	}
	
	m_titles[(int) label_pos] = t;
	m_title_colors[(int) label_pos] = c;
	
	SetNeedsRepaint(true);
}

void LCARS::Swept::SetColor(_color color) {
	m_color = color;
}

void LCARS::Swept::AddButton(Button * button, SWEPT_ADDPOS pos) {

	AddChild(button);
	
	if(pos == SWEPT_ADDPOS::IN_BAR) {
		
		m_buttons_bar += button;
		
		const int max_width = 200;
		int x_progression	= 0;
		int y				= 0;
		int sgn				= 0;
		
		if(m_orientation == SWEPT_ORIENTATION::ARM_DOWN_LEFT) {
			x_progression = m_arm_width + m_armpit_rx + m_button_margin;
			y = 0;
			sgn = 1;
			
		} else if(m_orientation == SWEPT_ORIENTATION::ARM_DOWN_RIGHT) {
			x_progression = m_bar_width - m_arm_width - m_armpit_rx - m_button_margin;
			y = 0;
			sgn = -1;
		
		} else if(m_orientation == SWEPT_ORIENTATION::ARM_UP_LEFT) {
			x_progression = m_arm_width + m_armpit_rx + m_button_margin;
			y = m_arm_height - m_bar_height;
			sgn = 1;
			
		} else if(m_orientation == SWEPT_ORIENTATION::ARM_UP_RIGHT) {
			x_progression = m_bar_width - m_arm_width - m_armpit_rx - m_button_margin;
			y = m_arm_height - m_bar_height;
			sgn = -1;
		}
		
		for(int i = 0; i < m_buttons_bar.Size(); ++i) {
			
			LCARS::Button * btn = m_buttons_bar[i];
			
			if(sgn == -1) {	
				x_progression += sgn*btn->GetWidth();
				x_progression += sgn*m_button_margin;
			}
			
			btn->SetPosX(x_progression);
			btn->SetPosY(y);
			btn->SetHeight(m_bar_height);
			
			if(btn->GetWidth() > max_width)
				btn->SetWidth(max_width);
			
			if(sgn == 1) {	
				x_progression += btn->GetWidth();
				x_progression += m_button_margin;
			}
		}
		
		
	} else if(pos == SWEPT_ADDPOS::IN_ARM) {
		
		m_buttons_arm += button;
		
		const int max_height = 200;
		
		int y_progression	= 0;
		int x				= 0;
		int sgn				= 0;
		
		if(m_orientation == SWEPT_ORIENTATION::ARM_DOWN_LEFT) {
			y_progression	= m_shoulder_ry + m_armpit_ry;
			x				= 0;
			sgn				= 1;
			
		} else if(m_orientation == SWEPT_ORIENTATION::ARM_DOWN_RIGHT) {
			y_progression	= m_shoulder_ry + m_armpit_ry;
			x				= m_bar_width - m_arm_width;
			sgn				= 1;
		
		} else if(m_orientation == SWEPT_ORIENTATION::ARM_UP_LEFT) {
			y_progression	= m_arm_height - m_shoulder_ry - m_armpit_ry;
			x				= 0;
			sgn				= -1;
			
		} else if(m_orientation == SWEPT_ORIENTATION::ARM_UP_RIGHT) {
			y_progression	= m_arm_height - m_shoulder_ry - m_armpit_ry;
			x				= m_bar_width - m_arm_width;
			sgn				= -1;
		}
		
		for(int i = 0; i < m_buttons_arm.Size(); ++i) {
			
			LCARS::Button * btn = m_buttons_arm[i];
			
			if(sgn == -1) {
				y_progression += sgn*btn->GetHeight();
				y_progression += sgn*m_button_margin;
			}
			
			btn->SetPosX(x);
			btn->SetPosY(y_progression);
			btn->SetWidth(m_arm_width);
			
			if(btn->GetHeight() > max_height)
				btn->SetHeight(max_height);
			
			if(sgn == 1) {
				y_progression += btn->GetHeight();
				y_progression += m_button_margin;
			}
		}
		
	}

	SetNeedsRepaint(true);
}

void LCARS::Swept::SetExtension(SWEPT_EXTENSION ext) {
	m_extension_type = ext;
	SetNeedsRepaint(true);
}

bool LCARS::Swept::PointInHitbox(int x, int y) {

	for(int i = 0; i < m_buttons_bar.Size(); ++i) {
		if(m_buttons_bar[i]->PointInHitbox(x, y)) {
			std::cout << "OOF\n";
			return true;
		}
	}

	return false;
}

void LCARS::Swept::HandleSDLEvent(XEvent * ev) {
	
}

void LCARS::Swept::OnUnhandledSDLEvent(XEvent * ev) {
	
}

void LCARS::Swept::HandleCMPEvent(CMP_EVT_TYPE type) {
	
}