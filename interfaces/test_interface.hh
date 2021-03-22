#ifndef INTERFACES_TEST_INTERFACE_HH_
#define INTERFACES_TEST_INTERFACE_HH_

#include <iostream>
#include <SDL2/SDL_ttf.h>


#include "../components/lcars_button_rounded.hh"
#include "../components/lcars_text_input.hh"
#include "../components/lcars_label.hh"
#include "../components/lcars_bigdeco.hh"
#include "../lcars_interface.hh"

static TTF_Font * font;
static char text[] = "SYSTEMS";

class TestInterface : public LCARS_Interface {

	public:
		TestInterface(int x, int y, int width, int height) : LCARS_Interface(x, y, width, height) {

			font = TTF_OpenFont("/usr/share/fonts/droid/DroidSans.ttf", 18);

			LCARS_Button_Rounded * rounded_btn = new LCARS_Button_Rounded(500, 100, 400, 50, font, text);
			LCARS_Button_Rounded * rounded_btn2 = new LCARS_Button_Rounded(0, 0, 200, 100, font, text);
			LCARS_Button_Rounded * rounded_btn3 = new LCARS_Button_Rounded(0, 0, 200, 100, font, text);
			LCARS_Button_Rounded * rounded_btn4 = new LCARS_Button_Rounded(0, 0, 200, 100, font, text);
			//rounded_btn->AddChild(rounded_btn2);
			//AddComponent(rounded_btn);

			rounded_btn4->SetRoundLeft(false);
			rounded_btn4->SetRoundRight(false);

			rounded_btn3->SetRoundLeft(false);
			rounded_btn3->SetRoundRight(false);

			rounded_btn2->SetRoundLeft(false);
			rounded_btn2->SetRoundRight(false);

			rounded_btn->SetRoundLeft(false);
			rounded_btn->SetRoundRight(false);

			LCARS_Text_Input * tip = new LCARS_Text_Input(500, 500, 200, 40, font, "Authorization");
			//AddComponent(tip);

			LCARS_Label * label = new LCARS_Label(700, 100, 100, 50, "/home/cediw/Downloads/lcars_reference/okuda/Okuda.otf", "LABEL Text");
			label->SetFontColor({0, 0, 0, 255});
			//AddComponent(label);

			LCARS_BigDeco * bd = new LCARS_BigDeco(0, 0, {
					80,
					1600,
					800,
					300,
					50,
					200,
					BIGDECO_ORIENTATION::ARM_UP_RIGHT
			});

			AddComponent(bd);
			//AddComponent(rounded_btn);

			bd->SetLabel(label, LABEL_POSITION::IN_ARM, 10);

			//AddComponent(rounded_btn);
//			bd->AddButton(rounded_btn4);
//			bd->AddButton(rounded_btn3);
//			bd->AddButton(rounded_btn2);
//			bd->AddButton(rounded_btn);

			//rounded_btn2->SetPosX(100);
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
