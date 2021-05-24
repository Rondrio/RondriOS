#include "lcars.hh"

#include <iostream>

#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/XF86keysym.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "lcars_keylistener.hh"

#include "interfaces/test_interface.hh"
#include "interfaces/home_interface.cpp"

#include "simple_list.hh"
#include "lcars_error.hh"
#include "lcars_component.hh"

static bool grabbed = false, resize = false;
static int grab_x, grab_y, grab_off_x, grab_off_y;
static Window grabbed_frame;

XErrorHandler LCARS::LCARS::s_x_error_handler;

LCARS::LCARS::LCARS(Display * dpy) {
	m_dpy					= dpy;
	m_screen				= nullptr;
	m_active_lcars_screen	= nullptr;
	m_root					= 0;
	s_x_error_handler		= XSetErrorHandler(LCARS::ErrorHandler);

	m_key_listeners = new smp::list<KeyListener *>();
}

LCARS::LCARS::~LCARS() {
	delete m_key_listeners;
}

void LCARS::LCARS::AddKeyListener(KeyListener * kl) {
	m_key_listeners->Add(kl);
}

int LCARS::LCARS::Checkmod(XKeyEvent ke, KeyListener listener) {
	//TODO: Numlockmask
	KeySym keysym = XkbKeycodeToKeysym(m_dpy, ke.keycode, 0, 0);
	return (keysym == listener.keysym && ke.state == listener.modmask);
}

int LCARS::LCARS::ErrorHandler(Display * dpy, XErrorEvent * ev) {
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

int LCARS::LCARS::HandleEventX(XEvent * ev) {

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
			m_focused_window = ewe.window;

			break;
		}

		case LeaveNotify: {

			XSetInputFocus(m_dpy, m_root, RevertToPointerRoot, CurrentTime);
			m_focused_window = m_root;

			break;
		}

		case DestroyNotify: {

			//TODO: Handle Window Destruction
			XDestroyWindowEvent dwe = ev->xdestroywindow;
			m_focused_window = m_root;

			UnframeWindow(dwe.window);

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

			XWindowAttributes wa;
			XGetWindowAttributes(m_dpy, mre.window, &wa);

			wc.x = wa.x;
			wc.y = wa.y;
			wc.width	= wa.width;
			wc.height	= wa.height;

			if(wa.width <= 50)
				wc.width = 50;

			if(wa.height <= 50)
				wc.height = 50;

			XConfigureWindow(m_dpy, mre.window, CWX, &wc);
			XConfigureWindow(m_dpy, mre.window, CWY, &wc);
			XConfigureWindow(m_dpy, mre.window, CWWidth, &wc);
			XConfigureWindow(m_dpy, mre.window, CWHeight, &wc);

			XMapWindow(m_dpy, mre.window);

			/* ADD Window to Interface */
			// if(m_active_lcars_screen && m_active_lcars_screen->GetInterface())
			// 	m_active_lcars_screen->GetInterface()->AddWindow(mre.window);
			// else
			// 	XDestroyWindow(m_dpy, mre.window);

			FrameWindow(mre.window);

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

		case ButtonPress: {

			XButtonPressedEvent bpe = ev->xbutton;

			XSetInputFocus(m_dpy, bpe.window, RevertToPointerRoot, CurrentTime);
			m_focused_window = bpe.window;

			grab_x = bpe.x_root;
			grab_y = bpe.y_root;
			grab_off_x = bpe.x;
			grab_off_y = bpe.y;
			grabbed_frame = bpe.window;

			if(bpe.button == Button1) {
				grabbed = true;
			
			} else if(bpe.button = Button3) {
				resize = true;
			}

			XRaiseWindow(m_dpy, bpe.window);

			GetScreen()->GetInterface()->SetNeedsBufferRepaint(true);
			break;
		}

		case ButtonRelease: {
			grabbed = false;
			resize = false;
			break;
		}

		case MotionNotify: {

			XMotionEvent me = ev->xmotion;
			
			if(grabbed)
				XMoveWindow(m_dpy, grabbed_frame, me.x_root - grab_off_x, me.y_root - grab_off_y);

			else if(resize) {
				XWindowAttributes wa;
				XGetWindowAttributes(m_dpy, grabbed_frame, &wa);

				int w = me.x_root - wa.x;
				int h = me.y_root - wa.y;

				if(w > 0 && h > 0)
					XResizeWindow(m_dpy, me.window, w, h);
			}
			
			m_active_lcars_screen->GetInterface()->SetNeedsBufferRepaint(true);

			break;
		}

		case ConfigureRequest: {
			
			XConfigureRequestEvent cre = ev->xconfigurerequest;
			
			XWindowChanges wc;
			wc.x = cre.x;
			wc.y = cre.y;
			wc.height = cre.height;
			wc.width = cre.width;
			wc.sibling = cre.above;
			wc.stack_mode = cre.detail;

			XConfigureWindow(m_dpy, cre.window, cre.value_mask, &wc);
			break;
		}
	}

	return 1;
}

