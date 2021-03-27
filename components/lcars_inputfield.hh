#ifndef _LCARS_INPUTFIELD_HH_
#define _LCARS_INPUTFIELD_HH_

#include <SDL2/SDL_ttf.h>
#include <string>

#include "../lcars_component.hh"
#include "../paint_context.hh"

class LCARS_InputField : public LCARS_Component {

	private:

		TTF_Font *		m_font;

		uint16_t		m_caret_position;

		std::string		m_content_string;
		std::string		m_placeholder_string;

		text_t	*		m_content_text;
		text_t	*		m_placeholder_text;

		SDL_Color		m_content_color;
		SDL_Color		m_placeholder_color;
		SDL_Color		m_border_color;
		SDL_Color		m_background_color;

		uint8_t			m_border_width;
		uint8_t			m_padding;
		uint8_t			m_vpadding;

		void OnTextInput(SDL_TextInputEvent * ev);


	public:
					LCARS_InputField()							= delete;
					LCARS_InputField(const LCARS_InputField&)	= delete;

					LCARS_InputField(int x, int y, int w, int h, TTF_Font * font, std::string placeholder);
		virtual ~	LCARS_InputField();

		void			SetPlaceholder(std::string placeholder);
		std::string *	GetPlaceholder();

		void			SetText(std::string text);
		std::string *	GetText();

		void SetPaddingHorizontal	(uint8_t padding);
		void SetPaddingVertical		(uint8_t padding);

		virtual bool PointInHitbox(int x, int y)			override;
		virtual void Paint(PaintContext * paintctx)			override;

		virtual void HandleSDLEvent(SDL_Event * ev)			override;
		virtual void OnUnhandledSDLEvent(SDL_Event * ev)	override;

		virtual void HandleCMPEvent(CMP_EVT_TYPE type)		override;

		virtual LCARS_InputField& operator=(const LCARS_InputField&) = delete;
};

#endif