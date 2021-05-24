#include "lcars_group.hh"
#include "../lcars_component.hh"

#include <iostream>
#include <cstdlib>
#include <cstring>

LCARS::Group::Group(int x, int y, int cols, int rows, int width, int height, int col_padding, int row_padding)
	: Component({x, y, cols*(width+col_padding), rows*(height+row_padding)})
	
	{

	m_force_height	= true;
	m_force_width	= true;

	m_cols = cols;
	m_rows = rows;
	m_width = width;
	m_height = height;
	m_col_padding = col_padding;
	m_row_padding = row_padding;


	m_grid = (Component **) malloc(sizeof(Component *)*m_cols*m_rows);

	memset(m_grid, 0, sizeof(Component *)*m_cols*m_rows);
}

LCARS::Group::~Group() {
	free(m_grid);
}

void LCARS::Group::Arrange(int gridx, int gridy, Component * cmp) {
	if(cmp) {

		if(m_force_width)
			cmp->SetWidth(m_width);
		
		if(m_force_height)
			cmp->SetHeight(m_height);


		/* These calculations a not done often */
		/* so I am not going to optimize. */
		int w = 0;
		for(int x = 0; x < gridx; ++x) {
			Component * prev_cmp = m_grid[gridy*m_cols + x];
			
			if(prev_cmp)
				w += prev_cmp->GetWidth();
			else
				w+= m_width;
		}

		int h = 0;
		for(int y = 0; y < gridy; ++y) {
			Component * prev_cmp = m_grid[y*m_cols + gridx];
			
			if(prev_cmp)
				h += prev_cmp->GetHeight();
			else
				h += m_height;
		}

		cmp->SetPosX(w + gridx * m_col_padding);
		cmp->SetPosY(h + gridy * m_row_padding);
	}
}

void LCARS::Group::Rearrange() {

	for(int y = 0; y < m_rows; ++y) {
		for(int x = 0; x < m_cols; ++x) {
			Component * cmp = m_grid[y*m_cols + x];
			Arrange(x, y, cmp);
		}
	}

	SetNeedsRepaint(true);
}

void LCARS::Group::Put(int gridx, int gridy, Component * cmp) {
	if(gridx < m_cols && gridy < m_rows) {
		m_grid[m_cols*gridy + gridx] = cmp;
		AddChild(cmp);

		/* Maybe I shouldn't rearrange every time a Component is added but instead */
		/* Give the User a Method to rearrange for themselves as soon they are done */
		/* with whatever operation (Put(), Pop()) they executed. */
		Rearrange();
	}
}

LCARS::Component * LCARS::Group::Get(int gridx, int gridy) {
	if(gridx < m_cols && gridy < m_rows) {
		return m_grid[m_cols*gridy + gridx];
	}

	return nullptr;
}

void LCARS::Group::Pop(int gridx, int gridy) {
	if(gridx < m_cols && gridy < m_rows) {
		Component * cmp = m_grid[m_cols*gridy + gridx];
		
		RemChild(cmp);
		m_grid[m_cols*gridy + gridx] = nullptr;

		/* Same as in the annotation inside the Put() Method. */
		Rearrange();
	}
}

void LCARS::Group::SetForceWidth(bool b) {
	m_force_width = b;
	SetNeedsRepaint(true);
}

void LCARS::Group::SetForceHeight(bool b) {
	m_force_height = b;
	SetNeedsRepaint(true);
}

bool LCARS::Group::PointInHitbox(int x, int y) {
	SDL_Rect abs = GetAbsoluteBounds();
	return x >= abs.x && x <= abs.x + m_bounds.w && y >= abs.y && y <= abs.y + m_bounds.h;
}

void LCARS::Group::Paint(PaintContext * paintctx) {
	//TODO: Leave empty, I guess.
}


void LCARS::Group::HandleSDLEvent(SDL_Event * ev) {
	//TODO: Leave empty, I guess.
}

void LCARS::Group::OnUnhandledSDLEvent(SDL_Event * ev) {
	//TODO: Leave empty, I guess.
}


void LCARS::Group::HandleCMPEvent(CMP_EVT_TYPE type) {
	//TODO: Leave empty, I guess.
}