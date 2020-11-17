#include "Builder.h"

namespace software
{
	Builder::Builder()
	{
		compiled_cpu = hardware::CPU(1,1);
		tokens = std::deque<Lexer::token>();
		program_links = std::unordered_map<int, std::vector<int>>();
		ram_links = std::unordered_map<int, std::vector<int>>();
		logs = std::vector<std::string>();
	}

	Builder::buider_exception::buider_exception(std::string str) : std::exception(str.c_str())
	{
		msg = str;
	}

	void Builder::put_token(Lexer::token token)
	{
		if ((token.name == TOKEN_BUILDER) && (token.data == BLDCMD_COMPILE)) {
			tokens.push_back(Lexer::copy_token(token));
			compile();
		}
		else {
			tokens.push_back(Lexer::copy_token(token));
		}
			
	}

	void Builder::put_tokens(std::vector<Lexer::token> token)
	{
	}

	void Builder::clear_token_buffer()
	{
		tokens.clear();
	}

	std::string Builder::get_logs()
	{
		std::string res = "";
		for (std::string s : logs) {
			res += s + '\n';
		}
		logs.clear();
		return res;
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
		#define IF_NOT_SUCCESS(details) if (!success) throw Builder::buider_exception("Bad token " + Lexer::token_to_str(tokens.front().name) + ':' + std::to_string(tokens.front().data) + " . Details: " + std::string(details))

		log("Start building. Given " + std::to_string(tokens.size()) + " tokens.");

		log("Tokens: ");
		std::vector<Lexer::token> tkns;
		int size = tokens.size();
		for (int i = 0; i < size; i++) {
			tkns.push_back(tokens[i]);
		}
		log(Lexer::tokens_to_to_str(tkns));

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
							log("Program size was not enought or not defined.");
							//throw Builder::buider_exception("Program memory error: the defined amount is not enough!");
						}
						else {
							program.resize(program_size);
						}

						if (ram_size < ram.size()) {
							ram_size = ram.size();
							log("RAM size was not enought or not defined.");
							//throw Builder::buider_exception("RAM error: the defined amount is not enough!");
						}
						else {
							ram.resize(ram_size);
						}

						log("Program size = " + std::to_string(program_size));
						log("RAM size = " + std::to_string(ram_size));

						log("Linking " + std::to_string(program_links.size()) +" program links.");
						log("Linking " + std::to_string(ram_links.size()) + " RAM links.");
						build_links(program);
						log("Building successful.");
						log("Program:");
						log(data_to_str(program));
						log("Memory:");
						log(data_to_str(ram));
						compiled_cpu = hardware::CPU(ram_size, program_size);
						compiled_cpu.set_memory(program.data(), ram.data());
						log("CPU has created. Use 'step' or 'steps *num*' to run.");
						success = true;
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
				IF_NOT_SUCCESS("this command is not allowed in settings mode.");
				tokens.pop_front();
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
				IF_NOT_SUCCESS("this command is not allowed in program mode.");
				tokens.pop_front();
				break;
			case MODE_RAM:
				IF_TKN_NAME(TOKEN_RAM_DATA) {
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
				IF_NOT_SUCCESS("this command is not allowed in RAM mode.");
				tokens.pop_front();
				break;
			}
		}
	}

	void software::Builder::log(std::string msg)
	{
		logs.push_back(msg);
	}

	std::string software::Builder::data_to_str(std::vector<short> data)
	{
		std::stringstream stream;
		int size = data.size();
		for (int i = 0; i < size; i++) {
			stream << i << '\t' << std::hex << data[i] << '\n';
		}
		return stream.str();
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