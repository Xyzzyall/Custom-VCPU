#include "VCPU_Console.h"
namespace software 
{
	VCPU_Console::VCPU_Console()
	{
		std::cout << "loading...\n";
		parser = tlang_parser();
		std::cout << "parser initialized...\n";
		lexer = TLang_lexer();
		std::cout << "lexer initialized...\n";
		builder = Builder();
		std::cout << "builder initialized...\n";
		current_cpu = hardware::CPU(1, 1);
		std::cout << "CPU initialized...\n";
		buffer = std::vector<std::string>();
		std::cout << "Done!\n";
		cmode = MODE_CMD;
	}

	VCPU_Console & VCPU_Console::operator<<(std::string line)
	{
		buffer.push_back(line);
		return *this;
	}

	void VCPU_Console::execute()
	{
		//oh, it's so fucking terrible code. I do not know why, but stringstream does not seem to work properly.
		std::string stream = "";
		for (std::string line : buffer)
			stream += line;
		parser.parse(std::stringstream() << stream);
		buffer.clear();
		//std::cout << '\n' << parser.parsed_tags_to_sstream().str();
		
		lexer.append_tags(parser.get_parsed_tags());

		try {
			std::vector<Lexer::token> tokens = lexer.lexerify();

			for (Lexer::token t : tokens) {
				switch (cmode)
				{
				case MODE_CMD:
					if (t.name != TOKEN_CONSOLE)
						throw VCPU_Console::console_exception("Console: this is not a console command.\n");
					switch (t.data)
					{
					case CONCMD_SET_COMPILER_MODE:
						cmode = MODE_COMPILER;
						break;
					case CONCMD_SET_CONSOLE_MODE:
						cmode = MODE_CMD;
						break;
					case CONCMD_SAVEBIN: //WIP
						break;
					case CONCMD_CPU_STATUS:
						std::cout << current_cpu.tell_me_about_yourself();
						break;
					case CONCMD_STEP:
						current_cpu.step();
						break;
					case CONCMD_STEPS:
						//std::cout << '\n' <<t.str_operand << '\n';
						try {
							current_cpu.steps(std::stoi(t.str_operand));
						}
						catch (const std::invalid_argument exc) {
							throw VCPU_Console::console_exception("Console: invalid command argument.\n");
						}

						break;
					case CONCMD_EXIT: //how to ruine everything by one line:
						throw console_exit();
						break;
					}
					break;
				case MODE_COMPILER:
					if ((t.name == TOKEN_CONSOLE) && (t.data == CONCMD_SET_CONSOLE_MODE)) {
						cmode = MODE_CMD;
						current_cpu = builder.get_cpu();
						builder.clear_token_buffer();
						std::cout << builder.get_logs();
					}
					else {
						builder.put_token(t);
					}
					break;
				}
			}
		}
		catch (const std::out_of_range exc) {
			std::cout << exc.what() << ": some troubles.\n";
		}
		catch (const Builder::buider_exception exc) {
			std::cout << exc.msg;
		}
		
		
	}

	VCPU_Console::console_exception::console_exception(std::string str) : std::exception(str.c_str())
	{
		msg = str;
	}
}