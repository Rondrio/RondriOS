#include "lcars_soliddeco.hh"

#include "../lcars_component.hh"

LCARS_SolidDeco::LCARS_SolidDeco(int x, int y, int w, int h, SDL_Color color) : LCARS_Component({x, y, w, h}) {
	m_color = color;
}

LCARS_SolidDeco::~LCARS_SolidDeco() {

}

void LCARS_SolidDeco::SetColor(SDL_Color color) {
	m_color = color;
	SetNeedsRepaint(true);
}

SDL_Color *	LCARS_SolidDeco::GetColor() {
	return &m_color;
}

// ------- overrides ---------

bool LCARS_SolidDeco::PointInHitbox(int x, int y) {
	SDL_Rect abs = GetAbsoluteBounds();
	return x >= abs.x && x <= abs.x + m_bounds.w && y >= abs.y && y <= abs.y + m_bounds.h;
}

void LCARS_SolidDeco::Paint(PaintContext * paintctx) {

	paintctx->SetColor(m_color.r, m_color.g, m_color.b, m_color.a);
	
	SDL_Rect r = {0, 0, m_bounds.w, m_bounds.h};
	paintctx->FillRect(&r);
}

void LCARS_SolidDeco::HandleSDLEvent(SDL_Event * ev) {
	
}

void LCARS_SolidDeco::OnUnhandledSDLEvent(SDL_Event * ev) {

}

void LCARS_SolidDeco::HandleCMPEvent(CMP_EVT_TYPE type) {

}
