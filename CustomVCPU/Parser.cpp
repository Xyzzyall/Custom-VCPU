#include "Parser.h"
#include <iostream>

namespace software 
{
	Parser::Parser()
	{
		parsed_tags = std::vector<tag>();
		syntax = syntax_tree();
	}

	void Parser::parse(std::stringstream text)
	{
		parsed_tags.clear(); //TODO: for JIT compilation it will be deleted
		int current_action = ACT_NOTHING;
		tag nulltag = tag();
		tag current_tag = nulltag;

		while (!text.eof()) {
			std::string s;
			text >> s;
			//std::cout << s << ' ';
			for (char c : s) { //todo: refactor this. I thought that sstream does not ignore space/nl characters.
				switch (current_action)
				{
				case ACT_NOTHING: //default procces of parsing common tags 
					if (meanful_chars.find(c) == std::string::npos) {
						if (current_tag.tag_name != TAG_UNKNOWN) { //tag is determined
							parsed_tags.push_back(current_tag.copy());
							syntax.erase_buffer();
							current_tag = nulltag;
						}
						else {
							//throw unknown_tag_exception("Unknown tag has been meet. It contains: " + current_tag.contains);
						}
					}
					else { //the word is not over
						syntax.append_next_char(c);
						syntax_tree::tag_out out = syntax.try_to_make_tag();
						current_tag = out.t;
						//std::cout << current_tag.to_string();
						current_action = out.action;
						if ((current_action == ACT_TAG) || (current_action == ACT_TEXT_NEXT)) { //for "in word" tags and names after tags
							parsed_tags.push_back(current_tag.copy());
							current_tag = nulltag;
							syntax.erase_buffer();
						}
						if (current_action == ACT_TAG) {
							current_action = ACT_NOTHING;  //back to finding other tags
						}
					}
					break;
				case ACT_NUMBER: //if the word starts with 1-9
					if (numbers.find(c) == std::string::npos) {
						parsed_tags.push_back(current_tag.copy());
						current_tag = nulltag;
						syntax.erase_buffer();
						current_action = ACT_NOTHING; //back to finding other tags
					}
					else {
						syntax.append_next_char(c);
						syntax_tree::tag_out out = syntax.try_to_make_tag(TAG_NUMBER);
						current_tag = out.t;
					}
					break;
				case ACT_TEXT_NEXT: //for names
					if (text_symbs.find(c) == std::string::npos) {
						parsed_tags.push_back(current_tag.copy());
						current_tag = nulltag;
						syntax.erase_buffer();
						current_action = ACT_NOTHING; //back to finding other tags
					}
					else {
						syntax.append_next_char(c);
						syntax_tree::tag_out out = syntax.try_to_make_tag(TAG_TEXT);
						current_tag = out.t;
					}
					break;
				case ACT_PASS_TO_NL: //for comments
					if (c == '/') {
						current_action = ACT_NOTHING; //back to finding other tags
						syntax.erase_buffer();
						current_tag = nulltag;
					}
				default:
					break;
				}
			}

			if (current_tag.tag_name != TAG_UNKNOWN) {
				parsed_tags.push_back(current_tag.copy());
				syntax.erase_buffer();
				current_tag = nulltag;
				current_action = ACT_NOTHING;
			}
			
		}
	}

	std::stringstream Parser::parsed_tags_to_sstream()
	{
		std::stringstream out = std::stringstream();
		int size = parsed_tags.size();
		for (int i = 0; i < size; i++)
		{
			out << std::to_string(i) << "\t" << parsed_tags[i].to_string() << '\n';
		}
		return out;
	}

	std::vector<tag> Parser::get_parsed_tags()
	{
		std::vector<tag> res = std::vector<tag>();
		for (tag t:parsed_tags) {
			res.push_back(t.copy());
		}
		return res;
	}

	
	Parser::unknown_tag_exception::unknown_tag_exception(std::string str) : std::exception(str.c_str())
	{

	}

}

