#pragma once
#include <string>

namespace software
{
	class tag
	{
	public:
		tag(int tag_name = software::TAG_UNKNOWN, std::string contains = "");
		~tag();
		int tag_name;
		std::string contains;
		tag copy();
	};


	enum tags {
		TAG_UNKNOWN,

		//common tags
		TAG_SEMICOLON,
		TAG_END,
		TAG_NUMBER,
		TAG_UNSEEN_DIVIDER,
		TAG_TEXT,
		TAG_EXIT,

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

		TAG_HALT,

		//ram 
		TAG_RAM,
		TAG_DEC

	};

	std::string meanful_chars = "._qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM;0123456789#/";
}

