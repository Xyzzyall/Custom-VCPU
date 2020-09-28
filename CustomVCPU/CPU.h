#pragma once
#define CPU_REG_COUNT 8
#include <string>

/* About registers:
reg0 -- accumulator, result of every operation stays here
from reg1 to reg6 -- general purpose
reg7 -- address register, but may be used as you like
*/
class CPU
{
	public:
		CPU(int ram_capacity, int prog_mem_capacity);

		void step();
		void steps(int steps);
		void goto_step(int step);

		void reset_CPU();
		void load_memory(std::string prog_bin, std::string ram_bin);

		std::string tell_me_about_yourself();

		enum Commands {
			CMD_CHILL = 0, //does nothing
			CMD_READ_TO_REG0 = 0x0100, //op - ram pointer, reads from RAM to reg0
			CMD_COPY_REG0_TO_REGX = 0x0200, //no op, copies var from reg0 to regX, X defined by low byte
			CMD_COPY_REGX_TO_REG0 = 0x0300, //no op, copies var from regX to reg0, X defined by low byte
			CMD_CMP_REGX = 0x0400, //no op, compares regX to reg0, X defined by low byte.
			CMD_SET_INDEX = 0x0500, //op - ram pointer, reads from RAM to reg7, that used for index addressation.
			CMD_INC_REGX = 0x0600, //no op, increments regX, X defined by low byte
			CMD_HALT = 0xFFFF //halt
		};

		~CPU();
	private:
		//debug vars
		int current_step;

		//memory vars
		int ram_capacity;
		short *ram;

		int prog_mem_capacity;
		short *program;

		//memory operations
		short read_prog_mem(int ptr);
		short read_operand();

		short read_ram(int ptr);
		void write_ram(int ptr, short data);

		void load_program(std::string bin_file);
		void load_ram(std::string bin_file);
		void erase_memory(); //fills memory with zeros	

		void ptr_check(int ptr, int capacity, std::string const commentary);

		//regs vars
		short reg_mem[CPU_REG_COUNT];
		int prog_counter;
		
		void reset_regs();
		
		//flags vars
		bool halt;
		bool zero;
		bool sign;

		//flags operations
		void reset_flags();
		void compare(int x, int y);
};
