#ifndef INTERFACES_TEST_INTERFACE_HH_
#define INTERFACES_TEST_INTERFACE_HH_

#include <iostream>

#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/XF86keysym.h>

#include "../components/lcars_button_rounded.hh"
#include "../lcars_interface.hh"

class TestInterface : public LCARS_Interface {

	public:
		TestInterface(int x, int y, int width, int height) : LCARS_Interface(x, y, width, height) {
			LCARS_Button_Rounded * rounded_btn = new LCARS_Button_Rounded(100, 100, 200, 50);
			LCARS_Button_Rounded * rounded_btn2 = new LCARS_Button_Rounded(50, 50, 40, 40);
			rounded_btn->AddChild(rounded_btn2);
			AddComponent(rounded_btn);
		}

		void Remap() {

			/*int xp[] = { 0, 		m_bounds.w-600, 	0,			m_bounds.w-600};
			int yp[] = {50, 		50,			m_bounds.h-600,	m_bounds.h-600};

			for(int i = 0; i < m_windows.Size(); i++) {
				LCARS_Window * lcars_win = m_windows[i];

				lcars_win->Move(xp[i%4], yp[i%4]);
			}*/
		}

};


#endif /* INTERFACES_TEST_INTERFACE_HH_ */
