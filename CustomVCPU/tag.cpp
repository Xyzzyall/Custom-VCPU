#include "tag.h"
#define str_tag_case(name) case name: return std::string( std::string(#name) + '(' + contains + ')');

software::tag::tag(int tag_name, std::string contains)
{
	this->tag_name = tag_name;
	this->contains = std::string(contains);
}

software::tag::~tag()
{
	//delete contains;
}

software::tag software::tag::copy()
{
	return tag(tag_name, std::string(contains));
}

std::string software::tag::to_string()
{
	switch (tag_name) //todo: could be packed in one big macros with other tags
	{
		str_tag_case(TAG_UNKNOWN)

		//common tags
		str_tag_case(TAG_SEMICOLON)
		str_tag_case(TAG_END)
		str_tag_case(TAG_NUMBER)
		str_tag_case(TAG_UNSEEN_DIVIDER)
		str_tag_case(TAG_TEXT)
		str_tag_case(TAG_EXIT)

		//mode sets
		str_tag_case(TAG_MODE)
		str_tag_case(TAG_COMPILER)
		str_tag_case(TAG_CMD)

		//compiler mode tags
		str_tag_case(TAG_COMPILE)

		//cmd mode tags
		str_tag_case(TAG_SAVEBIN)

		//settings
		str_tag_case(TAG_SETTINGS)
		str_tag_case(TAG_PROG_MEM_SIZE)
		str_tag_case(TAG_RAM_MEM_SIZE)

		//address tweaks
		str_tag_case(TAG_SHARP)
		str_tag_case(TAG_P_SHARP)
		str_tag_case(TAG_R_SHARP)

		//commands
		str_tag_case(TAG_PROGRAM)

		str_tag_case(TAG_READ)
		str_tag_case(TAG_COPY)
		str_tag_case(TAG_REG)
		str_tag_case(TAG_TO)
		str_tag_case(TAG_INDEX)
		str_tag_case(TAG_COMPARE)
		str_tag_case(TAG_JUMP)
		str_tag_case(TAG_SIGN)
		str_tag_case(TAG_NSIGN)
		str_tag_case(TAG_ZERO)
		str_tag_case(TAG_NZERO)
		str_tag_case(TAG_INC)

		str_tag_case(TAG_HALT)

		//ram 
		str_tag_case(TAG_RAM)
		str_tag_case(TAG_DEC)
	default:
		return std::string("Unknown");
	}
	return std::string();
}
