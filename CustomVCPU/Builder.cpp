#include "Builder.h"

namespace software
{
	Builder::Builder()
	{
		compiled_cpu = hardware::CPU();
		tokens = std::deque<Lexer::token>();
		program_links = std::unordered_map<int, std::vector<int>>();
		ram_links = std::unordered_map<int, std::vector<int>>();
	}

	Builder::buider_exception::buider_exception(std::string str) : std::exception(str.c_str())
	{
		msg = str;
	}

	void Builder::put_token(Lexer::token token)
	{
		if ((token.name == TOKEN_BUILDER) && (token.data == BLDCMD_COMPILE)) {
			compile();
		}
		else {
			tokens.push_back(Lexer::copy_token(token));
		}
			
	}

	void Builder::put_tokens(std::vector<Lexer::token> token)
	{
	}

	hardware::CPU Builder::get_cpu()
	{
		return hardware::CPU(compiled_cpu);
	}

	void Builder::compile()
	{
		enum modes {
			MODE_UNKNOWN, MODE_SETTINGS, MODE_PROGRAM, MODE_RAM
		};
		int mode = MODE_UNKNOWN;

		std::vector<short> ram = std::vector<short>();
		std::vector<short> program = std::vector<short>();

		unsigned int ram_size = 0;
		unsigned int program_size = 0;

		bool success = true;

		#define IF_TKN_END if ((tokens.front().name == TOKEN_COMMON) && (tokens.front().data == CONCMD_END) ) { mode = MODE_UNKNOWN; success = true; }
		#define IF_TKN_NAME(n) if (tokens.front().name == n)
		#define IF_NOT_SUCCESS(details) if (!success) throw Builder::buider_exception("Bad tokens. Details: " + std::string(details))

		while (!tokens.empty())
		{
			success = false;
			switch (mode)
			{
			case MODE_UNKNOWN:
				IF_TKN_NAME(TOKEN_BUILDER) {
					switch (tokens.front().data)
					{
					case BLDCMD_SETTINGS:
						mode = MODE_SETTINGS;
						success = true;
						break;
					case BLDCMD_PROGRAM_START:
						mode = MODE_PROGRAM;
						success = true;
						break;
					case BLDCMD_RAM_START:
						mode = MODE_RAM;
						success = true;
						break;
					case BLDCMD_COMPILE:
						if (program_size < program.size()) {
							program_size = program.size();
							//throw Builder::buider_exception("Program memory error: the defined amount is not enough!");
						}
						else {
							program.resize(program_size);
						}

						if (ram_size < ram.size()) {
							ram_size = ram.size();
							//throw Builder::buider_exception("RAM error: the defined amount is not enough!");
						}
						else {
							ram.resize(ram_size);
						}

						build_links(program);
						compiled_cpu = hardware::CPU(ram_size, program_size);
						compiled_cpu.set_memory(program.data(), ram.data());
						break;
					}
				}
				IF_NOT_SUCCESS("this command is not allowed in compiler mode.");
				tokens.pop_front();
				break;
			case MODE_SETTINGS:
				IF_TKN_NAME(TOKEN_BUILDER) {
					switch (tokens.front().data)
					{
					case BLDCMD_SET_PROG_MEM:
						tokens.pop_front();
						program_size = tokens.front().data;
						success = true;
						break;
					case BLDCMD_SET_RAM_MEM:
						tokens.pop_front();
						ram_size = tokens.front().data;
						success = true;
						break;
					}
				}
				
				IF_TKN_END;
				tokens.pop_front();
				IF_NOT_SUCCESS("this command is not allowed in settings mode.");
				break;
			case MODE_PROGRAM: //maybe better use switch/case too, but I am so tired of them
				IF_TKN_NAME(TOKEN_COMMAND) {
					program.push_back(tokens.front().data);
					success = true;
				}
				IF_TKN_NAME(TOKEN_OPERAND) {
					program.push_back(tokens.front().data);
					success = true;
				}
				IF_TKN_NAME(TOKEN_PROGMEM_LINK) {
					define_progmem_link(tokens.front().data, program.size());
					success = true;
				}
				IF_TKN_NAME(TOKEN_RAM_LINK) { //just skipping, it does not make sense in this context
					success = true;
				}
				IF_TKN_NAME(TOKEN_PUT_PROGMEM_LINK) {
					program.push_back(-1);
					insert_progmem_link(tokens.front().data, program.size() - 1);
					success = true;
				}
				IF_TKN_NAME(TOKEN_PUT_RAM_LINK) {
					program.push_back(-1);
					insert_ram_link(tokens.front().data, program.size() - 1);
					success = true;
				}
				IF_TKN_END;
				tokens.pop_front();
				IF_NOT_SUCCESS("this command is not allowed in program mode.");
				break;
			case MODE_RAM:
				IF_TKN_NAME(TOKEN_RAM) {
					ram.push_back(tokens.front().data);
					success = true;
				}
				IF_TKN_NAME(TOKEN_PROGMEM_LINK) { //just skipping, it does not make sense in this context
					success = true;
				}
				IF_TKN_NAME(TOKEN_RAM_LINK) {
					define_ram_link(tokens.front().data, ram.size());
					success = true;
				}
				IF_TKN_END;
				tokens.pop_front();
				IF_NOT_SUCCESS("this command is not allowed in RAM mode.");
				break;
			}
		}
	}

	void Builder::build_links(std::vector<short> & program)
	{
		//progmem links
		{
			auto it = program_links.begin();
			while (it != program_links.end()) {
				if (it->second[0] == -1)
					throw Builder::buider_exception("Program links are corrupted: there is undeclared link.");
				int size = it->second.size();
				for (int i = 1; i < size; i++) {
					try {
						program[it->second[i]] = it->second[0];
					}
					catch (const std::out_of_range exc) {
						throw Builder::buider_exception("Program links are corrupted: link forwards to nowhere.");
					}
				}
				it++;
			}
		}
		//ram links
		{
			auto it = ram_links.begin();
			while (it != ram_links.end()) {
				if (it->second[0] == -1)
					throw Builder::buider_exception("RAM links are corrupted: there is undeclared link.");
				int size = it->second.size();
				for (int i = 1; i < size; i++) {
					try {
						program[it->second[i]] = it->second[0];
					}
					catch (const std::out_of_range exc) {
						throw Builder::buider_exception("RAM links are corrupted: link forwards to nowhere.");
					}
				}
				it++;
			}
		}
		//clearing link buffers for further usage
		program_links.clear();
		ram_links.clear();
	}
	void software::Builder::insert_progmem_link(int link, int row)
	{
		try {
			program_links.at(link);
		}
		catch (const std::out_of_range exc) {
			define_progmem_link(link, -1);
		}
		program_links[link].push_back(row);
	}
	void software::Builder::define_progmem_link(int link, int row)
	{
		try {
			program_links.at(link);
			program_links[link][0] = row;
		}
		catch (const std::out_of_range exc) {
			program_links[link] = std::vector<int>();
			program_links[link].push_back(row);
		}
	}
	void software::Builder::insert_ram_link(int link, int row)
	{
		try {
			ram_links.at(link);
		}
		catch (const std::out_of_range exc) {
			define_ram_link(link, -1);
		}
		ram_links[link].push_back(row);
	}
	void software::Builder::define_ram_link(int link, int row)
	{
		try {
			ram_links.at(link);
			ram_links[link][0] = row;
		}
		catch (const std::out_of_range exc) {
			ram_links[link] = std::vector<int>();
			ram_links[link].push_back(row);
		}
	}
}