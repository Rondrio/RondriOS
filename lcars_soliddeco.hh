#ifndef LCARS_SOLIDDECO_HH_
#define LCARS_SOLIDDECO_HH_

#include "../lcars_component.hh"

namespace LCARS {

	class SolidDeco : public Component {

		private:
			SDL_Color m_color;

		public:
						SolidDeco()						= delete;
						SolidDeco(const SolidDeco&)	= delete;
						SolidDeco(int x, int y, int w, int h, SDL_Color color);
			virtual ~	SolidDeco();

			void		SetColor(SDL_Color color);
			SDL_Color *	GetColor();

			virtual bool PointInHitbox(int x, int y)			override;
			virtual void Paint(PaintContext * paintctx)			override;

			virtual void HandleSDLEvent(SDL_Event * ev)			override;
			virtual void OnUnhandledSDLEvent(SDL_Event * ev)	override;

			virtual void HandleCMPEvent(CMP_EVT_TYPE type)		override;

			virtual SolidDeco& operator=(const SolidDeco&) = delete;
	};
}

#endif