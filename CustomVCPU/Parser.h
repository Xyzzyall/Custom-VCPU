#pragma once
#include <string>
#include <vector>
#include "tag.h"
#include <sstream>
#include "syntax_tree.h"

namespace software
{
	class Parser
	{
	public:
		Parser();
		~Parser();
		std::vector<tag> parse(std::stringstream text);
	private:
		std::vector<tag> parsed_tags;
		std::vector<char> current_word;
		
	protected:
		syntax_tree syntax;
	};


}


