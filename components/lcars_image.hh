#ifndef COMPONENTS_LCARS_IMAGE_HH_
#define COMPONENTS_LCARS_IMAGE_HH_

#include <SDL2/SDL.h>


#include "../lcars_component.hh"

class LCARS_Image : public LCARS_Component {
	private:
		SDL_Surface *	m_img;
		int				m_resize_w;
		int				m_resize_h;

	public:
			LCARS_Image(int x, int y, int w, int h, SDL_Surface * img);
		virtual ~	LCARS_Image();

		void Resize	(int w, int h);
		void Reset	();

		virtual bool PointInHitbox	(int x, int y)				override;
		virtual void Paint			(PaintContext * paintctx)	override;

		virtual void HandleSDLEvent			(SDL_Event * ev)	override;
		virtual void OnUnhandledSDLEvent	(SDL_Event * ev)	override;

		virtual void HandleCMPEvent	(CMP_EVT_TYPE type)			override;

		static SDL_Surface * LoadImage(std::string image_path);
};



#endif /* COMPONENTS_LCARS_IMAGE_HH_ */
