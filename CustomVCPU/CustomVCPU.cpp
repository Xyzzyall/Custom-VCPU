// CustomVCPU.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include "VCPU_Console.h"

//#define DEBUG

int main(int argc, char* argv[])
{
	software::VCPU_Console console = software::VCPU_Console();
	std::cout << "Hello! I'm tiny shitty CPU emulator, that exists just for lulz!\n";

#ifndef DEBUG
	if (argc > 1) {
		std::cout << "Inputting text file '" + std::string(argv[1]) + "'\n";
		std::ifstream input(argv[1]);
		while (!input.eof()) {
			std::string str;
			input >> str;
			//std::cout << str;
			console << str;
		}
		console.execute();
	}
#endif // !DEBUG	

	while (true) {
		try
		{
			char str[256];
			std::cin.getline(str, 256);
			console << str;
			//std::cout << "dick" + str;
			console.execute();
		}
		catch (const std::exception& exception)
		{
			std::cout << exception.what();
		}
		catch (const software::Lexer::lexer_exception& exception)
		{
			std::cout << exception.msg;
		}
		catch (const software::VCPU_Console::console_exception exc) {
			std::cout << exc.msg;
		}
		catch (const software::VCPU_Console::console_exit exc) {
			return 0;
		}
	}	
}
