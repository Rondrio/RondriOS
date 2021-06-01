#include "../lcars_interface.hh"

#include "../components/lcars_swept.hh"
#include "../components/lcars_group.hh"
#include "../components/lcars_button_rounded.hh"
#include "../components/lcars_bar.hh"
#include "../lcars_colors.hh"

#include <cstdint>

class HomeInterface : public LCARS::Interface {

    private:
        LCARS::Group *           m_grp_btn_mid;
        LCARS::Group *           m_grp_btn_topleft;
        LCARS::Group *           m_grp_btn_botleft;
        LCARS::Group *           m_grp_dco_topleft;

        LCARS::ButtonRounded *  m_btn_reboot;
        LCARS::ButtonRounded *  m_btn_shutdown;
        LCARS::ButtonRounded *  m_btn_lock;
        LCARS::ButtonRounded *  m_btn_external;
        LCARS::ButtonRounded *  m_btn_systems;
        LCARS::ButtonRounded *  m_btn_database;

        LCARS::ButtonRounded * m_btn_unused1;
        LCARS::ButtonRounded * m_btn_unused2;
        LCARS::ButtonRounded * m_btn_unused3;

        LCARS::ButtonRounded * m_btn_unused4;
        LCARS::ButtonRounded * m_btn_unused5;
        LCARS::ButtonRounded * m_btn_unused6;
        LCARS::ButtonRounded * m_btn_unused7;
        LCARS::ButtonRounded * m_btn_unused8;
        LCARS::ButtonRounded * m_btn_unused9;

        LCARS::Bar * m_sld_dco1;
        LCARS::Bar * m_sld_dco2;
        LCARS::Bar * m_sld_dco3;
        LCARS::Bar * m_sld_dco4;
        LCARS::Bar * m_sld_dco5;
        LCARS::Bar * m_sld_dco6;
        LCARS::Bar * m_sld_dco7;
        LCARS::Bar * m_sld_dco8;
        LCARS::Bar * m_sld_dco9;
        LCARS::Bar * m_sld_dco10;
        LCARS::Bar * m_sld_dco11;
        LCARS::Bar * m_sld_dco12;


        LCARS::Swept * m_topleft_deco;
        LCARS::Swept * m_botleft_deco;


