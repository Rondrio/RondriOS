#ifndef LCARS_COMPONENT_HH_
#define LCARS_COMPONENT_HH_

#include <SDL2/SDL.h>

#include "simple_list.hh"
#include "paint_context.hh"
#include "lcars_icp.hh"
#include "lcars_interface.hh"

enum CMP_EVT_TYPE {
	KB_FOCUS, KB_BLUR, PD_FOCUS, PD_BLUR
};

enum struct EVENT_TYPE {
	SDL_EVENT, CMP_EVENT
};

struct Event {
	EVENT_TYPE type;
	SDL_Event * sdl_event;
	CMP_EVT_TYPE cmp_event;
};


typedef void (*event_listener)(Event event);

class LCARS_Component : public LCARS_ICP {

	protected:
		SDL_Rect						m_bounds;
		smp::list<LCARS_Component *>	m_children;
		smp::list<event_listener>		m_ev_listeners;

		volatile bool							m_has_pd_focus;
		volatile bool							m_has_kb_focus;

		LCARS_ICP * m_parent = nullptr;
		LCARS_Interface * m_interface = nullptr;

		LCARS_Component(SDL_Rect rect);

	public:
					LCARS_Component() = delete;
		virtual ~	LCARS_Component();

		virtual void		SetNeedsRepaint		(bool b)	override;
		virtual SDL_Rect	GetAbsoluteBounds	()			override;

		virtual bool PointInHitbox(int x, int y) = 0;
		virtual void Paint(PaintContext * paintctx) = 0;

		virtual void HandleSDLEvent(SDL_Event * ev) = 0;
		virtual void OnUnhandledSDLEvent(SDL_Event * ev) = 0;

		virtual void HandleCMPEvent(CMP_EVT_TYPE type) = 0;

		virtual void	SetPosX(int x);
		virtual int		GetPosX();

		virtual void	SetPosY(int y);
		virtual int		GetPosY();

		virtual void	SetHeight(int h);
		virtual int		GetHeight();

		virtual void	SetWidth(int w);
		virtual int		GetWidth();

		void SetInterface(LCARS_Interface * interface);

		void Draw(SDL_Renderer * renderer);
		LCARS_Component * ComponentAt(int x, int y);

		virtual void AddChild(LCARS_Component * cmp);
		virtual void RemChild(LCARS_Component * cmp);

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
		void SetParent(LCARS_ICP * parent);
};



#endif /* LCARS_COMPONENT_HH_ */
