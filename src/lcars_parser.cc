#include "lcars_parser.hh"

#include <iostream>
#include <string>
#include <cstring>

#include "lcars_error.hh"

static std::string parser_error = "Parser::Parse()";

LCARS::Parser::Parser(std::string document_filepath) {
	doc.LoadFile(document_filepath.c_str());
}

LCARS::Parser::~Parser() {

}

int LCARS::Parser::Parse() {
	
	/* Step 1: Look up the <Interface> */
	tinyxml2::XMLElement * interface = doc.RootElement();

	if(interface) {
		
		const char * name = interface->Name();

		if(strcmp(name, "Interface") != 0) {
			LOG(parser_error, "Root Element is not Interface!");
			return 0;
		}

		if(interface->NoChildren()) {
			LOG(parser_error, "Interface may not be empty!");
			return 0;
		}

		tinyxml2::XMLNode * firstchild = interface->FirstChild();
		

	} else {
		LOG(parser_error, "No Root Element found.");
		return 0;
	}
}