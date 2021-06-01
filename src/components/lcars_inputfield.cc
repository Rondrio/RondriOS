#include "lcars_inputfield.hh"

#include <string>
#include <cstring>

#include "../lcars_component.hh"
#include "../lcars_font.hh"

static bool isdigit(char c) {
	return c >= '0' && c <= '9';
}

static bool isfloat(char c) {
	return isdigit(c) || c == '.';
}

static bool isalpha(char c) {
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static bool isspecial(char c) {
	return (!isdigit(c) && !isfloat(c) && !isalpha(c)) || c == '.';
}

LCARS::InputField::InputField(int x, int y, int w, int h, _font * font, std::string placeholder) : Component((Rect) {x, y, w, h}) {

	m_font = font;

	m_content_text		= "";
	m_placeholder_text	= placeholder;
	m_starred_text		= "";
	m_shown_text		= &m_content_text;

	m_border_color		= {255, 255, 255, 255};
	m_background_color	= {  0,   0,   0, 255};

	m_border_width		= 3;
	m_padding			= 5;
	m_vpadding			= 10;
	m_caret_position	= 0;

	m_inputfilter	= IF_ALL;
}

LCARS::InputField::~InputField() {
	
}

// ------------ methods ------------------

bool LCARS::InputField::FilterInput(char * text, int len) {
	if(!m_inputfilter)
		return true;

	for(int i = 0; i < len; ++i) {

		char c = text[i];

		if(
			!((m_inputfilter & IF_INT)		&& isdigit(c)		||
			(m_inputfilter & IF_FLOAT)		&& isfloat(c)		||
			(m_inputfilter & IF_ALPHA)		&& isalpha(c) 		||
			(m_inputfilter & IF_SPECIAL)	&& isspecial(c))) {
				return false;
		}
	}

	return true;
}

void LCARS::InputField::SetPlaceholder(std::string placeholder) {
	m_placeholder_text = placeholder;
	SetNeedsRepaint(true);
}

std::string LCARS::InputField::GetPlaceholder() {
	return m_placeholder_text;
}


void LCARS::InputField::SetText(std::string text) {
	m_content_text = text;
	SetNeedsRepaint(true);
}

std::string LCARS::InputField::GetText() {
	return m_content_text;
}

void LCARS::InputField::SetPaddingHorizontal(uint8_t padding) {
	m_padding = padding;
	SetNeedsRepaint(true);
}

void LCARS::InputField::SetPaddingVertical(uint8_t padding) {
	m_vpadding = padding;
	SetNeedsRepaint(true);
}

void LCARS::InputField::SetPasswordMode(bool b) {
	if(b)
		m_shown_text = &m_starred_text;
	else
		m_shown_text = &m_content_text;
}

void LCARS::InputField::SetInputFilter(uint64_t filter) {
	m_inputfilter = filter;
}

// ------------ overrides ----------------

bool LCARS::InputField::PointInHitbox(int x, int y) {
	Rect abs = GetAbsoluteBounds();
	return x >= abs.x && x <= abs.x + m_bounds.w && y >= abs.y && y <= abs.y + m_bounds.h;
}

void LCARS::InputField::Paint(PaintContext * paintctx) {

	Rect bg_border = {0, 0, m_bounds.w, m_bounds.h};

	paintctx->SetColor(m_background_color.r, m_background_color.g, m_background_color.b, m_background_color.a);
	paintctx->FillRect(bg_border.x, bg_border.y, bg_border.w, bg_border.h);

	paintctx->SetColor(m_border_color.r, m_border_color.g, m_border_color.b, m_border_color.a);
	// TODO: Set Stroke Width
	paintctx->DrawRect(bg_border.x, bg_border.y, bg_border.w, bg_border.h);
	
	/* Draw Placeholder */
	if(!m_shown_text || m_shown_text->length() == 0) {
		
		m_text.font = m_font;
		m_text.str  = (char *) m_placeholder_text.c_str();
		
		LCARS::TextSize(&m_text);
		
		paintctx->SetColor(100, 100, 100, 255);
		paintctx->DrawText(m_padding, m_vpadding, &m_text);
		
	} else {
		
		/* Draw Text */
		if(m_shown_text && m_shown_text->c_str()) {
		
			m_text.font = m_font;
			m_text.str  = (char *) m_shown_text->c_str();
			
			LCARS::TextSize(&m_text);
			
			paintctx->SetColor(255, 255, 255, 255);
			paintctx->DrawText(m_padding, m_vpadding, &m_text);
			
		}

		/* Draw Caret */
		if(m_has_kb_focus) {

			std::string substring = m_shown_text->substr(0, m_caret_position);

			Text t;
			t.font	= m_font;
			t.str	= (char *) substring.c_str();

			LCARS::TextSize(&t);
			
			paintctx->SetColor(255, 255, 255, 255);
			paintctx->FillRect(t.w + m_padding, m_vpadding, 2, t.h);
		}
	}
}

void LCARS::InputField::HandleSDLEvent(XEvent * ev) {	
	switch(ev->type) {
		case KeyPress: {
			
			XKeyPressedEvent kpe = ev->xkey;
			
			KeySym keysym;
			char str[16];
			XLookupString(&(ev->xkey), str, 16, &keysym, nullptr);
			
			switch(keysym) {
				
				case XK_Left: {
					if(m_caret_position > 0)
			 			m_caret_position--;
					break;
				}
				
				case XK_Right: {
					
					if(m_caret_position < m_content_text.length())
			 			m_caret_position++;
					break;
				}
				
				case XK_BackSpace: {
					
					if(m_content_text.length() != 0) {
					
						if(m_caret_position == m_content_text.length()) {
							m_content_text.pop_back();
							m_starred_text.pop_back();

							m_caret_position--;

						} else if(m_caret_position < m_content_text.length()) {
							
							std::string pre		= m_content_text.substr(0, m_caret_position);
							std::string post	= m_content_text.substr(m_caret_position);

							if(pre.size() > 0) {
								pre.pop_back();
								m_starred_text.pop_back();
								m_caret_position--;

								m_content_text = pre + post;
							}
						}
						
					}
					
					break;
				}
				
				case XK_Return: {
					/* This is here to prevent the Tofu-Character being shown. */
					break;
				}
				
				default: {
					
					int textlen = strlen(str);

					if(FilterInput(str, textlen)) {

						if(m_caret_position >= m_content_text.length()) {
							m_content_text += str;
							
							for(int i = 0; i < textlen; ++i)
								m_starred_text += "*";

							m_caret_position += textlen;

						} else if(m_caret_position < m_content_text.length()) {

							std::string pre		= m_content_text.substr(0, m_caret_position);
							std::string post	= m_content_text.substr(m_caret_position);

							pre += str;

							for(int i = 0; i < textlen; ++i)
								m_starred_text += "*";

							m_content_text = pre + post;
							m_caret_position += textlen;
						}
					}
				}
			}

			SetNeedsRepaint(true);
			break;
		}
	}
}

void LCARS::InputField::OnUnhandledSDLEvent(XEvent * ev) {

}


void LCARS::InputField::HandleCMPEvent(CMP_EVT_TYPE type) {
	switch(type) {
		case CMP_EVT_TYPE::KB_FOCUS: {
			SetNeedsRepaint(true);
			break;
		}

		case CMP_EVT_TYPE::KB_BLUR: {
			SetNeedsRepaint(true);
			break;
		}
	}
}