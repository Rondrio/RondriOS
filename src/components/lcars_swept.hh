#ifndef COMPONENTS_LCARS_SWEPT_HH_
#define COMPONENTS_LCARS_SWEPT_HH_

#include "../paint_context.hh"
#include "../simple_list.hh"
#include "../lcars_component.hh"
#include "../lcars_button.hh"
#include "lcars_label.hh"

namespace LCARS {

	enum struct SWEPT_ORIENTATION {
		ARM_UP_LEFT, ARM_DOWN_LEFT, ARM_UP_RIGHT, ARM_DOWN_RIGHT
	};

	enum struct SWEPT_EXTENSION {
		ROUNDED, SHARP, FLAT
	};

	enum struct SWEPT_ADDPOS {
		IN_ARM, IN_BAR
	};

	struct BigDecoStats {
		uint16_t bar_height;
		uint16_t bar_width;
		uint16_t arm_height;
		uint16_t arm_width;
		uint16_t main_arc_ry;
		uint16_t small_arc_ry;

		SWEPT_ORIENTATION orientation;
	};

	class Swept : public Component {

		private:
			static constexpr double c_ellipse_factor	= 39.0/46.0;
			static constexpr double c_arc_factor		= 185.0/120.0;

			uint16_t m_arm_height;
			uint16_t m_arm_width;

			uint16_t m_bar_height;
			uint16_t m_bar_width;

			uint16_t m_shoulder_ry;
			uint16_t m_shoulder_rx;

			uint16_t m_armpit_ry;
			uint16_t m_armpit_rx;

			uint16_t m_ext_arc_ry;
			uint16_t m_ext_arc_rx;

			Text *				m_titles[2];
			_color				m_title_colors[2];
			
			smp::list<Button *>	m_buttons_bar;
			smp::list<Button *>	m_buttons_arm;

			uint16_t m_button_margin;
			uint16_t m_label_margin;

			SWEPT_ORIENTATION m_orientation;
			SWEPT_EXTENSION m_extension_type;

			_color m_color;

			Rect CalcBase();
			Rect CalcUnderMainArc();
			Rect CalcUnderMainArc2();
			Rect CalcMainArc();
			Rect CalcArm();
			Rect CalcUnderSmallArc();
			Rect CalcUnderSmallArc2();
			Rect CalcExtensionArc(PaintContext * paintctx);
			Rect CalcSmallArc();
			
		public:
						Swept()						= delete;
						Swept(const Swept&)	= delete;
						Swept(int x, int y, int bar_height, int bar_width, int arm_height, int arm_width, int shoulder_ry, int armpit_ry, SWEPT_ORIENTATION orientation);
			virtual ~	Swept();

			void SetOrientation(SWEPT_ORIENTATION o);


			void SetTitle(std::string text, std::string fontname, SWEPT_ADDPOS label_pos, _color c = {255, 255, 255, 255});

			void SetColor(_color color);

			/**
			 * Adds a Button to the Base of the Decoration.
			* The Button's Bounds will be updated by this Method to fit into the Base.
			*
			* @param button The Button that will be added to the Base.
			* */
			void AddButton(Button * button, SWEPT_ADDPOS pos);

			void SetExtension(SWEPT_EXTENSION ext);

			virtual void Paint(PaintContext * paintctx)		override;
			virtual bool PointInHitbox(int x, int y)		override;
			virtual void HandleSDLEvent(XEvent * ev)		override;
			virtual void OnUnhandledSDLEvent(XEvent * ev)	override;
			virtual void HandleCMPEvent(CMP_EVT_TYPE type)	override;

			virtual Swept& operator=(const Swept&) = delete;
	};
}

#endif /* COMPONENTS_LCARS_BIGDECO_HH_ */
