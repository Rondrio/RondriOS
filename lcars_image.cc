#include "lcars_image.hh"

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

LCARS::Image::Image(int x, int y, int w, int h, SDL_Surface * img) : Component({x, y, w, h}) {
	m_img = img;
	
	m_resize_w = img->w;
	m_resize_h = img->h;
}

LCARS::Image::~Image() {

}

void LCARS::Image::Resize(int w, int h) {
	m_resize_w = w;
	m_resize_h = h;
}

void LCARS::Image::Reset() {
	m_resize_w = m_img->w;
	m_resize_h = m_img->h;
}

bool LCARS::Image::PointInHitbox(int x, int y) {
	SDL_Rect abs = GetAbsoluteBounds();
	return x >= abs.x && x <= abs.x + m_bounds.w && y >= abs.y && y <= abs.y + m_bounds.h;
}

void LCARS::Image::Paint(PaintContext * paintctx) {
	paintctx->DrawImage(0, 0, m_resize_w, m_resize_h, m_img);
}

void LCARS::Image::HandleSDLEvent(SDL_Event * ev) {

}

void LCARS::Image::OnUnhandledSDLEvent(SDL_Event * ev) {

}

void LCARS::Image::HandleCMPEvent(CMP_EVT_TYPE type) {
	
}

SDL_Surface * LCARS::Image::LoadImage(std::string image_path) {
	return IMG_Load(image_path.c_str());
}
