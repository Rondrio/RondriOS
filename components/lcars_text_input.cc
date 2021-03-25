#include "lcars_text_input.hh"

#include <iostream>
#include <string>
#include <SDL2/SDL_ttf.h>

LCARS_Text_Input::LCARS_Text_Input(int16_t x, int16_t y, int16_t w, int16_t h, TTF_Font * font, std::string placeholder) : LCARS_Component({x, y, w, h}) {
	m_font = font;
	m_font_color	= {255, 255, 255, 255};
	m_border_width	= 5;
	m_padding		= 5;
	m_vpadding		= 10;

	m_inputfilter = IF_ALL;

	m_repaint_placeholder_text = true;
	m_placeholder_string		= placeholder;
	m_placeholder_text			= nullptr;
	m_shown_text				= &m_text_string;

	m_text		= nullptr;
	m_caret_pos	= 0;

	m_bg_color			= {  0,   0,   0, 255};
	m_border_color		= {255, 255, 255, 255};
	m_placeholder_color	= {100, 100, 100, 255};
}

LCARS_Text_Input::~LCARS_Text_Input() {
	if(m_placeholder_text)
		PaintContext::DestroyText(m_placeholder_text);

	if(m_text)
		PaintContext::DestroyText(m_text);
}

void LCARS_Text_Input::SetBorderWidth(uint8_t w) {
	m_border_width = w;
	SetNeedsRepaint(true);
}

void LCARS_Text_Input::SetBorderColor(SDL_Color c) {
	m_border_color = c;
	SetNeedsRepaint(true);
}

void LCARS_Text_Input::DrawTextContent(PaintContext * ctx) {
	if(m_text)
		ctx->DestroyText(m_text);

	ctx->SetFont(m_font);
	ctx->SetColor(m_font_color.r, m_font_color.g, m_font_color.b, m_font_color.a);
	
	m_text = ctx->PrepareBlendedText(0, 0,(char *) m_shown_text->c_str());

	if(m_text) {

		SDL_Rect src = {0, 0, m_text->bounds.w, m_text->bounds.h};
		SDL_Rect dst = {m_padding, m_vpadding, m_text->bounds.w, m_text->bounds.h};

		ctx->DrawText(m_text, &src, &dst);
	}
}

void LCARS_Text_Input::DrawPlaceholder(PaintContext * ctx) {
	ctx->SetColor(m_placeholder_color.r, m_placeholder_color.g, m_placeholder_color.b, m_placeholder_color.a);
	ctx->SetFont(m_font);

	m_placeholder_text = ctx->PrepareBlendedText(0, 0, (char *) m_placeholder_string.c_str());

	if(m_text_string.length() <= 0) {

		SDL_Rect src = {0, 0, m_placeholder_text->bounds.w, m_placeholder_text->bounds.h};
		SDL_Rect dst = {m_padding, m_vpadding, m_placeholder_text->bounds.w, m_placeholder_text->bounds.h};

		ctx->DrawText(m_placeholder_text, &src, &dst);
	}
}

void LCARS_Text_Input::SetPlaceholderText(std::string text) {
	m_placeholder_string		= text;
	m_repaint_placeholder_text	= true;
	SetNeedsRepaint(true);
}

void LCARS_Text_Input::SetPasswordField(bool b) {

	if(b)
		m_shown_text = &m_starred_text;
	else
		m_shown_text = &m_text_string;

	SetNeedsRepaint(true);
}

void LCARS_Text_Input::SetInputFilter(uint16_t filter) {
	m_inputfilter |= filter;
}

void LCARS_Text_Input::SetPlaceholderColor(SDL_Color c) {
	m_placeholder_color = c;
	SetNeedsRepaint(true);
}

void LCARS_Text_Input::SetText(std::string text) {
	m_text_string = text;
	SetNeedsRepaint(true);
}

std::string LCARS_Text_Input::GetText() {
	return m_text_string;
}

void LCARS_Text_Input::SetPadding(uint8_t padding) {
	m_padding = padding;
	SetNeedsRepaint(true);
}

void LCARS_Text_Input::SetVerticalPadding(uint8_t padding) {
	m_vpadding = padding;
	SetNeedsRepaint(true);
}


bool LCARS_Text_Input::PointInHitbox(int x, int y) {
	SDL_Rect abs = GetAbsoluteBounds();
	return x >= abs.x && x <= abs.x + m_bounds.w && y >= abs.y && y <= abs.y + m_bounds.h;
}

