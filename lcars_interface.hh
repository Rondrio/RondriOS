#ifndef LCARS_INTERFACE_HH_
#define LCARS_INTERFACE_HH_

#include <vector>
#include <queue>
#include <X11/Xlib.h>

#include "simple_list.hh"
#include "lcars_screen.hh"
#include "lcars_window.hh"
#include "lcars_component.hh"

/* Forward declaration of the LCARS_Screen */
class LCARS_Screen;
class LCARS_Window;
class LCARS_Component;

typedef struct window_move_t window_move_t;

struct window_move_t {
	LCARS_Window *	window;			// The Window that was clicked.
	bool			moving_enabled;	// True if .window moving is enabled (mouse button down)
	int				x_offset;
	int				y_offset;
};

class LCARS_Interface : public LCARS_ICP {

	private:
		window_move_t	m_window_move;

	protected:

		volatile bool	m_window_repaint;

		SDL_Texture * m_screen_texture;
		SDL_Texture * m_window_texture;

		smp::list<LCARS_Window *>	 *	m_windows;
		smp::list<LCARS_Component *> *	m_components;

		std::queue<LCARS_Component *>	m_priority_repaints;

		LCARS_Screen	*				m_lcars_screen;
		SDL_Rect 						m_bounds;

		LCARS_Component *				m_cmp_pd_focus;
		LCARS_Component *				m_cmp_kb_focus;

		LCARS_Window	*				m_wnd_focus;

		LCARS_Interface(int x, int y, int width, int height);

	public:
		virtual ~		LCARS_Interface	() {

		}

		virtual void 	Remap			() = 0;

		void Draw			(SDL_Renderer * renderer);

		void AttachToScreen(LCARS_Screen * screen);

		void AddWindow	(Window w);
		void RemWindow	(Window w);

		void AddComponent	(LCARS_Component * cmp);
		void RemComponent	(LCARS_Component * cmp);

		void AddPriorityRepaint(LCARS_Component * cmp);

		void SetNeedsRepaint(bool b);
		void SetNeedsWindowRepaint(bool b);

		void				SetFocusedWindow	(Window w);
		void				SetFocusedWindow	(LCARS_Window * w);
		LCARS_Window	*	GetFocusedWindow	();


		LCARS_Window	* IsOnLCARS_Window	(int x, int y);
		LCARS_Component	* ComponentAt		(int x, int y);

		void DispatchSDLEvents(SDL_Event * ev);
};



#endif /* LCARS_INTERFACE_HH_ */