    public:
            HomeInterface(int x, int y, int width, int height) : Interface(x, y, width, height) {
                
                // const int screen_margin_h   = 28;
                // const int screen_margin_w   = 13;

                // const int button_height     = 46;
                // const int button_width      = 82;

                // const int button_padding_h  = 4;

                // const std::string fontpath = "/home/cediw/Downloads/lcars_reference/LCARSGTJ3.ttf";
                
                // /* ------- Button Group Mid  ------ */
                
                // int group_mid_x = 183 + screen_margin_w;
                // m_grp_btn_mid = new LCARS::Group(group_mid_x*wp, screen_margin_h*hp, 1, 6, button_width*wp, 111*hp, 0, button_padding_h*hp);
                // m_grp_btn_mid->SetForceHeight(false);

                // m_btn_reboot = new LCARS::ButtonRounded(0, 0, 10, button_height*hp, "/home/cediw/Downloads/lcars_reference/LCARSGTJ3.ttf", "Reboot");
                // m_btn_reboot->SetRoundLeft(false);
                // m_btn_reboot->SetRoundRight(false);

                // m_btn_shutdown = new LCARS::ButtonRounded(0, 0, 10, button_height*hp, "/home/cediw/Downloads/lcars_reference/LCARSGTJ3.ttf", "Shutdown");
                // m_btn_shutdown->SetRoundLeft(false);
                // m_btn_shutdown->SetRoundRight(false);

                // m_btn_lock = new LCARS::ButtonRounded(0, 0, 10, 400*hp, "/home/cediw/Downloads/lcars_reference/LCARSGTJ3.ttf", "Lock");
                // m_btn_lock->SetRoundLeft(false);
                // m_btn_lock->SetRoundRight(false);

                // m_btn_external = new LCARS::ButtonRounded(0, 0, 10, button_height*hp, "/home/cediw/Downloads/lcars_reference/LCARSGTJ3.ttf", "External");
                // m_btn_external->SetRoundLeft(false);
                // m_btn_external->SetRoundRight(false);

                // m_btn_systems = new LCARS::ButtonRounded(0, 0, 10, button_height*hp, "/home/cediw/Downloads/lcars_reference/LCARSGTJ3.ttf", "Systems");
                // m_btn_systems->SetRoundLeft(false);
                // m_btn_systems->SetRoundRight(false);

                // m_btn_database = new LCARS::ButtonRounded(0, 0, 10, 355*hp, "/home/cediw/Downloads/lcars_reference/LCARSGTJ3.ttf", "Database");
                // m_btn_database->SetRoundLeft(false);
                // m_btn_database->SetRoundRight(false);

                // m_grp_btn_mid->Put(0, 0, m_btn_reboot   );
                // m_grp_btn_mid->Put(0, 1, m_btn_shutdown );
                // m_grp_btn_mid->Put(0, 2, m_btn_lock     );
                // m_grp_btn_mid->Put(0, 3, m_btn_external );
                // m_grp_btn_mid->Put(0, 4, m_btn_systems  );
                // m_grp_btn_mid->Put(0, 5, m_btn_database );

                // /* ------ TOP LEFT DECO ------ */
                // int topdeco_h = 87;
                // int topdeco_w = 110;
                // int topdeco_base_h = 23;
                // int topdeco_arm_space = 310 - topdeco_h;
                // int topdeco_x = screen_margin_w + group_mid_x + button_width + 7;
                // int topdeco_y = screen_margin_h + topdeco_arm_space;
                // m_topleft_deco = new LCARS::BigDeco(topdeco_x*wp, topdeco_y*hp, (LCARS::BigDecoStats){
                //     (uint16_t)(topdeco_base_h*hp),
                //     (uint16_t)(topdeco_w*wp),
                //     (uint16_t)((topdeco_h - 23)*hp),
                //     (uint16_t)(button_width*wp),
                //     (uint16_t)(40*hp),
                //     (uint16_t)(25*hp),
                //     LCARS::BIGDECO_ORIENTATION::ARM_UP_LEFT
                // });
                // m_topleft_deco->SetExtension(LCARS::BIGDECO_EXTENSION::FLAT);

                // /* ------ BOTTOM LEFT DECO ------ */
                // int botdeco_h = 101;
                // int botdeco_w = topdeco_w;
                // int botdeco_base_h = topdeco_base_h;
                // int botdeco_yoff = 10;
                // int botdeco_x = topdeco_x;
                // int botdeco_y = topdeco_y + topdeco_h + botdeco_yoff;
                // m_botleft_deco = new LCARS::BigDeco(botdeco_x*wp, botdeco_y*hp, (LCARS::BigDecoStats){
                //     (uint16_t)(botdeco_base_h*hp),
                //     (uint16_t)(botdeco_w*wp),
                //     (uint16_t)((botdeco_h - botdeco_base_h)*hp),
                //     (uint16_t)(button_width*wp),
                //     (uint16_t)(55*hp),
                //     (uint16_t)(25*hp),
                //     LCARS::BIGDECO_ORIENTATION::ARM_DOWN_LEFT
                // });
                // m_botleft_deco->SetExtension(LCARS::BIGDECO_EXTENSION::FLAT);


                // /* ------ TOP LEFT GROUP ------ */
                // int group_topleft_x = topdeco_x;
                // int group_topleft_y = screen_margin_h;
                // m_grp_btn_topleft = new LCARS::Group(group_topleft_x*wp, group_topleft_y*hp, 1, 3, button_width*wp, 223/3*hp, 0, button_padding_h*hp);
                // m_grp_btn_topleft->SetForceHeight(false);

                // m_btn_unused1 = new LCARS::ButtonRounded(0, 0, 10, button_height*hp, "/home/cediw/Downloads/lcars_reference/LCARSGTJ3.ttf", "0012-A");
                // m_btn_unused1->SetRoundLeft(false);
                // m_btn_unused1->SetRoundRight(false);

                // m_btn_unused2 = new LCARS::ButtonRounded(0, 0, 10, button_height*hp, "/home/cediw/Downloads/lcars_reference/LCARSGTJ3.ttf", "0012-B");
                // m_btn_unused2->SetRoundLeft(false);
                // m_btn_unused2->SetRoundRight(false);

                // m_btn_unused3 = new LCARS::ButtonRounded(0, 0, 10, 121*hp, "/home/cediw/Downloads/lcars_reference/LCARSGTJ3.ttf", "0012-C");
                // m_btn_unused3->SetRoundLeft(false);
                // m_btn_unused3->SetRoundRight(false);

                // m_grp_btn_topleft->Put(0, 0, m_btn_unused1);
                // m_grp_btn_topleft->Put(0, 1, m_btn_unused2);
                // m_grp_btn_topleft->Put(0, 2, m_btn_unused3);


                // /* ------ BOT LEFT GROUP ------ */
                // int group_botleft_x = topdeco_x;
                // int group_botleft_y = botdeco_y + botdeco_h + button_padding_h;

                // m_grp_btn_botleft = new LCARS::Group(group_botleft_x*wp, group_botleft_y*hp, 1, 6, button_width*wp, button_height*hp, 0, button_padding_h*hp);
                // m_grp_btn_botleft->SetForceHeight(false);

                // m_btn_unused4 = new LCARS::ButtonRounded(0, 0, 10, (button_height + 27)*hp, "/home/cediw/Downloads/lcars_reference/LCARSGTJ3.ttf", "0013-A");
                // m_btn_unused4->SetRoundLeft(false);
                // m_btn_unused4->SetRoundRight(false);

                // m_btn_unused5 = new LCARS::ButtonRounded(0, 0, 10, button_height*hp, "/home/cediw/Downloads/lcars_reference/LCARSGTJ3.ttf", "0013-B");
                // m_btn_unused5->SetRoundLeft(false);
                // m_btn_unused5->SetRoundRight(false);

                // m_btn_unused6 = new LCARS::ButtonRounded(0, 0, 10, button_height*hp, "/home/cediw/Downloads/lcars_reference/LCARSGTJ3.ttf", "0013-C");
                // m_btn_unused6->SetRoundLeft(false);
                // m_btn_unused6->SetRoundRight(false);

                // m_btn_unused7 = new LCARS::ButtonRounded(0, 0, 10, 255*hp, "/home/cediw/Downloads/lcars_reference/LCARSGTJ3.ttf", "0013-D");
                // m_btn_unused7->SetRoundLeft(false);
                // m_btn_unused7->SetRoundRight(false);

                // m_btn_unused8 = new LCARS::ButtonRounded(0, 0, 10, button_height*hp, "/home/cediw/Downloads/lcars_reference/LCARSGTJ3.ttf", "0013-E");
                // m_btn_unused8->SetRoundLeft(false);
                // m_btn_unused8->SetRoundRight(false);

                // m_btn_unused9 = new LCARS::ButtonRounded(0, 0, 10, button_height*hp, "/home/cediw/Downloads/lcars_reference/LCARSGTJ3.ttf", "0013-F");
                // m_btn_unused9->SetRoundLeft(false);
                // m_btn_unused9->SetRoundRight(false);

                // m_grp_btn_botleft->Put(0, 0, m_btn_unused4);
                // m_grp_btn_botleft->Put(0, 1, m_btn_unused5);
                // m_grp_btn_botleft->Put(0, 2, m_btn_unused6);
                // m_grp_btn_botleft->Put(0, 3, m_btn_unused7);
                // m_grp_btn_botleft->Put(0, 4, m_btn_unused8);
                // m_grp_btn_botleft->Put(0, 5, m_btn_unused9);

                // /* ------ Solid Deco Group ------ */
                // int group_deco_x = topdeco_x + topdeco_w + 4/wp;
                // int group_deco_y = topdeco_y + topdeco_h - topdeco_base_h;
                // m_grp_dco_topleft = new LCARS::Group(group_deco_x*wp, group_deco_y*hp, 6, 2, 10, topdeco_base_h*hp, 4, botdeco_yoff*hp);
                // m_grp_dco_topleft->SetForceWidth(false);

                // m_sld_dco1  = new LCARS::SolidDeco(0, 0, button_width*wp, 1, COL_GOLD);
                // m_sld_dco2  = new LCARS::SolidDeco(0, 0, button_width*wp, 1, COL_GOLD);
                // m_sld_dco3  = new LCARS::SolidDeco(0, 0, button_width*wp, 1, COL_GOLD);
                // m_sld_dco4  = new LCARS::SolidDeco(0, 0, button_width*wp, 1, COL_GOLD);
                // m_sld_dco5  = new LCARS::SolidDeco(0, 0, button_width*wp, 1, COL_GOLD);
                // m_sld_dco6  = new LCARS::SolidDeco(0, 0, button_width*wp, 1, COL_GOLD);
                // m_sld_dco7  = new LCARS::SolidDeco(0, 0, button_width*wp, 1, COL_GOLD);
                // m_sld_dco8  = new LCARS::SolidDeco(0, 0, button_width*wp, 1, COL_GOLD);
                // m_sld_dco9  = new LCARS::SolidDeco(0, 0, button_width*wp, 1, COL_GOLD);
                // m_sld_dco10 = new LCARS::SolidDeco(0, 0, button_width*wp, 1, COL_GOLD);
                // m_sld_dco11 = new LCARS::SolidDeco(0, 0, button_width*wp, 1, COL_GOLD);
                // m_sld_dco12 = new LCARS::SolidDeco(0, 0, button_width*wp, 1, COL_GOLD);

                // m_grp_dco_topleft->Put(0, 0, m_sld_dco1);
                // m_grp_dco_topleft->Put(1, 0, m_sld_dco2);
                // m_grp_dco_topleft->Put(2, 0, m_sld_dco3);
                // m_grp_dco_topleft->Put(3, 0, m_sld_dco4);
                // m_grp_dco_topleft->Put(4, 0, m_sld_dco5);
                // m_grp_dco_topleft->Put(5, 0, m_sld_dco6);
                // m_grp_dco_topleft->Put(0, 1, m_sld_dco7);
                // m_grp_dco_topleft->Put(1, 1, m_sld_dco8);
                // m_grp_dco_topleft->Put(2, 1, m_sld_dco9);
                // m_grp_dco_topleft->Put(3, 1, m_sld_dco10);
                // m_grp_dco_topleft->Put(4, 1, m_sld_dco11);
                // m_grp_dco_topleft->Put(5, 1, m_sld_dco12);

                // /* ------ Add to Interface ------ */
                // AddComponent(m_grp_btn_mid);
                // AddComponent(m_grp_btn_topleft);
                // AddComponent(m_grp_btn_botleft);
                // AddComponent(m_grp_dco_topleft);
                // AddComponent(m_topleft_deco);
                // AddComponent(m_botleft_deco);
            }

        ~   HomeInterface() {
                delete m_btn_reboot;
                delete m_btn_shutdown;
                delete m_btn_lock;
                delete m_btn_external;
                delete m_btn_systems;
                delete m_btn_database;

                delete m_grp_btn_mid;
                delete m_grp_btn_topleft;
                delete m_grp_btn_botleft;
                delete m_grp_dco_topleft;
                
                delete m_sld_dco1;
                delete m_sld_dco2;
                delete m_sld_dco3;
                delete m_sld_dco4;
                delete m_sld_dco5;
                delete m_sld_dco6;
                delete m_sld_dco7;
                delete m_sld_dco8;
                delete m_sld_dco9;
                delete m_sld_dco10;
                delete m_sld_dco11;
                delete m_sld_dco12;

                delete m_topleft_deco;
            }

        void Remap() override {

        }

		void OnEnable() override {

        }

		void OnDisable() override {

        }

        

};