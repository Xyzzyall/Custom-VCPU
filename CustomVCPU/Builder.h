#pragma once
#include <vector>
#include <deque>
#include <exception>
#include <unordered_map>
#include "Lexer.h"
#include "CPU.h"

namespace software
{
	enum builder_cmds {
		BLDCMD_PROGRAM_START,
		BLDCMD_SETTINGS,
		BLDCMD_SET_PROG_MEM,
		BLDCMD_SET_RAM_MEM,
		BLDCMD_RAM_START,
		BLDCMD_COMPILE
	};
	class Builder
	{
	public:
		Builder();

		static const class buider_exception : std::exception {
		public:
			buider_exception(std::string str);
			std::string msg;
		};

		void put_token(Lexer::token token);
		void put_tokens(std::vector<Lexer::token> token);

		hardware::CPU get_cpu();
	protected:
		//std::vector<>;
		std::deque<Lexer::token> tokens;
		
		hardware::CPU compiled_cpu;

		void compile();
	private:
		void build_links(std::vector<short> & program);
		std::unordered_map<int, std::vector<int>> program_links;
		std::unordered_map<int, std::vector<int>> ram_links;

		void insert_progmem_link(int link, int row);
		void define_progmem_link(int link, int row);

		void insert_ram_link(int link, int row);
		void define_ram_link(int link, int row);
	};
}


