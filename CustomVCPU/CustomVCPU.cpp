// CustomVCPU.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include "VCPU_Console.h"


int main(int argc, char* argv[])
{
	software::VCPU_Console console = software::VCPU_Console();

	if (argc == 1) {
		std::ifstream input(argv[0]);
		while (!input.eof()) {
			std::string str;
			input >> str;
			console << str;
		}
		console.execute();
	}

	try
	{
		
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what();
	}
	catch (const software::Lexer::lexer_exception& exception)
	{
		std::cout << exception.msg;
	}
	
	
}
