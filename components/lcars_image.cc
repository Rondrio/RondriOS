#include "lcars_image.hh"

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

LCARS_Image::LCARS_Image(int x, int y, int w, int h, SDL_Surface * img) : LCARS_Component({x, y, w, h}) {
	m_img = img;
	
	m_resize_w = img->w;
	m_resize_h = img->h;
}

LCARS_Image::~LCARS_Image() {

}

void LCARS_Image::Resize(int w, int h) {
	m_resize_w = w;
	m_resize_h = h;
}

void LCARS_Image::Reset() {
	m_resize_w = m_img->w;
	m_resize_h = m_img->h;
}

bool LCARS_Image::PointInHitbox(int x, int y) {
	SDL_Rect abs = GetAbsoluteBounds();
	return x >= abs.x && x <= abs.x + m_bounds.w && y >= abs.y && y <= abs.y + m_bounds.h;
}

void LCARS_Image::Paint(PaintContext * paintctx) {
	paintctx->DrawImage(0, 0, m_resize_w, m_resize_h, m_img);
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
