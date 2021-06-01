#ifndef LCARS_COMPONENT_HH_
#define LCARS_COMPONENT_HH_

#include "simple_list.hh"
#include "paint_context.hh"
#include "lcars_icp.hh"
#include "lcars_interface.hh"

namespace LCARS {

	enum CMP_EVT_TYPE {
		KB_FOCUS, KB_BLUR, PD_FOCUS, PD_BLUR
	};

	enum struct EVENT_TYPE {
		X_EVENT, CMP_EVENT
	};

	struct Event {
		EVENT_TYPE type;
		XEvent * xevent;
		CMP_EVT_TYPE cmp_event;
		bool is_cancelled;
	};

	class Interface;
	typedef void   (*event_listener)(Event * event);
	typedef void * (*component_func)(void * any);

	class Component : public ICP {

		protected:
			Rect						m_bounds;
			smp::list<Component *>		m_children;
			smp::list<event_listener>	m_ev_listeners;

			volatile bool	m_has_pd_focus;
			volatile bool	m_has_kb_focus;

			ICP *		m_parent	= nullptr;
			Interface *	m_interface	= nullptr;

			Component(Rect rect);

		public:
						Component()					= delete;
						Component(const Component&)	= delete;
			virtual ~	Component();

			virtual void	SetNeedsRepaint		(bool b)	override;
			virtual Rect	GetAbsoluteBounds	()			override;

			virtual void	SetPosX(int x);
			virtual int		GetPosX();

			virtual void	SetPosY(int y);
			virtual int		GetPosY();

			virtual void	SetHeight(int h);
			virtual int		GetHeight();

			virtual void	SetWidth(int w);
			virtual int		GetWidth();

			void SetInterface(Interface * interface);

			void Draw(Display * dpy, Window w, cairo_surface_t * interface_surface);
			Component * ComponentAt(int x, int y);

			virtual void AddChild(Component * cmp);
			virtual void RemChild(Component * cmp);

			void AddEventListener(event_listener listener);
			void RemEventListener(event_listener listener);

			/**
			 * Dispatches an Event to all registered Eventlisteners of this Component.
			* This Method is not recursive and only Eventlisteners registered to this
			* Component will receive the Events.
			*
			* @param event The Event (An SDL_Event* or an CMP_EVENT_TYPE).
			* @param event_type The Type of the Event.
			* */
			void DispatchEvent(Event event);

			/* DO NOT USE */
			void SetPDFocus(bool b);
			void SetKBFocus(bool b);
			void SetParent(ICP * parent);

			virtual bool PointInHitbox(int x, int y)		= 0;
			virtual void Paint(PaintContext * paintctx)		= 0;

			virtual void HandleSDLEvent(XEvent * ev)		= 0;
			virtual void OnUnhandledSDLEvent(XEvent * ev)	= 0;

			virtual void HandleCMPEvent(CMP_EVT_TYPE type)	= 0;

			virtual Component& operator=(const Component& rhs) = delete;
	};
}

#endif /* LCARS_COMPONENT_HH_ */
