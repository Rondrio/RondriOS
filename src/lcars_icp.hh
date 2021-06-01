#ifndef LCARS_ICP_HH_
#define LCARS_ICP_HH_

#include "paint_context.hh"

namespace LCARS {

	class ICP {
		protected:
			volatile bool m_needs_repaint = true;
			bool m_is_interface;

		public:
			ICP(bool is_interface) {
				m_is_interface = is_interface;
			}

			virtual ~ICP() {

			}

			bool IsInterface() {
				return m_is_interface;
			}

			virtual void SetNeedsRepaint(bool b) {
				m_needs_repaint = b;
			}

			virtual Rect GetAbsoluteBounds() {
				return {0, 0, 0, 0};
			}

			virtual bool NeedsRepaint() {
				return m_needs_repaint;
			}
	};
}

#endif /* LCARS_ICP_HH_ */
