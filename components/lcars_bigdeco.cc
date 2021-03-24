#include "lcars_bigdeco.hh"

#include "../lcars_colors.hh"

LCARS_BigDeco::LCARS_BigDeco(int x, int y, BigDecoStats stats) : LCARS_Component({x, y, stats.base_width + stats.main_arc_ry*c_arc_factor, (stats.arm_height - stats.main_arc_ry) > stats.base_height + stats.small_arc_ry ? stats.arm_height - stats.main_arc_ry + stats.main_arc_ry : stats.base_height + stats.small_arc_ry + stats.main_arc_ry}) {
	m_arm_height	= stats.arm_height;
	m_arm_width		= stats.arm_width;

	m_base_height	= stats.base_height;
	m_base_width	= stats.base_width;

	m_arc_ry	= stats.main_arc_ry;
	m_arc_rx	= c_arc_factor*m_arc_ry;

	m_arc_s_ry	= stats.small_arc_ry;
	m_arc_s_rx	= c_arc_factor*m_arc_s_ry;

	m_ext_arc_ry = m_base_height/2;
	m_ext_arc_rx = m_ext_arc_ry*c_ellipse_factor;

	m_labels[(int) BIGDECO_LABELPOS::IN_ARM] = nullptr;
	m_labels[(int) BIGDECO_LABELPOS::IN_BASE] = nullptr;
	m_base_label_y_adjustment = 0;

	m_button_margin	= 5;
	m_label_margin	= 5;

	m_orientation = stats.orientation;
	m_color = COL_GOLD;

	m_extension = BIGDECO_EXTENSION::ROUNDED;
}

LCARS_BigDeco::~LCARS_BigDeco() {

}

SDL_Rect LCARS_BigDeco::CalcBase() {

	int width = 0;

	switch(m_extension) {
		case BIGDECO_EXTENSION::FLAT: {
			width = m_base_width-m_arc_rx;
			break;
		}

		case BIGDECO_EXTENSION::ROUNDED: {
			width = m_base_width-m_arc_rx-m_ext_arc_rx;
			break;
		}

		case BIGDECO_EXTENSION::SHARP: {
			width = m_base_width - m_arc_rx - m_base_height/c_ellipse_factor;
			break;
		}
	}

	return {m_arc_rx, 0, width, m_base_height};
}

SDL_Rect LCARS_BigDeco::CalcUnderMainArc() {
	int height = 2*m_arc_ry-m_arm_height;
	height = height < 0 ? 0 : height;

	return {0, m_arm_height, m_arm_width, height};
}

SDL_Rect LCARS_BigDeco::CalcUnderMainArc2() {
	return {m_arm_width, m_base_height, m_arc_rx*2-m_arm_width, m_arc_ry*2+m_base_height};
}

SDL_Rect LCARS_BigDeco::CalcMainArc() {
	return {m_arc_rx, m_arc_ry, m_arc_rx, m_arc_ry};
}

SDL_Rect LCARS_BigDeco::CalcArm() {
	int height = m_arm_height - m_arc_ry;
	height = height > m_base_height + m_arc_s_ry ? height : m_base_height + m_arc_s_ry;

	return {0, m_arc_ry, m_arm_width, height};
}

SDL_Rect LCARS_BigDeco::CalcUnderSmallArc() {
	int arm_main_arc_diff_x = m_arm_width - m_arc_rx;
	int main_arc_base_diff_y = m_arc_ry - m_base_height;

	arm_main_arc_diff_x = arm_main_arc_diff_x > 0 ? arm_main_arc_diff_x : 0;
	main_arc_base_diff_y = main_arc_base_diff_y > 0 ? main_arc_base_diff_y : 0;

	int x = m_arm_width < m_arc_rx ? m_arm_width : m_arc_rx;

	return {x, m_base_height, arm_main_arc_diff_x + m_arc_s_rx, main_arc_base_diff_y + m_arc_s_ry};
}

SDL_Rect LCARS_BigDeco::CalcUnderSmallArc2() {
	return {m_arm_width, m_base_height+m_arc_s_ry, m_arc_s_rx*2, m_arc_s_ry*2 + m_base_height};
}

