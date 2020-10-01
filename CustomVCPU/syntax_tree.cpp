#include "syntax_tree.h"

namespace software 
{
	syntax_tree::tag_out::tag_out(tag t, int action)
	{
		this->t = t;
		this->action = action;
	}

	syntax_tree::tag_out::tag_out()
	{
		action = ACT_NOTHING;
	}

	syntax_tree::syntax_tree()
	{
		keywords = std::vector<std::string>();
		actions = std::vector<int>();
		current_word = std::vector<char>();
		num_of_keywords = 0;
	}
	void syntax_tree::put_keyword(std::string word, int word_tag, int action)
	{
		keywords.push_back(word);
		actions.push_back(action);
		tags.push_back(word_tag);
		num_of_keywords++;
	}
	void syntax_tree::append_next_char(char c)
	{
		current_word.push_back(c);
	}

	syntax_tree::tag_out syntax_tree::try_to_make_tag(int you_must_make_it_like = TAG_UNKNOWN) //ok, I know, it a terrible, but I'm too lazy to deal with it in a cool way.
	{
		if (you_must_make_it_like == TAG_UNKNOWN) {
			for (int i = 0; i < num_of_keywords; i++) {
				if (keywords[i] == current_word.data) {
					return tag_out(tag(tags[i], std::string(keywords[i])), actions[i]);
				}
			}
			return tag_out();
		}
		else {
			return tag_out(tag(you_must_make_it_like, current_word.data), ACT_TAG);
		}
		
	}
	
}