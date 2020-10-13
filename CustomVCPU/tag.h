#pragma once
#include <string>

namespace software
{
	enum tags {
		TAG_UNKNOWN,

		//common tags
		TAG_SEMICOLON,
		TAG_END,
		TAG_NUMBER,
		TAG_UNSEEN_DIVIDER,
		TAG_TEXT,

		//mode sets
		TAG_MODE,
		TAG_COMPILER,
		TAG_CMD,

		//compiler mode tags
		TAG_COMPILE,

		//cmd mode tags
		TAG_SAVEBIN,

		//settings
		TAG_SETTINGS,
		TAG_PROG_MEM_SIZE,
		TAG_RAM_MEM_SIZE,

		//address tweaks
		TAG_SHARP,
		TAG_P_SHARP,
		TAG_R_SHARP,

		//commands
		TAG_PROGRAM,

		TAG_READ,
		TAG_COPY,
		TAG_REG,
		TAG_TO,
		TAG_INDEX,
		TAG_COMPARE,
		TAG_JUMP,
		TAG_SIGN,
		TAG_NSIGN,
		TAG_ZERO,
		TAG_NZERO,
		TAG_INC,

		TAG_SUM,
		TAG_SUM2,
		TAG_MULT,
		
		TAG_WRITE,

		TAG_HALT,

		//ram 
		TAG_RAM,
		TAG_DEC,

		//console
		TAG_STEP,
		TAG_STEPS,
		TAG_CPU_STATS,
		TAG_EXIT,

		
	};

	static const std::string meanful_chars = "._qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789#/";
	static const std::string numbers = "0123456789";
	static const std::string text_symbs = "._qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789";

	class tag
	{
	public:
		tag(int tag_name = TAG_UNKNOWN, std::string contains = "");
		~tag();
		int tag_name;
		std::string contains;
		tag copy();
		std::string to_string();
		static std::string get_name_of_tag(int tag_name);
	};

	
}

