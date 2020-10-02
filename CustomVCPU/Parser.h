#pragma once
#include <string>
#include <vector>
#include "tag.h"
#include <sstream>
#include "syntax_tree.h"
#include <exception>

namespace software
{
	class Parser
	{
	public:
		Parser();
		~Parser();
		void parse(std::stringstream text);
		std::stringstream parsed_tags_to_sstream();
	private:
		std::vector<tag> parsed_tags;
		
	protected:
		syntax_tree syntax;

		class unknown_tag_exception : public std::exception {
		public:
			unknown_tag_exception(std::string st);
		};
	};


}


