#ifndef INTERFACES_TEST_INTERFACE_HH_
#define INTERFACES_TEST_INTERFACE_HH_

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


#include "../components/lcars_button_rounded.hh"
#include "../components/lcars_text_input.hh"
#include "../components/lcars_label.hh"
#include "../components/lcars_bigdeco.hh"
#include "../components/lcars_image.hh"
#include "../lcars_interface.hh"

static TTF_Font * font;
static char text[] = "SYSTEMS";
static std::string font_path = "/home/cediw/Downloads/lcars_reference/LCARSGTJ3.ttf";

class TestInterface : public LCARS_Interface {

	public:
		TestInterface(int x, int y, int width, int height) : LCARS_Interface(x, y, width, height) {

			font = TTF_OpenFont(font_path.c_str(), 18);

			LCARS_Button_Rounded * btn1 = new LCARS_Button_Rounded(20, 20 + 400 + 5, 200, 80, font, "Reboot");
			btn1->SetRoundLeft(false);
			btn1->SetRoundRight(false);

			LCARS_Button_Rounded * btn2 = new LCARS_Button_Rounded(20, 20 + 400 + 80 + 10, 200, 60, font, "Shutdown");
			btn2->SetRoundLeft(false);
			btn2->SetRoundRight(false);

			SDL_Surface * base_img = LCARS_Image::LoadImage("/home/cediw/Downloads/lcars_reference/sf-logo.jpg");
			LCARS_Image * img1 = new LCARS_Image(500, 500, base_img->w, base_img->h, base_img);

			LCARS_BigDeco * topdeco = new LCARS_BigDeco(20, 20, {
					60,
					2560 - 40,
					400,
					200,
					60,
					40,

					BIGDECO_ORIENTATION::ARM_DOWN_LEFT
			});

			LCARS_BigDeco * botdeco = new LCARS_BigDeco(20, 400+80+60+35, {
					60,
					2560 - 40,
					1440 - (20 + 400 + 80 + 60 + 20),
					200,
					60,
					40,

					BIGDECO_ORIENTATION::ARM_UP_LEFT
			});

			LCARS_Label * lbl1 = new LCARS_Label(0, 0, 10, 35, font_path, "LCARS PRE-ALPHA");
			LCARS_Label * lbl2 = new LCARS_Label(0, 0, 10, 10, font_path, "ENGINEERING");

			topdeco->SetLabel(lbl1, BIGDECO_LABELPOS::IN_ARM, -10);
			topdeco->SetLabel(lbl2, BIGDECO_LABELPOS::IN_BASE, 5);



			topdeco->SetExtension(BIGDECO_EXTENSION::ROUNDED);
//			topdeco->AddButton(btn1);
//			topdeco->AddButton(btn2);

			AddComponent(topdeco);
			AddComponent(botdeco);
			AddComponent(btn1);
			AddComponent(btn2);

			AddComponent(img1);
		}

		void Remap() {

			/*int xp[] = { 0, 		m_bounds.w-600, 	0,			m_bounds.w-600};
			int yp[] = {50, 		50,			m_bounds.h-600,	m_bounds.h-600};

			for(int i = 0; i < m_windows.Size(); i++) {
				LCARS_Window * lcars_win = m_windows[i];

				lcars_win->Move(xp[i%4], yp[i%4]);
			}*/
		}

		LCARS_Interface& operator=(LCARS_Interface& rhs) {
			exit(0);
			return rhs;
		}

};


#endif /* INTERFACES_TEST_INTERFACE_HH_ */
