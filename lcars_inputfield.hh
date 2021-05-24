#ifndef LCARS_INPUTFIELD_HH_
#define LCARS_INPUTFIELD_HH_

#include <SDL2/SDL_ttf.h>
#include <string>

#include "../lcars_component.hh"
#include "../paint_context.hh"

namespace LCARS {

	enum input_filter {
		IF_ALL = 0x0, IF_INT = 0x1, IF_FLOAT = 0x10, IF_ALPHA = 0x100, IF_SPECIAL = 0x1000
	};

	class InputField : public Component {

		private:

			Font *			m_font;

			Text *			m_content_text;
			Text *			m_placeholder_text;
			Text *			m_starred_text;
			Text *			m_shown_text;

			uint16_t		m_caret_position;
			uint64_t		m_inputfilter;

			SDL_Color		m_border_color;
			SDL_Color		m_background_color;

			uint8_t			m_border_width;
			uint8_t			m_padding;
			uint8_t			m_vpadding;

			bool FilterInput(char * text, int len);
			void OnTextInput(SDL_TextInputEvent * ev);

		public:
						InputField()							= delete;
						InputField(const InputField&)	= delete;

						InputField(int x, int y, int w, int h, Font * font, std::string placeholder);
			virtual ~	InputField();

			void	SetPlaceholder(std::string placeholder);
			Text *	GetPlaceholder();

			void	SetText(std::string text);
			Text *	GetText();

			void SetPaddingHorizontal	(uint8_t padding);
			void SetPaddingVertical		(uint8_t padding);

			void SetPasswordMode(bool b);
			void SetInputFilter(uint64_t filter);

			virtual bool PointInHitbox(int x, int y)			override;
			virtual void Paint(PaintContext * paintctx)			override;

			virtual void HandleSDLEvent(SDL_Event * ev)			override;
			virtual void OnUnhandledSDLEvent(SDL_Event * ev)	override;

			virtual void HandleCMPEvent(CMP_EVT_TYPE type)		override;

			virtual InputField& operator=(const InputField&) = delete;
	};
}

#endif