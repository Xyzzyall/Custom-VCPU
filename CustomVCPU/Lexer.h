#pragma once
#include "tag.h"
#include <vector>
#include <string>
#include <exception>

namespace software
{
	enum tag_behaviour 
	{
		TAGB_PASS, 

		TAGB_PROGRAM,
		TAGB_PUT_CMD,
		TAGB_PUT_AS_SUBOPERAND,
		TAGB_SET_LINK_PROGMEM,
		TAGB_LINK_PROGMEM,
		TAGB_PUT_OPERAND,
		
		TAGB_RAM,
		TAGB_SET_LINK_RAM,
		TAGB_LINK_RAM,

		TAGB_NUMERIC,
		TAGB_FROM_DEC_TO_HEX,

		TAGB_CONSOLE_CMD,

		TAGB_SETTINGS,

		TAGB_PUT_TOKEN_DATA,
		TAGB_PUT_TOKEN_NAME,

		TAGB_CHECK_ENVIRONMENT,
		TAGB_ENV_CONSOLE,
		TAGB_ENV_COMPILER
	};

	enum tokens
	{
		TOKEN_COMMAND,
		TOKEN_OPERAND,
		TOKEN_PROGMEM_LINK,
		TOKEN_PUT_PROGMEM_LINK,
		TOKEN_PUT_RAM_LINK,

		TOKEN_RAM,
		TOKEN_RAM_LINK,
		TOKEN_RAM_DATA,

		TOKEN_RAM_SIZE_SETTINGS,
		TOKEN_PROGMEM_SIZE_SETTINGS,

		TOKEN_COMMON,
		TOKEN_CONSOLE,
		TOKEN_BUILDER,
	};

	class Lexer
	{
	public:
		Lexer();
		
		static struct token {
			int data;
			int name;
			std::string str_operand;
		};
		static token create_token(int name, int data);
		static token create_token_with_str(int name, int data, std::string str_operand);
		static token copy_token(token t);

		static const class rule { //wip: for dynamic syntax trees
		public:
			rule(tag* tag_sequence, int ** tag_beh_sequence);

			bool check_tags(std::vector<tag> tags);
			std::vector<token> get_tokens();
		private:
			std::vector<tag> tag_sequence;
			int** tag_beh_sequence;
		};

		static const class lexer_exception : std::exception {
		public:
			lexer_exception(std::string str);
			std::string msg;
		};

		void add_rule(rule r); //wip: for dynamic syntax trees
		
		void append_tag(tag t);
		void append_tags(std::vector<tag> tags);

		std::vector<token> lexerify();

		static std::string tokens_to_to_str(std::vector<token> tokens);
		static std::string token_to_str(int token);

	protected:
		std::vector<tag> tags;

		void throw_expected_tag_exc(int * expected, int given, int tag_n);

		static struct link {
			std::string name;
			int position;
		};

		void add_progmem_link(std::string name);
		void add_ram_link(std::string name);

		int get_progmem_link(std::string name);
		int get_ram_link(std::string name);

		void clear_links_buffer();
	private:
		std::vector<std::string> progmem_links;
		std::vector<std::string> ram_links;
	};
}


