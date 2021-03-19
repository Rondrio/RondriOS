#ifndef COMPONENTS_LCARS_TEXT_INPUT_HH_
#define COMPONENTS_LCARS_TEXT_INPUT_HH_

#include <string>
#include <SDL2/SDL_ttf.h>

#include "../paint_context.hh"
#include "../lcars_component.hh"


class LCARS_Text_Input : public LCARS_Component {

	private:
		TTF_Font *	m_font;
		SDL_Color	m_font_color;

		SDL_Color	m_bg_color;

		uint8_t		m_border_width;
		SDL_Color	m_border_color;

		bool		m_repaint_placeholder_text;
		std::string	m_placeholder_string;
		text_t *	m_placeholder_text;
		SDL_Color	m_placeholder_color;

		uint64_t		m_caret_pos;
		std::string 	m_text_string;
		text_t *		m_text;

		uint8_t			m_padding;
		uint8_t			m_vpadding;

	protected:
		void DrawCursor();
		void DrawTextContent(PaintContext * ctx);
		void DrawPlaceholder(PaintContext * ctx);

	public:
		LCARS_Text_Input() = delete;
		LCARS_Text_Input(int16_t x, int16_t y, int16_t w, int16_t h, TTF_Font * font, std::string placeholder);

		virtual ~LCARS_Text_Input();

		void SetBorderWidth(uint8_t w);
		void SetBorderColor(SDL_Color c);

		void SetPlaceholderText(std::string text);
		void SetPlaceholderColor(SDL_Color c);

		void		SetText(std::string text);
		std::string	GetText();

		void SetPadding(uint8_t padding);
		void SetVerticalPadding(uint8_t padding);

		virtual bool PointInHitbox(int x, int y)		override;
		virtual void Paint(PaintContext * paintctx)		override;

		virtual void HandleSDLEvent(SDL_Event * ev)			override;
		virtual void OnUnhandledSDLEvent(SDL_Event * ev)	override;

		virtual void HandleCMPEvent(CMP_EVT_TYPE type)		override;

};



#endif /* COMPONENTS_LCARS_TEXT_INPUT_HH_ */
