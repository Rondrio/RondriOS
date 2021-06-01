#ifndef LCARS_INPUTFIELD_HH_
#define LCARS_INPUTFIELD_HH_

#include <string>

#include "../lcars_component.hh"
#include "../paint_context.hh"

namespace LCARS {

	enum input_filter {
		IF_ALL = 0x0, IF_INT = 0x1, IF_FLOAT = 0x10, IF_ALPHA = 0x100, IF_SPECIAL = 0x1000
	};

	class InputField : public Component {

		private:
			_font *	m_font;
			Text	m_text;
			
			std::string	m_content_text;
			std::string	m_placeholder_text;
			std::string	m_starred_text;
			std::string	* m_shown_text;

			uint16_t m_caret_position;
			uint64_t m_inputfilter;

			_color m_border_color;
			_color m_background_color;

			uint8_t	m_border_width;
			uint8_t	m_padding;
			uint8_t	m_vpadding;

			bool FilterInput(char * text, int len);

		public:
						InputField()					= delete;
						InputField(const InputField&)	= delete;

						InputField(int x, int y, int w, int h, _font * font, std::string placeholder);
			virtual ~	InputField();

			void		SetPlaceholder(std::string placeholder);
			std::string	GetPlaceholder();

			void		SetText(std::string text);
			std::string	GetText();

			void SetPaddingHorizontal	(uint8_t padding);
			void SetPaddingVertical		(uint8_t padding);

			void SetPasswordMode(bool b);
			void SetInputFilter(uint64_t filter);

			virtual bool PointInHitbox(int x, int y)		override;
			virtual void Paint(PaintContext * paintctx)		override;

			virtual void HandleSDLEvent(XEvent * ev)		override;
			virtual void OnUnhandledSDLEvent(XEvent * ev)	override;

			virtual void HandleCMPEvent(CMP_EVT_TYPE type)	override;

			virtual InputField& operator=(const InputField&) = delete;
	};
}

#endif