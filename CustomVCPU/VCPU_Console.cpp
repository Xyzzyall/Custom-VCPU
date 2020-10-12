#include "VCPU_Console.h"
namespace software 
{
	VCPU_Console::VCPU_Console()
	{
		parser = tlang_parser();
		lexer = TLang_lexer();
		builder = Builder();
		current_cpu = hardware::CPU();
		buffer = std::vector<std::string>();
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
		lexer.append_tags(parser.get_parsed_tags());
		std::vector<Lexer::token> tokens = lexer.lexerify();
		for (Lexer::token t : tokens) {
			switch (cmode)
			{
			case MODE_CMD:
				if (t.name != TOKEN_CONSOLE)
					throw VCPU_Console::console_exception("Console: this is not a console command.");
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
				case CONCMD_EXIT: //how to ruine everything by one line:
					throw console_exit();
					break;
				}
				break;
			case MODE_COMPILER:
				if ((t.name == TOKEN_CONSOLE) && (t.data == CONCMD_SET_CONSOLE_MODE)) {
					cmode = MODE_CMD;
					current_cpu = builder.get_cpu();
					break;
				}
				builder.put_token(t);
				break;
			}
		}
	}

	VCPU_Console::console_exception::console_exception(std::string str) : std::exception(str.c_str())
	{
		msg = str;
	}
}