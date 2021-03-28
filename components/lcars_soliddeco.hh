#ifndef LCARS_SOLIDDECO_HH_
#define LCARS_SOLIDDECO_HH_

#include "../lcars_component.hh"

class LCARS_SolidDeco : public LCARS_Component {

	private:
		SDL_Color m_color;

	public:
					LCARS_SolidDeco()						= delete;
					LCARS_SolidDeco(const LCARS_SolidDeco&)	= delete;
					LCARS_SolidDeco(int x, int y, int w, int h, SDL_Color color);
		virtual ~	LCARS_SolidDeco();

		void		SetColor(SDL_Color color);
		SDL_Color *	GetColor();

		virtual bool PointInHitbox(int x, int y)			override;
		virtual void Paint(PaintContext * paintctx)			override;

		virtual void HandleSDLEvent(SDL_Event * ev)			override;
		virtual void OnUnhandledSDLEvent(SDL_Event * ev)	override;

		virtual void HandleCMPEvent(CMP_EVT_TYPE type)		override;

		virtual LCARS_SolidDeco& operator=(const LCARS_SolidDeco&) = delete;
};

#endif