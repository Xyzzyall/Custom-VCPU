﻿// CustomVCPU.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "CPU.h"

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

int main(int argc, char* argv[])
{
	
    std::cout << "Hello World!\n";
	CPU cpu = CPU(128, 128);

	try
	{
		cpu.set_memory(test_prog, test_RAM);

		cpu.steps(500);
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what();
	}
	
	std::cout << cpu.tell_me_about_yourself();
	std::cin;
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
