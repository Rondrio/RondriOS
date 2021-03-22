#ifndef COMPONENTS_LCARS_BIGDECO_HH_
#define COMPONENTS_LCARS_BIGDECO_HH_

#include <SDL2/SDL.h>

#include "../paint_context.hh"
#include "../simple_list.hh"
#include "../lcars_component.hh"
#include "../lcars_button.hh"
#include "lcars_label.hh"

enum struct BIGDECO_ORIENTATION {
	ARM_UP_LEFT, ARM_DOWN_LEFT, ARM_UP_RIGHT, ARM_DOWN_RIGHT
};

enum struct LABEL_POSITION {
	IN_ARM, IN_BASE
};

struct BigDecoStats {
	uint16_t base_height;
	uint16_t base_width;
	uint16_t arm_height;
	uint16_t arm_width;
	uint16_t main_arc_ry;
	uint16_t small_arc_ry;

	BIGDECO_ORIENTATION orientation;
};

class LCARS_BigDeco : public LCARS_Component {

	private:
		static constexpr double c_ellipse_factor	= 39.0/46.0;
		static constexpr double c_arc_factor		= 185.0/120.;

		uint16_t m_arm_height;
		uint16_t m_arm_width;

		uint16_t m_base_height;
		uint16_t m_base_width;

		uint16_t m_arc_ry;
		uint16_t m_arc_rx; // = c_arc_factor * m_arc_ry

		uint16_t m_arc_s_ry;
		uint16_t m_arc_s_rx; // =  c_arc_factor * m_arc_s_ry

		uint16_t m_ext_arc_ry;
		uint16_t m_ext_arc_rx;

		LCARS_Label *				m_labels[2];
		int							m_base_label_y_adjustment;
		smp::list<LCARS_Button *>	m_buttons;

		uint16_t m_button_margin;
		uint16_t m_label_margin;

		BIGDECO_ORIENTATION m_orientation;

		SDL_Color m_color;

		SDL_Rect CalcBase();
		SDL_Rect CalcUnderMainArc();
		SDL_Rect CalcMainArc();
		SDL_Rect CalcArm();
		SDL_Rect CalcUnderSmallArc();
		SDL_Rect CalcExtensionArc();
		SDL_Rect CalcSmallArc();
		SDL_Rect CalcButtonsContainer();

	public:
			LCARS_BigDeco() = delete;
			LCARS_BigDeco(int x, int y, BigDecoStats stats);
		~	LCARS_BigDeco();

		void SetOrientation(BIGDECO_ORIENTATION o);

		/**
		 * Sets a Label to one of the two possible Label-Positions.
		 * The Label's Bounds will be adjusted to fit into the Deco.
		 * This Method will do nothing if the given Label-Position is IN_BASE
		 * but there is already at least one Button attached.
		 *
		 * @param label The Label to set.
		 * @param label_pos The Position to where the Label will be set.
		 * */
		void SetLabel(LCARS_Label * label, LABEL_POSITION label_pos, int y_adjustment);

		/**
		 * Adds a Button to the Base of the Decoration.
		 * The Button's Bounds will be updated by this Method to fit into the Base.
		 * This Method will do nothing, if there is already a Label set IN_BASE.
		 *
		 * @param button The Button that will be added to the Base.
		 * */
		void AddButton(LCARS_Button * button);

		void Paint(PaintContext * paintctx) override;

		virtual bool PointInHitbox(int x, int y)			override;
		virtual void HandleSDLEvent(SDL_Event * ev)			override;
		virtual void OnUnhandledSDLEvent(SDL_Event * ev)	override;
		virtual void HandleCMPEvent(CMP_EVT_TYPE type)		override;
};



#endif /* COMPONENTS_LCARS_BIGDECO_HH_ */
