#include "lcars_image.hh"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

LCARS_Image::LCARS_Image(int x, int y, int w, int h, SDL_Surface * img) : LCARS_Component({x, y, w, h}) {
	m_img = img;
}

LCARS_Image::~LCARS_Image() {

}

bool LCARS_Image::PointInHitbox(int x, int y) {
	SDL_Rect abs = GetAbsoluteBounds();
	return x >= abs.x && x <= abs.x + m_bounds.w && y >= abs.y && y <= abs.y + m_bounds.h;
}

void LCARS_Image::Paint(PaintContext * paintctx) {
	paintctx->DrawImage(0, 0, m_img->w, m_img->h, m_img);
}

void LCARS_Image::HandleSDLEvent(SDL_Event * ev) {

}

void LCARS_Image::OnUnhandledSDLEvent(SDL_Event * ev) {

}

void LCARS_Image::HandleCMPEvent(CMP_EVT_TYPE type) {

}

SDL_Surface * LCARS_Image::LoadImage(std::string image_path) {
	return IMG_Load(image_path.c_str());
}
