#include "Lexer.h"

namespace software 
{
	Lexer::lexer_exception::lexer_exception(std::string str) : std::exception(str.c_str())
	{

	}

	Lexer::Lexer()
	{
		tags = std::vector<tag>();
		progmem_links = std::vector<std::string>();
		ram_links = std::vector<std::string>();
	}

	Lexer::token Lexer::create_token(int name, int data)
	{
		token res = token();
		res.data = data;
		res.name = name;
		return res;
	}

	Lexer::token Lexer::create_token_with_str(int name, int data, std::string str_operand)
	{
		token res = token();
		res.data = data;
		res.name = name;
		res.str_operand = std::string(str_operand);
		return res;
	}

	void Lexer::add_rule(rule r)
	{
		throw std::exception("work in progress!");
	}

	void Lexer::append_tag(tag t)
	{
		tags.push_back(t);
	}

	void Lexer::append_tags(std::vector<tag> tags)
	{
		for (tag t : tags)
			this->tags.push_back(t);
	}

	std::vector<Lexer::token> Lexer::lexerify()
	{
		//wip: for dynamic syntax trees
		throw std::exception("work in progress!");
		return std::vector<token>();
	}

	void Lexer::throw_expected_tag_exc(int * expected, int given)
	{
		std::string result = "Lexer error. Expected (";
		int size = sizeof(expected) / sizeof(*expected);
		for (int i = 0; i < size; i++) {
			result+= tag::get_name_of_tag(expected[i]) + ',';
		}
		result += "); given " + tag::get_name_of_tag(given) + ".";
		
		delete[] expected;

		throw lexer_exception(result);
	}

	void Lexer::add_progmem_link(std::string name)
	{
		if (get_progmem_link(name) == -1) {
			progmem_links.push_back(std::string(name));
		}
		else {
			throw lexer_exception("Duplicated progmem link. Name: '" + name + "'.");
		}
	}

	void Lexer::add_ram_link(std::string name)
	{
		if (get_ram_link(name) == -1) {
			ram_links.push_back(std::string(name));
		}
		else {
			throw lexer_exception("Duplicated ram link. Name: '" + name + "'.");
		}
	}

	int Lexer::get_progmem_link(std::string name)
	{
		int size = progmem_links.size();

		for (int i = 0; i < size; i++)
			if (name == progmem_links[i]) return i;

		progmem_links.push_back(std::string(name));
		return size;
	}

	int Lexer::get_ram_link(std::string name)
	{
		int size = ram_links.size();

		for (int i = 0; i < size; i++)
			if (name == ram_links[i]) return i;

		ram_links.push_back(std::string(name));
		return size;
	}

	Lexer::rule::rule(tag * tag_sequence, int ** tag_beh_sequence)
	{		

	}
}

