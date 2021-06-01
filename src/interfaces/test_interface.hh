#ifndef INTERFACES_TEST_INTERFACE_HH_
#define INTERFACES_TEST_INTERFACE_HH_

#include <iostream>

#include "../components/lcars_button_rounded.hh"
#include "../components/lcars_inputfield.hh"
#include "../components/lcars_label.hh"
#include "../components/lcars_swept.hh"
#include "../components/lcars_bar.hh"
#include "../components/lcars_image.hh"
#include "../components/lcars_group.hh"
#include "../lcars_interface.hh"
#include "../lcars_font.hh"
#include "../lcars_component.hh"
#include "../lcars_colors.hh"

static char text[] = "SYSTEMS";
static std::string font_path = "/home/cediw/Downloads/lcars_reference/LCARSGTJ3.ttf";

std::string example_text[] = {"IT JUST WORKS", "THIS IS GREAT", "LMAO"};
int z = 0;

static void exit_func() {
	exit(0);
}

class TestInterface : public LCARS::Interface {

	public:
		LCARS::Bar * topbar_sd;
		LCARS::Swept * frame_bd;
		
		LCARS::Button * shutdown_btr;
		LCARS::Button * reboot_btr;
	
		_font * font;
	
		TestInterface(int x, int y, int width, int height) : LCARS::Interface(x, y, width, height) {
			
			font = LCARS::CreateFontPX("LCARSGTJ3", 25);
			
			topbar_sd = new LCARS::Bar(9.89*wp, 23.15*hp, 973.95*wp, 37.03*hp, COL_GOLD);
			topbar_sd->SetRoundLeft		(true);
			topbar_sd->SetRoundRight	(true);
			topbar_sd->SetBackgroundColor(COL_SKIN);
			
			topbar_sd->SetTitle("Test Label", "LCARSGTJ3");
			
			frame_bd = new LCARS::Swept((int)(173.43*wp), (int)(203.7*hp),
				(int) (26*hp),
				(int) (812.5*wp),
				(int) (775*hp),
				(int) (120*wp),
				(int) (50*hp),
				(int) (40*hp),
				
				LCARS::SWEPT_ORIENTATION::ARM_DOWN_LEFT
			);
			frame_bd->SetColor(COL_SKIN);
			frame_bd->SetTitle("Dummy Title", "LCARSGTJ3", LCARS::SWEPT_ADDPOS::IN_BAR, COL_GOLD);
			frame_bd->SetExtension(LCARS::SWEPT_EXTENSION::SHARP);
			
			
			shutdown_btr	= new LCARS::Button(100, 100, 150, 50, font, "Shutdown");
			reboot_btr		= new LCARS::Button(200, 200, 150, 50, font, "Reboot");
			
			// frame_bd->AddButton(shutdown_btr, LCARS::SWEPT_ADDPOS::IN_BAR);
			// frame_bd->AddButton(reboot_btr, LCARS::SWEPT_ADDPOS::IN_ARM);
			
			topbar_sd->AddButton(shutdown_btr);
			topbar_sd->AddButton(reboot_btr);
			
			this->AddComponent(topbar_sd);
			this->AddComponent(frame_bd);
		}
		
		void Remap() {

			/*int xp[] = { 0, 		m_bounds.w-600, 	0,			m_bounds.w-600};
			int yp[] = {50, 		50,			m_bounds.h-600,	m_bounds.h-600};

			for(int i = 0; i < m_windows.Size(); i++) {
				LCARS_Window * lcars_win = m_windows[i];

				lcars_win->Move(xp[i%4], yp[i%4]);
			}*/
		}

		void OnEnable() {
			
		}

		void OnDisable() {

		}
};


#endif /* INTERFACES_TEST_INTERFACE_HH_ */
