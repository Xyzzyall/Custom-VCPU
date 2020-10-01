#include "tlang_parser.h"

namespace software
{
	tlang_parser::tlang_parser()
	{
		//commentary
		syntax.put_keyword("//", TAG_UNKNOWN, ACT_PASS_TO_NL);
		//common tags
		syntax.put_keyword(";", TAG_SEMICOLON);
		syntax.put_keyword("end", TAG_END);
		syntax.put_keyword("1", TAG_NUMBER, ACT_NUMBER);
		syntax.put_keyword("2", TAG_NUMBER, ACT_NUMBER);
		syntax.put_keyword("3", TAG_NUMBER, ACT_NUMBER);
		syntax.put_keyword("4", TAG_NUMBER, ACT_NUMBER);
		syntax.put_keyword("5", TAG_NUMBER, ACT_NUMBER);
		syntax.put_keyword("6", TAG_NUMBER, ACT_NUMBER);
		syntax.put_keyword("7", TAG_NUMBER, ACT_NUMBER);
		syntax.put_keyword("8", TAG_NUMBER, ACT_NUMBER);
		syntax.put_keyword("9", TAG_NUMBER, ACT_NUMBER);
		syntax.put_keyword("exit", TAG_EXIT);

		//mode sets
		syntax.put_keyword("mode", TAG_MODE);
		syntax.put_keyword("compiler", TAG_COMPILER);
		syntax.put_keyword("cmd", TAG_CMD);

		//compiler mode tags
		syntax.put_keyword("compile", TAG_COMPILE);

		//cmd mode tags
		syntax.put_keyword("savebin", TAG_SAVEBIN);

		//settings
		syntax.put_keyword("settings", TAG_SETTINGS);
		syntax.put_keyword("size_prog", TAG_PROG_MEM_SIZE);
		syntax.put_keyword("size_ram", TAG_RAM_MEM_SIZE);

		//address tweaks
		syntax.put_keyword("#", TAG_SHARP);
		syntax.put_keyword("p#", TAG_P_SHARP);
		syntax.put_keyword("r#", TAG_R_SHARP);

		//commands
		syntax.put_keyword("program", TAG_PROGRAM);

		syntax.put_keyword("read", TAG_READ);
		syntax.put_keyword("copy", TAG_COPY);
		syntax.put_keyword("reg", TAG_REG);
		syntax.put_keyword("to", TAG_TO);
		syntax.put_keyword("index", TAG_INDEX);
		syntax.put_keyword("compare", TAG_COMPARE);
		syntax.put_keyword("jump", TAG_JUMP);
		syntax.put_keyword("sign", TAG_SIGN);
		syntax.put_keyword("nsign", TAG_NSIGN);
		syntax.put_keyword("zero", TAG_ZERO);
		syntax.put_keyword("nzero", TAG_NZERO);

		syntax.put_keyword("halt", TAG_HALT);

		//ram
		syntax.put_keyword("ram", TAG_RAM);

		syntax.put_keyword("dec", TAG_DEC);

	}
}