#include "lcars_screen.hh"

#include <X11/Xlib.h>
#include <X11/cursorfont.h>

#include "time.h"
#include "lcars_timer.hh"
#include "lcars_interface.hh"

LCARS::Monitor::Monitor(Display * dpy, int x, int y, int width, int height) {
	m_dpy		= dpy;
	m_width		= width;
	m_height	= height;
	
	m_xwindow = -1;

	m_interface = nullptr;
}

LCARS::Monitor::~Monitor() {
	XDestroyWindow(m_dpy, m_xwindow);
	XFreePixmap(m_dpy, m_pixmap_buffer);
}

void LCARS::Monitor::Start() {
	m_xwindow = XCreateSimpleWindow(
		m_dpy, RootWindow(m_dpy, 0), 0, 0, m_width, m_height, 0, BlackPixel(m_dpy, 0), BlackPixel(m_dpy, 0)
	);
	
	XMapWindow(m_dpy, m_xwindow);
	
	//m_xwindow = RootWindow(m_dpy, 0);
	
	XSelectInput(m_dpy, m_xwindow, ButtonPressMask|ButtonReleaseMask|KeyPressMask|KeyReleaseMask|PointerMotionMask);
	
	Cursor c = XCreateFontCursor(m_dpy, XC_arrow);
	XDefineCursor(m_dpy, m_xwindow, c);
	
	XWindowAttributes wa;
	XGetWindowAttributes(m_dpy, m_xwindow, &wa);
	
	m_pixmap_buffer = XCreatePixmap(m_dpy, m_xwindow, wa.width, wa.height, wa.depth);
}

void LCARS::Monitor::Draw() {
	
	if(m_interface) {
		m_interface->Draw(m_dpy, m_xwindow, m_pixmap_buffer);
	}

	if(m_timers.Size() > 0) {

		time_t ti;
		time(&ti);

		for(int i = 0; i < m_timers.Size(); ++i) {
			lcarstimer_t * timer = m_timers[i];
			bool decreased = false;

			if(timer->repeats != 0 && (ti >= timer->endtime)) {
				timer->repeats--;
				timer->action_func();

				decreased = true;
			}

			if(timer->repeats == 0) {
				m_timers -= timer;
				free(timer);

			} else if(decreased)
				timer->endtime += timer->runtime;
		}
	}
}

void LCARS::Monitor::Remap() {
	m_interface->Remap();
}

void LCARS::Monitor::SetInterface(Interface * interface) {
	m_interface = interface;
	m_interface->AttachToScreen(this);
	
	m_interface->Init(m_dpy, m_xwindow);
}

LCARS::Interface * LCARS::Monitor::GetInterface() {
	return m_interface;
}

Display * LCARS::Monitor::GetDisplay() {
	return m_dpy;
}

void LCARS::Monitor::AddTimer(uint64_t runtime, int repeats, timer_action action_func) {

	time_t ti;
	time(&ti);

	lcarstimer_t * timer = (lcarstimer_t *) malloc(sizeof(lcarstimer_t));
	timer->endtime		= runtime + ti;
	timer->runtime		= runtime;
	timer->repeats		= repeats;
	timer->action_func	= action_func;

	m_timers += timer;
}