#ifndef LCARS_ICP_HH_
#define LCARS_ICP_HH_

#include <SDL2/SDL.h>

class LCARS_ICP {
	protected:
		volatile bool m_needs_repaint = true;

	public:
		virtual ~LCARS_ICP() {

		}

		virtual void SetNeedsRepaint(bool b) {
			m_needs_repaint = b;
		}

		virtual SDL_Rect GetAbsoluteBounds() {
			return {0, 0, 0, 0};
		}

		virtual bool NeedsRepaint() {
			return m_needs_repaint;
		}
};


#endif /* LCARS_ICP_HH_ */
