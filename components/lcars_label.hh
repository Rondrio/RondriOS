#ifndef COMPONENTS_LCARS_LABEL_HH_
#define COMPONENTS_LCARS_LABEL_HH_


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "../lcars_component.hh"
#include "../paint_context.hh"

class LCARS_Label : public LCARS_Component {

	private:
		TTF_Font *	m_font;
		SDL_Color	m_font_color;
		text_t *	m_text;
		std::string m_text_string;

		bool		m_draw_bg;
		SDL_Color	m_bg_color;

		uint16_t	m_padding;
		uint16_t	m_vpadding;

	public:
					LCARS_Label() = delete;
					LCARS_Label(int x, int y, int w, int h, TTF_Font * font, std::string text);
		virtual ~	LCARS_Label();

		void		SetFontColor	(SDL_Color c);
		SDL_Color	GetFontColor	();

		SDL_Color	GetBGColor	();
		void		SetBGColor	(SDL_Color c);

		void		SetFont(TTF_Font * font);
		TTF_Font *	GetFont();

		virtual bool PointInHitbox(int x, int y)		override;
		virtual void Paint(PaintContext * paintctx)		override;

		virtual void HandleSDLEvent(SDL_Event * ev)			override;
		virtual void OnUnhandledSDLEvent(SDL_Event * ev)	override;

		virtual void HandleCMPEvent(CMP_EVT_TYPE type)		override;

};


#endif /* COMPONENTS_LCARS_LABEL_HH_ */