void LCARS_Text_Input::Paint(PaintContext * paintctx) {

	/* Draw Background */
	paintctx->SetColor(m_bg_color.r, m_bg_color.g, m_bg_color.b, m_bg_color.a);
	SDL_Rect r = {0, 0, m_bounds.w, m_bounds.h};
	paintctx->FillRect(&r);

	DrawTextContent(paintctx);

	if(m_repaint_placeholder_text)
		DrawPlaceholder(paintctx);

	/* Get the actual xy-Position of where the Caret should be on screen */
	std::string substring = m_shown_text->substr(0, m_caret_pos);
	int pixheight, pixlen;
	TTF_SizeText(m_font, substring.c_str(), &pixlen, &pixheight);

	/* Draw Caret to the Screen */
	if(m_has_kb_focus) {
		SDL_Rect r2 = {pixlen + m_padding, m_vpadding, 2, pixheight};
		paintctx->SetColor(255, 255, 255, 255);
		paintctx->FillRect(&r2);
	}

	/* Draw Border */
	paintctx->SetColor(m_border_color.r, m_border_color.g, m_border_color.b, m_border_color.a);
	paintctx->DrawRect(&r);
}

void LCARS_Text_Input::HandleSDLEvent(SDL_Event * ev) {
	switch(ev->type) {
		case SDL_MOUSEBUTTONDOWN:
			// TODO: Calculate the new Cursor position.
			break;
		case SDL_KEYDOWN: {

			SDL_KeyboardEvent * kbe = &(ev->key);

			switch(kbe->keysym.sym) {
				case SDLK_RETURN: {

					if(m_caret_pos >= m_text_string.length())
						m_text_string += '\n';
					else {

						std::string pre_str		= m_text_string.substr(0, m_caret_pos);
						std::string post_str	= m_text_string.substr(m_caret_pos);

						pre_str += '\n';

						m_text_string = pre_str + post_str;
					}

					m_caret_pos++;
					SetNeedsRepaint(true);
					break;
				}

				case SDLK_BACKSPACE: {
					if(m_caret_pos >= m_text_string.length())
						m_text_string = m_text_string.substr(0, m_text_string.length() - 1);
					else {

						std::string pre_str		= m_text_string.substr(0, m_caret_pos-1);
						std::string post_str	= m_text_string.substr(m_caret_pos);

						m_text_string = pre_str + post_str;
					}

					if(m_starred_text.size() > 0)
						m_starred_text.pop_back();
					
					m_caret_pos--;
					SetNeedsRepaint(true);
					break;
				}

				case SDLK_LEFT:{

					// Left Bounds Check
					if(m_caret_pos > 0)
						m_caret_pos--;
					else
						m_caret_pos = 0;

					uint64_t text_len = m_text_string.length() - 1;

					// Right Bounds Check
					if(m_caret_pos > text_len)
						m_caret_pos = text_len;

					SetNeedsRepaint(true);
					break;
				}

				case SDLK_UP: {
					//if(m_caret_pos < m_text_string.length() - 1)

					break;
				}

				case SDLK_RIGHT: {

					uint64_t text_len = m_text_string.length() - 1;

					if(m_caret_pos <= text_len)
						m_caret_pos++;
					else
						m_caret_pos = text_len + 1;

					SetNeedsRepaint(true);
					break;
				}

				case SDLK_DOWN: {

					break;
				}
			}

			break;
		}

		case SDL_TEXTINPUT: {

			SDL_TextInputEvent * tip = &(ev->text);
			std::string new_text_string;

			if(m_caret_pos >= m_text_string.length()) {
				new_text_string = m_text_string + tip->text;
			} else {

				//TODO: Ausallokieren in einer Funktion (wird bei SDLK_RETURN wiederverwendet)
				std::string pre_str		= m_text_string.substr(0, m_caret_pos);
				std::string post_str	= m_text_string.substr(m_caret_pos);

				pre_str += tip->text;
				new_text_string = pre_str + post_str;
			}

			int h, w;
			TTF_SizeText(m_font, new_text_string.c_str(), &w, &h);

			//if(w < m_bounds.w - m_padding) {

				int len = strlen(tip->text);

				m_text_string	 = new_text_string;
				m_caret_pos		+= len;
			//}

			for(int i = 0; i < len; ++i)
				m_starred_text += '*';

			SetNeedsRepaint(true);
			break;
		}
		case SDL_TEXTEDITING: {

			break;
		}
	}
}

void LCARS_Text_Input::HandleCMPEvent(CMP_EVT_TYPE type) {
	switch(type) {
		case KB_FOCUS:
			SDL_StartTextInput();
			SetNeedsRepaint(true);
			break;
		case KB_BLUR:
			SDL_StopTextInput();
			SetNeedsRepaint(true);
			break;
		default:
		{}
	}
}

void LCARS_Text_Input::OnUnhandledSDLEvent(SDL_Event * ev) {

}
