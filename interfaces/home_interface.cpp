#include "../lcars_interface.hh"

#include "../components/lcars_bigdeco.hh"
#include "../components/lcars_group.hh"
#include "../components/lcars_button_rounded.hh"

#include <cstdint>

class HomeInterface : public LCARS_Interface {

    private:
        TTF_Font *              m_font;
        LCARS_Group *           m_grp_btn_mid;

        LCARS_Button_Rounded *  m_btn_reboot;
        LCARS_Button_Rounded *  m_btn_shutdown;
        LCARS_Button_Rounded *  m_btn_lock;
        LCARS_Button_Rounded *  m_btn_external;
        LCARS_Button_Rounded *  m_btn_systems;
        LCARS_Button_Rounded *  m_btn_database;

        LCARS_BigDeco * m_topleft_deco;
        LCARS_BigDeco * m_botleft_deco;


    public:
            HomeInterface(int x, int y, int width, int height) : LCARS_Interface(x, y, width, height) {
                
                const int screen_margin_h   = 28;
                const int screen_margin_w   = 13;

                const int button_height     = 46;
                const int button_width      = 82;

                const int button_padding_h  = 4;

                m_font = TTF_OpenFont("/home/cediw/Downloads/lcars_reference/LCARSGTJ3.ttf", 26);
                
                /* ------- Button Group Mid  ------ */
                
                int group_x = 183;
                m_grp_btn_mid = new LCARS_Group((group_x + screen_margin_w)*wp, screen_margin_h*hp, 1, 6, button_width*wp, 111*hp, 0, button_padding_h*hp);
                m_grp_btn_mid->SetForceHeight(false);

                m_btn_reboot = new LCARS_Button_Rounded(0, 0, 10, 174*hp, m_font, "Reboot");
                m_btn_reboot->SetRoundLeft(false);
                m_btn_reboot->SetRoundRight(false);

                m_btn_shutdown = new LCARS_Button_Rounded(0, 0, 10, 100*hp, m_font, "Shutdown");
                m_btn_shutdown->SetRoundLeft(false);
                m_btn_shutdown->SetRoundRight(false);

                m_btn_lock = new LCARS_Button_Rounded(0, 0, 10, 250*hp, m_font, "Lock");
                m_btn_lock->SetRoundLeft(false);
                m_btn_lock->SetRoundRight(false);

                m_btn_external = new LCARS_Button_Rounded(0, 0, 10, 100*hp, m_font, "External");
                m_btn_external->SetRoundLeft(false);
                m_btn_external->SetRoundRight(false);

                m_btn_systems = new LCARS_Button_Rounded(0, 0, 10, 85*hp, m_font, "Systems");
                m_btn_systems->SetRoundLeft(false);
                m_btn_systems->SetRoundRight(false);

                m_btn_database = new LCARS_Button_Rounded(0, 0, 10, 230*hp, m_font, "Database");
                m_btn_database->SetRoundLeft(false);
                m_btn_database->SetRoundRight(false);

                m_grp_btn_mid->Put(0, 0, m_btn_reboot   );
                m_grp_btn_mid->Put(0, 1, m_btn_shutdown );
                m_grp_btn_mid->Put(0, 2, m_btn_lock     );
                m_grp_btn_mid->Put(0, 3, m_btn_external );
                m_grp_btn_mid->Put(0, 4, m_btn_systems  );
                m_grp_btn_mid->Put(0, 5, m_btn_database );

                /* ------ TOP LEFT DECO ------ */
                int topdeco_h = 87;
                int topdeco_arm_space = 310 - topdeco_h;
                int topdeco_x = screen_margin_w + group_x + button_width + 7;
                int topdeco_y = screen_margin_h + topdeco_arm_space;
                m_topleft_deco = new LCARS_BigDeco(topdeco_x*wp, topdeco_y*hp, (BigDecoStats){
                    (uint16_t)(23*hp),
                    (uint16_t)(100*wp),
                    (uint16_t)((topdeco_h - 23)*hp),
                    (uint16_t)(65*wp),
                    (uint16_t)(40*hp),
                    (uint16_t)(25*hp),
                    BIGDECO_ORIENTATION::ARM_UP_LEFT
                });
                m_topleft_deco->SetExtension(BIGDECO_EXTENSION::FLAT);

                /* ------ BOTTOM LEFT DECO ------ */
                int botdeco_h = 130;
                int botdeco_x = topdeco_x;
                int botdeco_y = topdeco_y + topdeco_h + 10;
                m_botleft_deco = new LCARS_BigDeco(botdeco_x*wp, botdeco_y*hp, (BigDecoStats){
                    (uint16_t)(21*hp),
                    (uint16_t)(100*wp),
                    (uint16_t)(70*hp),
                    (uint16_t)(65*wp),
                    (uint16_t)(40*hp),
                    (uint16_t)(25*hp),
                    BIGDECO_ORIENTATION::ARM_DOWN_LEFT
                });
                m_botleft_deco->SetExtension(BIGDECO_EXTENSION::FLAT);

                /* Add to Interface */
                AddComponent(m_grp_btn_mid);
                AddComponent(m_topleft_deco);
                AddComponent(m_botleft_deco);
            }

        ~   HomeInterface() {
                delete m_btn_reboot;
                delete m_btn_shutdown;
                delete m_btn_lock;
                delete m_btn_external;
                delete m_btn_systems;
                delete m_btn_database;

                delete m_grp_btn_mid;

                delete m_topleft_deco;

                TTF_CloseFont(m_font);
            }

        void Remap() override {

        }

		void OnEnable() override {

        }

		void OnDisable() override {

        }

        

};