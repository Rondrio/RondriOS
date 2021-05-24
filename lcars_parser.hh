#ifndef LCARS_PARSER_HH_
#define LCARS_PARSER_HH_

#include <string>
#include <tinyxml2.h>

namespace LCARS {

	/*
	
	TagNames:

	Interface, Resources

	Font, Color, String

	*/

	const std::string tagnames[] = {
		"Interface", "Resources", "Font", "Color"
	};

	class Parser {
		
		private:
			tinyxml2::XMLDocument doc;

		public:
			Parser()				= delete;
			Parser(const Parser&)	= delete;
			Parser(std::string document_filepath);
			virtual ~Parser();

			int Parse();


			virtual Parser& operator=(const Parser&) = delete;
	};

}

#endif