SDL_Rect LCARS_BigDeco::CalcExtensionArc(PaintContext * paintctx) {

	paintctx->SetColor(m_color.r, m_color.g, m_color.b, m_color.a);

	switch(m_extension) {
		case BIGDECO_EXTENSION::ROUNDED: {
			paintctx->FillEllipse(m_base_width-m_ext_arc_rx, m_base_height/2, m_ext_arc_rx, m_ext_arc_ry);
			break;
		}

		case BIGDECO_EXTENSION::FLAT: {
			break;
		}

		case BIGDECO_EXTENSION::SHARP: {

			paintctx->FillEllipse(m_base_width - m_base_height/c_ellipse_factor, m_base_height, m_base_height/c_ellipse_factor, m_base_height);

			SDL_Rect black = {m_base_width - m_base_height/c_ellipse_factor*2, m_base_height, m_base_height/c_ellipse_factor*2, m_base_height/c_ellipse_factor};
			paintctx->SetColor(0, 0, 0, 255);
			paintctx->FillRect(&black);
			break;
		}
	}

	return {};
}

SDL_Rect LCARS_BigDeco::CalcSmallArc() {
	return {m_arm_width + m_arc_s_rx, m_base_height+m_arc_s_ry, m_arc_s_rx, m_arc_s_ry};
}

SDL_Rect LCARS_BigDeco::CalcButtonsContainer() {

	int count = m_buttons.Size();
	int container_width = m_base_width - m_arc_s_rx - m_arm_width - m_base_height*2 + m_button_margin*(count + 1);
	int container_height = m_base_height;

	int height = m_arm_height - m_arc_ry;
		height = height > m_base_height + m_arc_s_ry ? height : m_base_height + m_arc_s_ry;

	switch(m_orientation) {
		case BIGDECO_ORIENTATION::ARM_UP_LEFT: {
			return {m_arm_width+m_arc_s_rx, height + m_arc_ry - m_base_height, container_width, container_height};
		}

		case BIGDECO_ORIENTATION::ARM_DOWN_LEFT: {
			return {m_arm_width+m_arc_s_rx, 0, container_width, container_height};
		}

		case BIGDECO_ORIENTATION::ARM_UP_RIGHT: {

			return {m_base_height, m_arm_height, container_width, container_height};
		}

		case BIGDECO_ORIENTATION::ARM_DOWN_RIGHT: {

			return {m_base_height, 0, container_width, container_height};
		}
	}

	return {};
}

void LCARS_BigDeco::Paint(PaintContext * paintctx) {


	paintctx->SetColor(m_color.r, m_color.g, m_color.b, m_color.a);

	SDL_Rect main_base = CalcBase();
	paintctx->FillRect(&main_base);



	SDL_Rect main_arc = CalcMainArc();
	paintctx->FillEllipse(main_arc.x, main_arc.y, main_arc.w, main_arc.h);

	paintctx->SetColor(0, 0, 0, 255);

	SDL_Rect fill_under_main_arc2 = CalcUnderMainArc2();
	paintctx->FillRect(&fill_under_main_arc2);

	paintctx->SetColor(m_color.r, m_color.g, m_color.b, m_color.a);

	SDL_Rect arm_rect = CalcArm();
	paintctx->FillRect(&arm_rect);

	SDL_Rect fill_small_arc = CalcUnderSmallArc();
	paintctx->FillRect(&fill_small_arc);

	paintctx->SetColor(0, 0, 0, 255);
	SDL_Rect fill_small_arc2 = CalcUnderSmallArc2();
	paintctx->FillRect(&fill_small_arc2);

	SDL_Rect small_arc = CalcSmallArc();
	paintctx->FillEllipse(small_arc.x, small_arc.y, small_arc.w, small_arc.h);

	CalcExtensionArc(paintctx);

	paintctx->SetColor(0, 0, 0, 255);
	for(int i = 0; i < m_buttons.Size(); ++i) {
		LCARS_Button * btn = m_buttons[i];
		SDL_Rect fill = {btn->GetPosX() - m_button_margin, 0, btn->GetWidth() + m_button_margin*2, btn->GetHeight()};
		paintctx->FillRect(&fill);
	}

	LCARS_Label * lbl = m_labels[(int) BIGDECO_LABELPOS::IN_BASE];
	if(lbl) {
		SDL_Rect r = {lbl->GetPosX() - m_label_margin, 0, lbl->GetWidth() + m_label_margin*3, m_base_height};
		paintctx->FillRect(&r);
	}

	switch(m_orientation) {
		case BIGDECO_ORIENTATION::ARM_DOWN_RIGHT: {
			paintctx->FlipHorizontally();
			break;
		}
		case BIGDECO_ORIENTATION::ARM_UP_RIGHT: {
			paintctx->FlipHorizontally();
			paintctx->FlipVertically();
			break;
		}
		case BIGDECO_ORIENTATION::ARM_UP_LEFT: {
			paintctx->FlipVertically();
			break;
		}
		case BIGDECO_ORIENTATION::ARM_DOWN_LEFT:
			break;
	}
}

