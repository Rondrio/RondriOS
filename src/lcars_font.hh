#ifndef MAIN_HH_
#define MAIN_HH_

#include "paint_context.hh"

namespace LCARS {
	
	/* Both of these Functions are defined in the main.cc file. */
	
	_font *	CreateFont	(std::string fontname, uint16_t ptsize);
	_font * CreateFontPX(std::string fontname, uint16_t pxsize);
	void	DestroyFont	(_font * font);
	
	void TextSize(Text * t);
	
	KeySym KeyCodeToKeySym(KeyCode kc);
	
}

#endif