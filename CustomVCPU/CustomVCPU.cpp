// CustomVCPU.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "CPU.h"
#include <sstream>
#include "tlang_parser.h"

using namespace hardware;

short test_prog[128] = {
	CPU::CMD_READ, 0, //0, 1
	CPU::CMD_COPY_REG0_TO_REGX + 6, //2

	CPU::CMD_READ_INDEX, 1, //3, 4
	CPU::CMD_CMP_REGX + 1, //5
	CPU::CMD_JUMP_NSIGN, 13, //6, 7

	CPU::CMD_INC_REGX + 7, //8
	CPU::CMD_CMP_ARRAY, //9
	CPU::CMD_JUMP_SIGN, 3, //10, 11
	CPU::CMD_HALT, //12

	CPU::CMD_COPY_REG0_TO_REGX + 1, //13
	CPU::CMD_JUMP, 8, //14, 15

};

short test_RAM[128] = {
 11,
 1, 2, 3, 4, 15, 5, 6, 7, 8, 9, 0
};

std::stringstream test_prog_text = std::stringstream() << R"(
mode compiler;
settings;
	size_prog 128; /commentary/
	size_ram 128;
end settings;

program;
	/main/
	read r#array_len;
	copy reg0 to reg6;
	#reading_array;
		read index r#array;
		compare reg1; 
		jump nsign p#rewrite_reg1; #reg1copied;
		inc index;
		compare index;
		jump sign p#reading_array;
	halt; 	
	
	/functions/
	#rewrite_reg1;
		copy reg0 to reg1;
		jump p#reg1copied;
		
end program;

ram;
	#array_len;
		dec 10;
	#array;
		dec 1; dec 2; dec 3; dec 10; dec 5; dec 3; dec 3; dec 1; dec 0; dec 7;
end ram;

compile;

mode cmd;
savebin test_program.bin;
exit;
)";

int main(int argc, char* argv[])
{
	
    std::cout << "Hello World!\n";
	CPU cpu = CPU(128, 128);

	try
	{
		cpu.set_memory(test_prog, test_RAM);

		cpu.steps(500);
		std::cout << cpu.tell_me_about_yourself();
		std::getchar();
		software::tlang_parser parser = software::tlang_parser();
		parser.parse(std::stringstream() << R"(
mode compiler;
settings;
	size_prog 128; /commentary/
	size_ram 128;
end settings;

program;
	/main/
	read r#array_len;
	copy reg0 to reg6;
	#reading_array;
		read index r#array;
		compare reg1; 
		jump nsign p#rewrite_reg1; #reg1copied;
		inc index;
		compare index;
		jump sign p#reading_array;
	halt; 	
	
	/functions/
	#rewrite_reg1;
		copy reg0 to reg1;
		jump p#reg1copied;
		
end program;

ram;
	#array_len;
		dec 10;
	#array;
		dec 1; dec 2; dec 3; dec 10; dec 5; dec 3; dec 3; dec 1; dec 0; dec 7;
end ram;

compile;

mode cmd;
savebin test_program.bin;
exit;
)");
		//std::cout << test_prog_text.str();
		std::cout << parser.parsed_tags_to_sstream().str();
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what();
	}
	
	
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
