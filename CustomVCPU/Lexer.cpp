#include "Lexer.h"

namespace software 
{
	Lexer::lexer_exception::lexer_exception(std::string str) : std::exception(str.c_str())
	{
		msg = str;
	}

	Lexer::Lexer()
	{
		tags = std::vector<tag>();
		progmem_links = std::vector<std::string>();
		ram_links = std::vector<std::string>();
	}

	Lexer::~Lexer()
	{
		for (tag t : tags)
			delete &t;
		delete &tags;
		delete 
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

	Lexer::token Lexer::copy_token(token t)
	{
		return create_token_with_str(t.name, t.data, t.str_operand);
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

	std::string Lexer::tokens_to_to_str(std::vector<token> tokens)
	{
		std::string result = "Total lines: " + std::to_string(tokens.size()) + ".\n" + "i\ttoken\tdata\tstr\n";
		int size = tokens.size();
		for (int i = 0; i < size; i++)
		{
			result += std::to_string(i) + '\t' + token_to_str(tokens[i].name) + 
				'\t' + std::to_string(tokens[i].data) + '\t' + tokens[i].str_operand + '\n';
		}
		return result;
	}


	std::string Lexer::token_to_str(int token)
	{
#define tkn_str(name) case name: return std::string(#name);
		switch (token) {
			tkn_str(TOKEN_COMMAND)
			tkn_str(TOKEN_OPERAND)
			tkn_str(TOKEN_PROGMEM_LINK)
			tkn_str(TOKEN_PUT_PROGMEM_LINK)
			tkn_str(TOKEN_PUT_RAM_LINK)

			tkn_str(TOKEN_RAM)
			tkn_str(TOKEN_RAM_LINK)
			tkn_str(TOKEN_RAM_DATA)

			tkn_str(TOKEN_RAM_SIZE_SETTINGS)
			tkn_str(TOKEN_PROGMEM_SIZE_SETTINGS)

			tkn_str(TOKEN_COMMON)
			tkn_str(TOKEN_CONSOLE)
			tkn_str(TOKEN_BUILDER)
		}
		return std::string();
	}

	void Lexer::throw_expected_tag_exc(int * expected, int given, int tag_n)
	{
		std::string result = "Tag: " + std::to_string(tag_n) + ". Lexer error. Expected (";
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

	void Lexer::clear_links_buffer()
	{
		progmem_links.clear();
		ram_links.clear();
	}

	Lexer::rule::rule(tag * tag_sequence, int ** tag_beh_sequence)
	{		

	}
}