void LCARS_BigDeco::SetOrientation(BIGDECO_ORIENTATION o) {
	m_orientation = o;
	SetNeedsRepaint(true);
}

void LCARS_BigDeco::SetLabel(LCARS_Label * label, BIGDECO_LABELPOS label_pos, int y_adjustment) {

	if(label_pos == BIGDECO_LABELPOS::IN_BASE && m_buttons.Size() > 0)
		return;

	m_labels[(int) label_pos] = label;

	if(label_pos == BIGDECO_LABELPOS::IN_BASE) {

		/* Save this for black Outline Calculations */
		m_base_label_y_adjustment = y_adjustment;

		SDL_Rect container = CalcButtonsContainer();
		label->SetHeight(m_base_height + y_adjustment);

		if(m_orientation == BIGDECO_ORIENTATION::ARM_DOWN_RIGHT || m_orientation == BIGDECO_ORIENTATION::ARM_UP_RIGHT)
			label->SetPosX(container.x);
		else
			label->SetPosX(container.x + container.w - label->GetWidth());

		label->SetPosY(container.y - y_adjustment);

	} else {

		if(m_orientation == BIGDECO_ORIENTATION::ARM_DOWN_LEFT || m_orientation == BIGDECO_ORIENTATION::ARM_UP_LEFT)
			label->SetPosX(m_arm_width - label->GetWidth() - m_label_margin*3);
		else
			label->SetPosX(m_base_width - label->GetWidth()/2);

		if(m_orientation == BIGDECO_ORIENTATION::ARM_DOWN_LEFT || m_orientation == BIGDECO_ORIENTATION::ARM_DOWN_RIGHT)
			label->SetPosY(m_arm_height - label->GetHeight() + y_adjustment);
		else
			label->SetPosY(m_label_margin + y_adjustment);
	}

	AddChild(label);
	SetNeedsRepaint(true);
}


void LCARS_BigDeco::AddButton(LCARS_Button * button) {

	if(m_labels[(int) BIGDECO_LABELPOS::IN_BASE])
		return;

	m_buttons.Add(button);
	AddChild(button);

	const int max_width = 200;

	int count = m_buttons.Size();
	SDL_Rect container = CalcButtonsContainer();

	for(int i = 0; i < count; ++i) {
		LCARS_Button * btn = m_buttons[i];

		int width = (container.w - (count+ 1)*m_button_margin)/count;
		width = width < max_width ? width : max_width;

		btn->SetHeight(container.h);
		btn->SetWidth(width);
		btn->SetPosX(container.x + m_button_margin*(i+1) + i*width);
		btn->SetPosY(container.y);
	}

	SetNeedsRepaint(true);
}

void LCARS_BigDeco::SetExtension(BIGDECO_EXTENSION ext) {
	m_extension = ext;
	SetNeedsRepaint(true);
}

bool LCARS_BigDeco::PointInHitbox(int x, int y) {

	for(int i = 0; i < m_buttons.Size(); ++i) {
		if(m_buttons[i]->PointInHitbox(x, y)) {
			std::cout << "OOF\n";
			return true;
		}
	}

	return false;
}

void LCARS_BigDeco::HandleSDLEvent(SDL_Event * ev) {
	//TODO nothing
}

void LCARS_BigDeco::OnUnhandledSDLEvent(SDL_Event * ev) {
	//TODO nothing
}

void LCARS_BigDeco::HandleCMPEvent(CMP_EVT_TYPE type) {
	//TODO nothing
}
