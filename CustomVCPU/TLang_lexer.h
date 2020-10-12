#pragma once
#include "Lexer.h"

namespace software {

	class TLang_lexer :
		public Lexer
	{
	public:
		std::vector<token> lexerify();
	};
}


