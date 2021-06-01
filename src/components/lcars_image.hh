#ifndef COMPONENTS_LCARS_IMAGE_HH_
#define COMPONENTS_LCARS_IMAGE_HH_

#include <SDL2/SDL.h>

#include "../lcars_component.hh"

namespace LCARS {

	class Image : public Component {
		private:

			std::string		m_imgfile;
			int				m_resize_w;
			int				m_resize_h;
			
			cairo_surface_t * m_img_surface;

		public:
						Image()					= delete;
						Image(const Image&)	= delete;
						Image(int x, int y, int w, int h, std::string imgfile);
			virtual ~	Image();

			void Resize	(int w, int h);
			void Reset	();

			virtual bool PointInHitbox	(int x, int y)				override;
			virtual void Paint			(PaintContext * paintctx)	override;

			virtual void HandleSDLEvent			(XEvent * ev)	override;
			virtual void OnUnhandledSDLEvent	(XEvent * ev)	override;

			virtual void HandleCMPEvent	(CMP_EVT_TYPE type)			override;

			static void * LoadImage(std::string image_path);

			virtual Image& operator=(const Image&) = delete;
	};
}

#endif /* COMPONENTS_LCARS_IMAGE_HH_ */
