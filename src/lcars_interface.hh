#ifndef LCARS_INTERFACE_HH_
#define LCARS_INTERFACE_HH_

#include <vector>
#include <queue>
#include <X11/Xlib.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>

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

			smp::list<Component *>	m_components;
			std::queue<Component *>	m_priority_repaints;

			Monitor	*	m_lcars_screen;
			Rect 		m_bounds;

			cairo_surface_t * m_interface_surface;

			Component *		m_cmp_pd_focus;
			Component *		m_cmp_kb_focus;

			double wp;
			double hp;

			Interface(int x, int y, int width, int height);

		public:
							Interface	()							= delete;
							Interface	(const Interface&)	= delete;
			virtual ~		Interface	();

			void Init(Display * dpy, Window w);

			void Draw(Display * dpy, Window w, Pixmap buffer);

			void AttachToScreen(Monitor * screen);

			void AddComponent	(Component * cmp);
			void RemComponent	(Component * cmp);

			void AddPriorityRepaint(Component * cmp);

			void SetNeedsRepaint		(bool b);
			void SetNeedsBufferRepaint	(bool b);

			void SetFocusedWindow(Window w);

			Monitor	*		GetScreen();
			Component	*	ComponentAt(int x, int y);

			void DispatchSDLEvents(XEvent * ev);

			virtual void Remap()		= 0;
			virtual void OnEnable()		= 0;
			virtual void OnDisable()	= 0;

			Interface& operator=(const Interface&) = delete;
	};

}

#endif /* LCARS_INTERFACE_HH_ */
