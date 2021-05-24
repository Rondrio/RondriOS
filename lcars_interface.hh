#ifndef LCARS_INTERFACE_HH_
#define LCARS_INTERFACE_HH_

#include <vector>
#include <queue>
#include <X11/Xlib.h>

#include "simple_list.hh"
#include "lcars_screen.hh"
#include "lcars_component.hh"

namespace LCARS {

	/* Forward declarations */
	class Monitor;
	class Component;

	class Interface : public ICP {

		protected:

			volatile bool	m_needs_buffer_repaint;

			SDL_Texture * m_screen_texture;

			smp::list<Component *>	m_components;
			std::queue<Component *>	m_priority_repaints;

			Monitor	*				m_lcars_screen;
			SDL_Rect 						m_bounds;

			Component *				m_cmp_pd_focus;
			Component *				m_cmp_kb_focus;

			double wp;
			double hp;

			Interface(int x, int y, int width, int height);

		public:
							Interface	()							= delete;
							Interface	(const Interface&)	= delete;
			virtual ~		Interface	();

			void Draw(SDL_Renderer * renderer);

			void AttachToScreen(Monitor * screen);

			void AddComponent	(Component * cmp);
			void RemComponent	(Component * cmp);

			void AddPriorityRepaint(Component * cmp);

			void SetNeedsRepaint		(bool b);
			void SetNeedsBufferRepaint	(bool b);

			void SetFocusedWindow(Window w);

			Monitor	*	GetScreen();
			Component	*	ComponentAt(int x, int y);

			void DispatchSDLEvents(SDL_Event * ev);

			virtual void Remap()		= 0;
			virtual void OnEnable()		= 0;
			virtual void OnDisable()	= 0;

			Interface& operator=(const Interface&) = delete;
	};

}

#endif /* LCARS_INTERFACE_HH_ */
