#include "Parser.h"

namespace software 
{
	Parser::Parser()
	{
		current_word = std::vector<char>();
		parsed_tags = std::vector<tag>();
		syntax = syntax_tree();
	}

	Parser::~Parser()
	{
		
	}

	std::vector<tag> Parser::parse(std::stringstream text)
	{
		while (!text.eof()) {
			char c;
			text >> c;
			if (meanful_chars.find(c) == std::string::npos) {
				
				continue;
			}
			else {
				current_word.push_back(c);
			}
		}
		return parsed_tags;
	}

	
}

