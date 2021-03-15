#ifndef LCARS_COMPONENT_HH_
#define LCARS_COMPONENT_HH_

#include <SDL2/SDL.h>

#include "simple_list.hh"

class LCARS_Component {

	protected:
		SDL_Rect						m_bounds;
		smp::list<LCARS_Component *>	m_children;

		bool							m_has_pd_focus;
		bool							m_has_kb_focus;

		LCARS_Component * m_parent = nullptr;

		LCARS_Component(SDL_Rect rect) {
			m_bounds = rect;
			m_has_pd_focus = false;
			m_has_kb_focus = false;
		}

		SDL_Rect GetAbsoluteBounds();

	public:
					LCARS_Component() = delete;
		virtual ~	LCARS_Component();

		virtual bool PointInHitbox(int x, int y) = 0;
		virtual void Paint(SDL_Renderer * renderer) = 0;

		virtual void HandleSDLEvent(SDL_Event * ev) = 0;
		virtual void OnUnhandledEvent(SDL_Event * ev) = 0;

		void	SetPosX(int x);
		int		GetPosX();

		void	SetPosY(int y);
		int		GetPosY();

		void	SetHeight(int h);
		int		GetHeight();

		void	SetWidth(int w);
		int		GetWidth();

		void Draw(SDL_Renderer * renderer);
		LCARS_Component * ComponentAt(int x, int y);

		void AddChild(LCARS_Component * cmp);
		void RemChild(LCARS_Component * cmp);

		bool IsPointInHitbox(int x, int y);

		/* DO NOT USE */
		void SetPDFocus(bool b);
		void SetKBFocus(bool b);
};



#endif /* LCARS_COMPONENT_HH_ */
