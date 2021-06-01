#include "lcars_inputfield.hh"

#include <string>
#include <cstring>

#include "../lcars_component.hh"

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

LCARS::InputField::InputField(int x, int y, int w, int h, TTF_Font * font, std::string placeholder) : Component({x, y, w, h}) {

	m_font					= font;
	m_placeholder_string	= placeholder;

	m_content_text		= nullptr;
	m_placeholder_text	= nullptr;

	m_content_color		= {255, 255, 255, 255};
	m_placeholder_color	= {100, 100, 100, 255};
	m_border_color		= {255, 255, 255, 255};
	m_background_color	= {  0,   0,   0, 255};

	m_border_width		= 3;
	m_padding			= 5;
	m_vpadding			= 10;
	m_caret_position	= 0;

	m_inputfilter = IF_ALL;
	m_shown_string = &m_content_string;
}

LCARS::InputField::~InputField() {
	if(m_content_text)
		PaintContext::DestroyText(m_content_text);

	if(m_placeholder_text)
		PaintContext::DestroyText(m_placeholder_text);
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

void LCARS::InputField::OnTextInput(SDL_TextInputEvent * ev) {
	
	int textlen = strlen(ev->text);

	if(FilterInput(ev->text, textlen)) {

		if(m_caret_position == m_content_string.size()) {
			m_content_string += ev->text;
			
			for(int i = 0; i < textlen; ++i)
				m_starred_string += '*';

			m_caret_position += textlen;

		} else if(m_caret_position < m_content_string.size()) {

			std::string pre		= m_content_string.substr(0, m_caret_position);
			std::string post	= m_content_string.substr(m_caret_position);

			pre += ev->text;

			for(int i = 0; i < textlen; ++i)
				m_starred_string += '*';

			m_content_string = pre + post;
			m_caret_position += textlen;
		}

		m_content_text = nullptr;
		SetNeedsRepaint(true);
	}
}

void LCARS::InputField::SetPlaceholder(std::string placeholder) {
	m_placeholder_string = placeholder;
	m_placeholder_text = nullptr;
	SetNeedsRepaint(true);
}

std::string * LCARS::InputField::GetPlaceholder() {
	return &m_placeholder_string;
}


void LCARS::InputField::SetText(std::string text) {
	m_content_string = text;
	SetNeedsRepaint(true);
}

std::string * LCARS::InputField::GetText() {
	return &m_content_string;
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
		m_shown_string = &m_starred_string;
	else
		m_shown_string = &m_content_string;

	m_content_text = nullptr;
}

void LCARS::InputField::SetInputFilter(uint64_t filter) {
	m_inputfilter = filter;
}

// ------------ overrides ----------------

bool LCARS::InputField::PointInHitbox(int x, int y) {
	SDL_Rect abs = GetAbsoluteBounds();
	return x >= abs.x && x <= abs.x + m_bounds.w && y >= abs.y && y <= abs.y + m_bounds.h;
}

void LCARS::InputField::Paint(PaintContext * paintctx) {

	SDL_Rect bg_border = {0, 0, m_bounds.w, m_bounds.h};

	paintctx->SetColor(m_background_color.r, m_background_color.g, m_background_color.b, m_background_color.a);
	paintctx->FillRect(&bg_border);

	paintctx->SetColor(m_border_color.r, m_border_color.g, m_border_color.b, m_border_color.a);
	// TODO: Set Stroke Width
	paintctx->DrawRect(&bg_border);

	/* (Re-)Generate the Content Text */
	if(!m_content_text) {
		paintctx->SetFont(m_font);
		paintctx->SetColor(m_content_color.r, m_content_color.g, m_content_color.b, m_content_color.a);
		m_content_text = paintctx->PrepareBlendedText(0, 0, m_shown_string);
	}

	/* Draw the Content */
	/* The if statement is a bit redundant but I'll keep it for saftey reasons. */
	if(m_content_text) {
		SDL_Rect src = {0, 0, m_content_text->bounds.w, m_content_text->bounds.h};
		SDL_Rect dst = {m_padding, m_vpadding, m_content_text->bounds.w, m_content_text->bounds.h};

		paintctx->DrawText(m_content_text, &src, &dst);
	}

	/* Prepare the Placeholder */
	/* This must only be done when the Placeholder is not generated or must be regenereted */
	/* for example because the Placeholder was changed during runtime. */
	if(!m_placeholder_text) {
		paintctx->SetFont(m_font);
		paintctx->SetColor(m_placeholder_color.r, m_placeholder_color.g, m_placeholder_color.b, m_placeholder_color.a);
		m_placeholder_text = paintctx->PrepareBlendedText(0, 0, &m_placeholder_string);
	}

	/* Draw the Placeholder is the Content String is empty. */
	if(m_placeholder_text && m_content_string.size() == 0) {
		SDL_Rect src = {0, 0, m_placeholder_text->bounds.w, m_placeholder_text->bounds.h};
		SDL_Rect dst = {m_padding, m_vpadding, m_placeholder_text->bounds.w, m_placeholder_text->bounds.h};

		paintctx->DrawText(m_placeholder_text, &src, &dst);
	}

	if(m_has_kb_focus) {

		std::string substring = m_shown_string->substr(0, m_caret_position);
		int pixheight, pixlen;
		TTF_SizeText(m_font, substring.c_str(), &pixlen, &pixheight);

		SDL_Rect r2 = {pixlen + m_padding, m_vpadding, 2, pixheight};
		paintctx->SetColor(255, 255, 255, 255);
		paintctx->FillRect(&r2);

	}

}

void LCARS::InputField::HandleSDLEvent(SDL_Event * ev) {
	switch(ev->type) {
		case SDL_TEXTINPUT: {
			OnTextInput(&ev->text);
			break;
		}

		case SDL_KEYDOWN: {
			
			SDL_KeyboardEvent kbe = ev->key;

			if(kbe.keysym.sym == SDLK_LEFT) {
				if(m_caret_position > 0)
					m_caret_position--;

			} else if(kbe.keysym.sym == SDLK_RIGHT) {
				if(m_caret_position < m_content_string.size())
					m_caret_position++;

			} else if(kbe.keysym.sym == SDLK_BACKSPACE) {
				
				if(m_content_string.size() > 0) {
					if(m_caret_position == m_content_string.size()) {
						m_content_string.pop_back();
						m_starred_string.pop_back();

						m_caret_position--;
						m_content_text = nullptr;
					} else if(m_caret_position < m_content_string.size()) {
						
						std::string pre		= m_content_string.substr(0, m_caret_position);
						std::string post	= m_content_string.substr(m_caret_position);

						if(pre.size() > 0) {
							pre.pop_back();
							m_starred_string.pop_back();
							m_caret_position--;

							m_content_string = pre + post;
							m_content_text = nullptr;
						}
					}
				}
			}

			SetNeedsRepaint(true);
			break;
		}
	}
}

void LCARS::InputField::OnUnhandledSDLEvent(SDL_Event * ev) {

}


void LCARS::InputField::HandleCMPEvent(CMP_EVT_TYPE type) {
	switch(type) {
		case CMP_EVT_TYPE::KB_FOCUS: {
			SDL_StartTextInput();
			SetNeedsRepaint(true);
			break;
		}

		case CMP_EVT_TYPE::KB_BLUR: {
			SDL_StopTextInput();
			SetNeedsRepaint(true);
			break;
		}
	}
}