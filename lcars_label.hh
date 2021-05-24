#ifndef COMPONENTS_LCARS_LABEL_HH_
#define COMPONENTS_LCARS_LABEL_HH_


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "../lcars_component.hh"
#include "../paint_context.hh"
#include "../lcars_text.hh"

namespace LCARS {

	class Label : public Component {

		private:
			Text * m_content_text;

			bool		m_draw_bg;
			SDL_Color	m_bg_color;

			uint16_t	m_padding;
			uint16_t	m_vpadding;

		public:
						Label()					= delete;
						Label(const Label&)	= delete;
						Label(int x, int y, int w, int h, std::string font_path, std::string text);
			virtual ~	Label();

			void		SetFontColor	(SDL_Color c);
			SDL_Color	GetFontColor	();

			SDL_Color	GetBGColor	();
			void		SetBGColor	(SDL_Color c);

			void SetFont(std::string font_path);

			virtual void SetHeight(int h) override;

			virtual bool PointInHitbox(int x, int y)		override;
			virtual void Paint(PaintContext * paintctx)		override;

			virtual void HandleSDLEvent(SDL_Event * ev)			override;
			virtual void OnUnhandledSDLEvent(SDL_Event * ev)	override;

			virtual void HandleCMPEvent(CMP_EVT_TYPE type)		override;

			virtual Label& operator=(const Label&) = delete;
	};
}

#endif /* COMPONENTS_LCARS_LABEL_HH_ */
