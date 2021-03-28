#ifndef LCARS_GROUP_HH_
#define LCARS_GROUP_HH_

#include "../lcars_button.hh"
#include "../lcars_component.hh"


class LCARS_Group : public LCARS_Component {

    private:
        LCARS_Component ** m_grid;

        bool m_force_height;
        bool m_force_width;

        int m_cols;
        int m_rows;

        int m_width;
        int m_height;

        int m_col_padding;
        int m_row_padding;

    void Arrange(int gridx, int gridy, LCARS_Component * cmp);

    /**
     * Rearranges all Components that were Put() in this group.
     * Basically this Method calls Arrange() on every registered Component.
     * 
    */
    void Rearrange();

    public:
                    LCARS_Group()                   = delete;
                    LCARS_Group(const LCARS_Group&) = delete;
                    LCARS_Group(int x, int y, int cols, int rows, int width, int height, int col_padding, int row_padding);
        virtual ~   LCARS_Group();

        void                Put(int gridx, int gridy, LCARS_Component * cmp);
        LCARS_Component *   Get(int gridx, int gridy);
        void                Pop(int gridx, int gridy);

        void    SetForceWidth   (bool b);
        void    SetForceHeight  (bool b);

        virtual bool PointInHitbox  (int x, int y)              override;
		virtual void Paint          (PaintContext * paintctx)   override;

		virtual void HandleSDLEvent         (SDL_Event * ev)    override;
		virtual void OnUnhandledSDLEvent    (SDL_Event * ev)    override;

		virtual void HandleCMPEvent (CMP_EVT_TYPE type)         override;

        LCARS_Group& operator=(const LCARS_Group&) = delete;
};

#endif