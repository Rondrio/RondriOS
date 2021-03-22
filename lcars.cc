#include "lcars.hh"

#include <iostream>

#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/XF86keysym.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "lcars_keylistener.hh"
#include "interfaces/test_interface.hh"
#include "simple_list.hh"

XErrorHandler LCARS::s_x_error_handler;

LCARS::LCARS(Display * dpy) {
	m_dpy					= dpy;
	m_screen				= nullptr;
	m_active_lcars_screen	= nullptr;
	m_root					= 0;
	s_x_error_handler		= XSetErrorHandler(LCARS::ErrorHandler);

	m_key_listeners = new smp::list<KeyListener *>();
}

LCARS::~LCARS() {
	delete m_key_listeners;
}

void LCARS::AddKeyListener(KeyListener * kl) {
	m_key_listeners->Add(kl);
}

int LCARS::Checkmod(XKeyEvent ke, KeyListener listener) {
	//TODO: Numlockmask
	KeySym keysym = XkbKeycodeToKeysym(m_dpy, ke.keycode, 0, 0);
	return (keysym == listener.keysym && ke.state == listener.modmask);
}

int LCARS::ErrorHandler(Display * dpy, XErrorEvent * ev) {
	switch(ev->error_code) {
		case BadWindow: {
			std::cerr << "WindowID: " << ev->resourceid << std::endl;
			return 1;
		}

		default: {
			return s_x_error_handler(dpy, ev);
		}
	}

	return 0;
}

int LCARS::HandleEventX(XEvent * ev) {

	switch(ev->type) {

		case KeyPress: {

			XKeyPressedEvent kpe = ev->xkey;

			for(int i = 0; i < m_key_listeners->Size(); ++i) {
				KeyListener * kl = m_key_listeners->Get(i);

				if(Checkmod(kpe, *kl)) {
					kl->func(this, &(kl->arg));
					break;
				}
			}

			break;
		}


		case EnterNotify: {

			XEnterWindowEvent ewe = ev->xcrossing;
			XSetInputFocus(m_dpy, ewe.window, RevertToPointerRoot, CurrentTime);
			break;
		}

		case LeaveNotify: {

			XSetInputFocus(m_dpy, m_root, RevertToPointerRoot, CurrentTime);
			break;
		}

		case DestroyNotify: {

			//TODO: Handle Window Destruction
			XDestroyWindowEvent dwe = ev->xdestroywindow;

			/* REMOVE Window from Interface */
			m_active_lcars_screen->GetInterface()->RemWindow(dwe.window);

			/* REMAP all Windows */
			m_active_lcars_screen->Remap();
			break;
		}

		case MapRequest: {

			XMapRequestEvent mre = ev->xmaprequest;

			/* Set Window off-screen */
			XWindowChanges wc;
			wc.x = 0	+ 10;
			wc.y = 0	+ 10;

			XConfigureWindow(m_dpy, mre.window, CWX, &wc);
			XConfigureWindow(m_dpy, mre.window, CWY, &wc);

			XMapWindow(m_dpy, mre.window);

			/* ADD Window to Interface */
			if(m_active_lcars_screen && m_active_lcars_screen->GetInterface())
				m_active_lcars_screen->GetInterface()->AddWindow(mre.window);
			else
				XDestroyWindow(m_dpy, mre.window);

			/* REMAP all Windows */
			if(m_active_lcars_screen)
				m_active_lcars_screen->Remap();

			break;
		}

		case Expose: {

			if(m_active_lcars_screen)
				m_active_lcars_screen->Draw();
			else
				return 0;

			break;
		}
	}

	return 1;
}

int x_off, y_off;
LCARS_Window * window_to_move;
bool move_window;

int LCARS::HandleEventSDL(SDL_Event * ev) {
	m_active_lcars_screen->DispatchSDLEvents(ev);
	return 1;
}

int LCARS::Init() {

	/* INITIALIZE SDL & XSERVER-CONNECTION */
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
	TTF_Init();


	if(!m_dpy) {
		std::cerr << "XOpenDisplay(): Could not open Display.\n";
		return 0;
	}

	int screen 	= DefaultScreen	(m_dpy);
		m_root	= DefaultRootWindow(m_dpy);

	/* RETRIEVE SCREEN SIZE */
	m_screen = ScreenOfDisplay(m_dpy, screen);

	/* SETUP CURSOR */
	SDL_Cursor * cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	SDL_SetCursor(cursor);
	SDL_ShowCursor(true);

	/* SETUP LCARS_SCREENS */
	m_active_lcars_screen = new LCARS_Screen(m_dpy, 0, 0, m_screen->width, m_screen->height);
	LCARS_Interface * test_interface = new TestInterface(0, 0, m_screen->width, m_screen->height);
	m_active_lcars_screen->SetInterface(test_interface);

	XSelectInput(m_dpy, m_root,
			SubstructureRedirectMask 	|
			KeyPressMask 				|
			EnterWindowMask 			|
			FocusChangeMask				|
			SubstructureNotifyMask
		);

	m_running = false;
	return 1;
}

int LCARS::Run() {

	m_running = true;

	/* SETUP KEYGRABS */
	KeyCode kc;
	for(int i = 0; i < m_key_listeners->Size(); ++i) {
		KeyListener * kl = m_key_listeners->Get(i);

		kc = XKeysymToKeycode(m_dpy, kl->keysym);
		XGrabKey(m_dpy, kc, kl->modmask, m_root, true, GrabModeAsync, GrabModeAsync);
	}

	sigset_t	sig;
	XEvent		ev;
	SDL_Event	sdl_ev;
	while(m_running) {

		/* Poll and Handle X Server Events */
		while(XPending(m_dpy)) {
			XNextEvent(m_dpy, &ev);
			if(!HandleEventX(&ev)) {
				return Exit(0);
			}
		}

		/* Poll and handle SDL Events */
		while(SDL_PollEvent(&sdl_ev)) {
			if(!HandleEventSDL(&sdl_ev)) {
				return Exit(0);
			}
		}

		/* Render Active LCARS_Screen */
		if(m_active_lcars_screen)
			m_active_lcars_screen->Draw();

		SDL_Delay(12);
	}

	return Exit(1);
}

int LCARS::Exit(int exitcode) {

	m_running = false;

	if(m_dpy)
		XCloseDisplay(m_dpy);

	if(m_active_lcars_screen)
		delete m_active_lcars_screen;

	return exitcode;
}

void LCARS::CloseFocusedWindow() {
	LCARS_Interface * intf = m_active_lcars_screen->GetInterface();
	LCARS_Window * wnd = intf->GetFocusedWindow();

	intf->RemWindow(wnd->GetXWindow());
	XDestroyWindow(m_dpy, wnd->GetXWindow());
}

Display * LCARS::GetDisplay() {
	return m_dpy;
}

LCARS_Screen * LCARS::GetScreen() {
	return m_active_lcars_screen;
}
