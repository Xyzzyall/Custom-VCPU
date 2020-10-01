#include "tag.h"

software::tag::tag(int tag_name = software::TAG_UNKNOWN, std::string contains = "")
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
