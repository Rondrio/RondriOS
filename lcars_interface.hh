#ifndef LCARS_INTERFACE_HH_
#define LCARS_INTERFACE_HH_

#include <vector>
#include <queue>
#include <X11/Xlib.h>

#include "simple_list.hh"
#include "lcars_screen.hh"
#include "lcars_component.hh"

/* Forward declarations */
class LCARS_Screen;
class LCARS_Component;

class LCARS_Interface : public LCARS_ICP {

	protected:

		volatile bool	m_window_repaint;

		SDL_Texture * m_screen_texture;

		smp::list<LCARS_Component *> *	m_components;
		std::queue<LCARS_Component *>	m_priority_repaints;

		LCARS_Screen	*				m_lcars_screen;
		SDL_Rect 						m_bounds;

		LCARS_Component *				m_cmp_pd_focus;
		LCARS_Component *				m_cmp_kb_focus;

		LCARS_Interface(int x, int y, int width, int height);

	public:
						LCARS_Interface	()							= delete;
						LCARS_Interface	(const LCARS_Interface&)	= delete;
		virtual ~		LCARS_Interface	();

		void Draw(SDL_Renderer * renderer);

		void AttachToScreen(LCARS_Screen * screen);

		void AddComponent	(LCARS_Component * cmp);
		void RemComponent	(LCARS_Component * cmp);

		void AddPriorityRepaint(LCARS_Component * cmp);

		void SetNeedsRepaint(bool b);
		void SetNeedsWindowRepaint(bool b);

		void SetFocusedWindow(Window w);

		LCARS_Screen	*	GetScreen();
		LCARS_Component	*	ComponentAt(int x, int y);

		void DispatchSDLEvents(SDL_Event * ev);

		virtual void Remap() = 0;

		LCARS_Interface& operator=(const LCARS_Interface&) = delete;
};



#endif /* LCARS_INTERFACE_HH_ */
