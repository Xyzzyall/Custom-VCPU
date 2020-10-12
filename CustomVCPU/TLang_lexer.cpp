#include "TLang_lexer.h"
#include <stack>
#include "VCPU_Console.h"
#include "Builder.h"
#include "CPU.h"


namespace software {
	std::vector<Lexer::token> software::TLang_lexer::lexerify()
	{
		std::vector<token> result = std::vector<token>();

		#define push_tkn(tk, data) result.push_back(Lexer::create_token(tk, data))
		
		std::stack<tag> tag_stack = std::stack<tag>();
		int size = tags.size();
		
		for (int i = size - 1; i >= 0; i--) {
			tag_stack.push(tags[i].copy());
		}
		int stack_start_size = tag_stack.size();
		#define lex_excp(given_tag, exp_tags, ...) throw_expected_tag_exc(new int[exp_tags]{ __VA_ARGS__ }, given_tag.tag_name, stack_start_size - tag_stack.size())

		while (!tag_stack.empty()) {
			tag t = tag_stack.top();
			tag_stack.pop();
			tag next_t;

			switch (t.tag_name) {
			case TAG_UNKNOWN:
				throw lexer_exception("Unknown tag '" + t.contains + "'.");
			#pragma region Console commands
			case TAG_MODE:
				next_t = tag_stack.top();
				switch (next_t.tag_name) {
				case TAG_COMPILER:
					push_tkn(TOKEN_CONSOLE, CONCMD_SET_COMPILER_MODE);
					break;
				case TAG_CMD:
					push_tkn(TOKEN_CONSOLE, CONCMD_SET_CONSOLE_MODE);
					break;
				default:
					lex_excp(next_t, 2,TAG_COMPILER, TAG_CMD);
				}
				tag_stack.pop();
				break;
			case TAG_SAVEBIN:
				next_t = tag_stack.top();
				result.push_back(Lexer::create_token_with_str(TOKEN_CONSOLE, CONCMD_SAVEBIN, std::string(next_t.contains)));
				tag_stack.pop();
				break;
			case TAG_EXIT:
				push_tkn(TOKEN_CONSOLE, CONCMD_EXIT);
				break;
			#pragma endregion
			#pragma region CPU settings commands
			case TAG_SETTINGS:
				push_tkn(TOKEN_BUILDER, BLDCMD_SETTINGS);
				break;
			case TAG_PROG_MEM_SIZE:
			case TAG_RAM_MEM_SIZE:
				if (t.tag_name == TAG_RAM_MEM_SIZE) push_tkn(TOKEN_BUILDER, BLDCMD_SET_RAM_MEM);
				if (t.tag_name == TAG_PROG_MEM_SIZE) push_tkn(TOKEN_BUILDER, BLDCMD_SET_PROG_MEM);
				next_t = tag_stack.top();
				if (next_t.tag_name == TAG_NUMBER) {
					push_tkn(TOKEN_BUILDER, std::stoi(next_t.contains));
				}
				else {
					lex_excp(next_t, 1, TAG_NUMBER);
				}
				tag_stack.pop();
				break;
			case TAG_END:
				push_tkn(TOKEN_COMMON, CONCMD_END);
				break;
			case TAG_PROGRAM:
				push_tkn(TOKEN_BUILDER, BLDCMD_PROGRAM_START);
				break;
			case TAG_RAM:
				push_tkn(TOKEN_BUILDER, BLDCMD_RAM_START);
				break;
			case TAG_COMPILE:
				push_tkn(TOKEN_BUILDER, BLDCMD_COMPILE);
				break;
			#pragma endregion
#pragma region RAM commands
			case TAG_DEC:
				next_t = tag_stack.top();
				if (next_t.tag_name == TAG_NUMBER) {
					push_tkn(TOKEN_RAM_DATA, std::stoi(next_t.contains));
				}
				else {
					lex_excp(next_t, 1, TAG_NUMBER);
				}
				tag_stack.pop();
				break;
#pragma endregion
#pragma region Links
			case TAG_SHARP:
				next_t = tag_stack.top();
				//since I cannot know where the link is, I have to put both tokens. Builder will deal with it
				push_tkn(TOKEN_RAM_LINK, get_ram_link(next_t.contains));
				push_tkn(TOKEN_PROGMEM_LINK, get_progmem_link(next_t.contains));
				tag_stack.pop();
				break;
#pragma endregion
#pragma region CPU commands
			#pragma region read command
			case TAG_READ:
				next_t = tag_stack.top();
				switch (next_t.tag_name) {
				case TAG_R_SHARP:
					push_tkn(TOKEN_COMMAND, hardware::CPU::CMD_READ);
					tag_stack.pop();
					next_t = tag_stack.top();
					push_tkn(TOKEN_PUT_RAM_LINK, get_ram_link(next_t.contains));
					break;
				case TAG_INDEX:
					push_tkn(TOKEN_COMMAND, hardware::CPU::CMD_READ_INDEX);
					tag_stack.pop();
					next_t = tag_stack.top();
					if (next_t.tag_name == TAG_R_SHARP) {
						tag_stack.pop();
						next_t = tag_stack.top();
						push_tkn(TOKEN_PUT_RAM_LINK, get_ram_link(next_t.contains));
					}
					else {
						lex_excp(next_t, 1, TAG_R_SHARP);
					}
					break;
				default:
					lex_excp(next_t, 2, TAG_R_SHARP, TAG_INDEX);
				}
				tag_stack.pop();
				break;
			#pragma endregion
			#pragma region copy commands
			case TAG_COPY:
				next_t = tag_stack.top();
				if (next_t.tag_name == TAG_REG) {
					tag_stack.pop();
					next_t = tag_stack.top();
					if (next_t.tag_name == TAG_NUMBER) { //maybe better use int buffer? :)
						if (std::stoi(next_t.contains) == 0) {
							tag_stack.pop();
							if (tag_stack.top().tag_name != TAG_TO) lex_excp(tag_stack.top(), TAG_TO);
							tag_stack.pop();
							if (tag_stack.top().tag_name != TAG_REG) lex_excp(tag_stack.top(), TAG_REG);
							tag_stack.pop();
							next_t = tag_stack.top();
							if (next_t.tag_name != TAG_NUMBER) lex_excp(next_t, TAG_NUMBER);
							push_tkn(TOKEN_COMMAND, hardware::CPU::CMD_COPY_REG0_TO_REGX + std::stoi(next_t.contains));
						}
						else {
							tag_stack.pop();
							if (tag_stack.top().tag_name != TAG_TO) lex_excp(tag_stack.top(), TAG_TO);
							tag_stack.pop();
							if (tag_stack.top().tag_name != TAG_REG) lex_excp(tag_stack.top(), TAG_REG);
							tag_stack.pop();
							next_t = tag_stack.top();
							if (next_t.tag_name != TAG_NUMBER) lex_excp(next_t, TAG_NUMBER);
							push_tkn(TOKEN_COMMAND, hardware::CPU::CMD_COPY_REGX_TO_REG0 + std::stoi(next_t.contains));
						}
						tag_stack.pop();
					} 
					else lex_excp(next_t, 1, TAG_NUMBER);
				}
				else {
					lex_excp(next_t, 1, TAG_REG);
				}
				break;
			#pragma endregion
			#pragma region cmp commands
			case TAG_COMPARE:
				next_t = tag_stack.top();
				switch (next_t.tag_name)
				{
				case TAG_REG:
					tag_stack.pop();
					next_t = tag_stack.top();
					if (next_t.tag_name == TAG_NUMBER) {
						push_tkn(TOKEN_COMMAND, hardware::CPU::CMD_CMP_REGX + std::stoi(next_t.contains));
					}
					else {
						lex_excp(next_t, 1, TAG_NUMBER);
					}
					break;
				case TAG_INDEX:
					push_tkn(TOKEN_COMMAND, hardware::CPU::CMD_CMP_ARRAY);
					break;
				default:
					lex_excp(next_t, 2, TAG_INDEX, TAG_REG);
				}
				tag_stack.pop();
				break;
			#pragma endregion
			#pragma region inc regx command
			case TAG_INC:
				next_t = tag_stack.top();
				switch (next_t.tag_name)
				{
				case TAG_REG:
					tag_stack.pop();
					next_t = tag_stack.top();
					if (next_t.tag_name == TAG_NUMBER) {
						push_tkn(TOKEN_COMMAND, hardware::CPU::CMD_INC_REGX + std::stoi(next_t.contains));
					}
					else {
						lex_excp(next_t, 1, TAG_NUMBER);
					}
					break;
				case TAG_INDEX:
					push_tkn(TOKEN_COMMAND, hardware::CPU::CMD_INC_REGX + CPU_INDEX_REG);
					break;
				default:
					lex_excp(next_t, 2, TAG_REG, TAG_INDEX);
				}
				tag_stack.pop();
				break;
			#pragma endregion
			#pragma region JUMP commands
			case TAG_JUMP:
				next_t = tag_stack.top();
				switch (next_t.tag_name)
				{
				case TAG_P_SHARP:
					push_tkn(TOKEN_COMMAND, hardware::CPU::CMD_JUMP);
					tag_stack.pop();
					//if (tag_stack.top().tag_name != TAG_P_SHARP) lex_excp(tag_stack.top(), 1, TAG_P_SHARP);
					//tag_stack.pop();
					next_t = tag_stack.top();
					push_tkn(TOKEN_PUT_PROGMEM_LINK, get_progmem_link(next_t.contains));
					break;
				case TAG_SIGN:
					push_tkn(TOKEN_COMMAND, hardware::CPU::CMD_JUMP_SIGN);
					tag_stack.pop();
					if (tag_stack.top().tag_name != TAG_P_SHARP) lex_excp(tag_stack.top(), 1, TAG_P_SHARP);
					tag_stack.pop();
					next_t = tag_stack.top();
					push_tkn(TOKEN_PUT_PROGMEM_LINK, get_progmem_link(next_t.contains));
					break;
				case TAG_NSIGN:
					push_tkn(TOKEN_COMMAND, hardware::CPU::CMD_JUMP_NSIGN);
					tag_stack.pop();
					if (tag_stack.top().tag_name != TAG_P_SHARP) lex_excp(tag_stack.top(), 1, TAG_P_SHARP);
					tag_stack.pop();
					next_t = tag_stack.top();
					push_tkn(TOKEN_PUT_PROGMEM_LINK, get_progmem_link(next_t.contains));
					break;
				case TAG_ZERO:
					push_tkn(TOKEN_COMMAND, hardware::CPU::CMD_JUMP_ZERO);
					tag_stack.pop();
					if (tag_stack.top().tag_name != TAG_P_SHARP) lex_excp(tag_stack.top(), 1, TAG_P_SHARP);
					tag_stack.pop();
					next_t = tag_stack.top();
					push_tkn(TOKEN_PUT_PROGMEM_LINK, get_progmem_link(next_t.contains));
					break;
				case TAG_NZERO:
					push_tkn(TOKEN_COMMAND, hardware::CPU::CMD_JUMP_NZERO);
					tag_stack.pop();
					if (tag_stack.top().tag_name != TAG_P_SHARP) lex_excp(tag_stack.top(), 1, TAG_P_SHARP);
					tag_stack.pop();
					next_t = tag_stack.top();
					push_tkn(TOKEN_PUT_PROGMEM_LINK, get_progmem_link(next_t.contains));
					break;
				default:
					lex_excp(next_t, TAG_P_SHARP, TAG_SIGN, TAG_NSIGN, TAG_ZERO, TAG_NZERO);
				}
				tag_stack.pop();
				break;
			#pragma endregion
			#pragma region HALT command
			case TAG_HALT:
				push_tkn(TOKEN_COMMAND, hardware::CPU::CMD_HALT);
				break;
			#pragma endregion
#pragma endregion
			default:
				throw lexer_exception("Unexpected tag '" + t.contains + "'.");
			}
		}

		//TODO: delete this
		clear_links_buffer();
		tags.clear();

		return result;
	}
}

