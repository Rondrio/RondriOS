#ifndef COMPONENTS_LCARS_IMAGE_HH_
#define COMPONENTS_LCARS_IMAGE_HH_

#include <SDL2/SDL.h>

#include "../lcars_component.hh"

namespace LCARS {

	class Image : public Component {
		private:
			SDL_Surface *	m_img;
			int				m_resize_w;
			int				m_resize_h;

		public:
						Image()					= delete;
						Image(const Image&)	= delete;
						Image(int x, int y, int w, int h, SDL_Surface * img);
			virtual ~	Image();

			void Resize	(int w, int h);
			void Reset	();

			virtual bool PointInHitbox	(int x, int y)				override;
			virtual void Paint			(PaintContext * paintctx)	override;

			virtual void HandleSDLEvent			(SDL_Event * ev)	override;
			virtual void OnUnhandledSDLEvent	(SDL_Event * ev)	override;

			virtual void HandleCMPEvent	(CMP_EVT_TYPE type)			override;

			static SDL_Surface * LoadImage(std::string image_path);

			virtual Image& operator=(const Image&) = delete;
	};
}

#endif /* COMPONENTS_LCARS_IMAGE_HH_ */
