#pragma once
#include <vector>
#include <string>
#include "tag.h"
#include <sstream>

namespace software 
{
	enum actions {
		ACT_NOTHING,
		ACT_TAG,
		ACT_TEXT_NEXT,
		ACT_NUMBER,
		ACT_PASS_TO_NL,
		ACT_
	};

	class syntax_tree //it is not an actual tree, as you see
	{
	public:
		syntax_tree();
		void put_keyword(std::string word, int word_tag, int action = ACT_NOTHING);
		void append_next_char(char c);
		void erase_buffer();

		class tag_out {
		public:
			tag_out(tag t, int action);
			tag_out();
			tag t; int action;
		};
		tag_out try_to_make_tag(int you_must_make_it_like = TAG_UNKNOWN);

		
	private:
		std::vector<std::string> keywords;
		std::vector<int> actions;
		std::vector<int> tags;
		std::vector<char> current_word;

		std::string current_word_str();

		int num_of_keywords;
	};

}


