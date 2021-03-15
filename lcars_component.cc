#include "lcars_component.hh"

#include <SDL2/SDL.h>

LCARS_Component::~LCARS_Component() {

}

SDL_Rect LCARS_Component::GetAbsoluteBounds() {
	SDL_Rect pos = {0, 0, 0, 0};

	if(m_parent) {
		pos = m_parent->GetAbsoluteBounds();
	}

	pos.x += m_bounds.x;
	pos.y += m_bounds.y;

	return pos;
}

void LCARS_Component::SetPosX(int x) {
	m_bounds.x = x;
}

int LCARS_Component::GetPosX() {
	return m_bounds.x;
}

void LCARS_Component::SetPosY(int y) {
	m_bounds.y = y;
}

int LCARS_Component::GetPosY() {
	return m_bounds.y;
}

void LCARS_Component::SetHeight(int h) {
	m_bounds.h = h;
}

int LCARS_Component::GetHeight() {
	return m_bounds.h;
}

void LCARS_Component::SetWidth(int w) {
	m_bounds.w = w;
}

int LCARS_Component::GetWidth() {
	return m_bounds.w;
}

void LCARS_Component::Draw(SDL_Renderer * renderer) {

	Paint(renderer);

	for(int i = 0; i < m_children.Size(); ++i) {
		m_children[i]->Paint(renderer);
	}
}

LCARS_Component * LCARS_Component::ComponentAt(int x, int y) {
	for(int i = 0; i < m_children.Size(); ++i) {
		if(m_children[i]->ComponentAt(x, y)) {
			return m_children[i];
		}
	}

	if(PointInHitbox(x, y))
		return this;
	return nullptr;
}

void LCARS_Component::AddChild(LCARS_Component * cmp) {
	m_children.Add(cmp);
	cmp->m_parent = this;
}

void LCARS_Component::RemChild(LCARS_Component * cmp) {
	cmp->m_parent = nullptr;
	m_children.Rem(cmp);
}

bool LCARS_Component::IsPointInHitbox(int x, int y) {
	return false;
}

void LCARS_Component::SetPDFocus(bool b) {
	m_has_pd_focus = b;
}

void LCARS_Component::SetKBFocus(bool b) {
	m_has_kb_focus = b;
}