int LCARS::LCARS::HandleEventSDL(SDL_Event * ev) {
	m_active_lcars_screen->DispatchSDLEvents(ev);
	return 1;
}

int LCARS::LCARS::GetWindowCount() {
	return m_windows.Size();
}

void LCARS::LCARS::FrameWindow(Window w) {
	XWindowAttributes x_window_attrs;
	XGetWindowAttributes(m_dpy, w, &x_window_attrs);

	// 3. Select events on frame.
	XSelectInput(
		m_dpy,
		w,
		SubstructureRedirectMask | SubstructureNotifyMask);

	// 4. Add client to save set, so that it will be restored and kept alive if we
	// crash.
	XAddToSaveSet(m_dpy, w);

	XGrabButton(
		m_dpy,
		Button1,
		Mod4Mask,
		w,
		true,
		ButtonPressMask | ButtonReleaseMask | ButtonMotionMask,
		GrabModeAsync,
		GrabModeAsync,
		None,
		None
	);

	XGrabButton(
		m_dpy,
		Button3,
		Mod4Mask,
		w,
		true,
		ButtonPressMask | ButtonReleaseMask | ButtonMotionMask,
		GrabModeAsync,
		GrabModeAsync,
		None,
		None
	);

	m_windows += w;
}

void LCARS::LCARS::UnframeWindow(Window w) {

	m_windows -= w;
	XDestroyWindow(m_dpy, w);
	m_active_lcars_screen->GetInterface()->SetNeedsBufferRepaint(true);
}

Window LCARS::LCARS::GetFocusedWindow() {
	return m_focused_window;
}

int LCARS::LCARS::Init() {

	/* INITIALIZE SDL & XSERVER-CONNECTION */
	
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
		LOGEX("FATAL ERROR in LCARS::Init()", "Could not initialize SDL", SDL_GetError());
	}
	
	if(TTF_Init() == -1) {
		LOGEX("FATAL ERROR in LCARS::Init()", "Could not initialize TTF", TTF_GetError());
	}

	if(IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG) == 0) {
		LOGEX("FATAL ERROR in LCARS::Init()", "Could not initialize IMG", IMG_GetError());
	}

	int screen 	= DefaultScreen	(m_dpy);
		m_root	= DefaultRootWindow(m_dpy);

	/* RETRIEVE SCREEN SIZE */
	m_screen = ScreenOfDisplay(m_dpy, screen);

	/* SETUP CURSOR */
	SDL_Cursor * cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);

	if(cursor) {
		SDL_SetCursor(cursor);
		SDL_ShowCursor(true);

	} else {
		LOGEX("ERROR in LCARS::Init()", "Could not create System-Cursor", SDL_GetError());
	}

	/* SETUP LCARS_SCREENS */
	m_active_lcars_screen = new Monitor(m_dpy, 0, 0, m_screen->width, m_screen->height);
	// TestInterface * test_interface = new TestInterface(0, 0, m_screen->width, m_screen->height);
	// m_active_lcars_screen->SetInterface(test_interface);
	// test_interface->Init();

	HomeInterface * home_interface = new HomeInterface(0, 0, m_screen->width, m_screen->height);
	m_active_lcars_screen->SetInterface(home_interface);

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

int LCARS::LCARS::Run() {

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

int LCARS::LCARS::Exit(int exitcode) {

	m_running = false;

	if(m_dpy)
		XCloseDisplay(m_dpy);

	if(m_active_lcars_screen)
		delete m_active_lcars_screen;

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return exitcode;
}

Display * LCARS::LCARS::GetDisplay() {
	return m_dpy;
}

LCARS::Monitor * LCARS::LCARS::GetScreen() {
	return m_active_lcars_screen;
}
