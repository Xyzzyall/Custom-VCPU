#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <exception>
#include <iostream>
#include "CPU.h"
#include "Builder.h"
#include "TLang_lexer.h"
#include "tlang_parser.h"

namespace software {
	
	class VCPU_Console
	{
	public:
		VCPU_Console();

		static const class console_exception : std::exception {
		public:
			console_exception(std::string str);
			std::string msg;
		};

		static const class console_exit : std::exception {};

		VCPU_Console& operator<< (std::string line);

		void execute();
	private:
		enum console_modes{ MODE_COMPILER, MODE_CMD };
		int cmode;

		software::Builder builder;
		TLang_lexer lexer;
		tlang_parser parser;
		

		hardware::CPU current_cpu;

		std::vector<std::string> buffer;
	};
}


