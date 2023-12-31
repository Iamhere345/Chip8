#include <stdint.h>
#include <string>
#include <vector>
#include <SDL2/SDL_scancode.h>

#pragma once

class Chip8 {
	public:
	
		Chip8();
		bool load_game(char*);
		bool dump_memory(char*);
		void cycle();
		void update_keys(SDL_Scancode, uint8_t);

		uint16_t opcode;

		uint8_t memory[4096];
		uint8_t gfx[64 * 32];

		uint8_t V[16];
		uint16_t I;
		uint16_t pc;

		uint8_t delay_timer;
		uint8_t sound_timer;

		uint16_t stack[16];
		uint8_t sp;

		uint8_t key[16];

		uint8_t flag;
		bool flag_set;

		bool halt;
		bool step;

		#ifdef CHIP8_DEBUG
		uint16_t break_addr[16];
		uint16_t break_op[16];
		bool break_active;
		#endif

private:
		void CLS(uint16_t);		void RET(uint16_t);		void SYS(uint16_t); 	void JMP(uint16_t);
		void CALL(uint16_t);	void SE(uint16_t);		void SNE(uint16_t);		void SEY(uint16_t);
		void LD(uint16_t);		void ADD(uint16_t);		void LDY(uint16_t);		void OR(uint16_t);
		void AND(uint16_t);		void XOR(uint16_t);		void ADDY(uint16_t);	void SUB(uint16_t);
		void SHR(uint16_t);		void SUBN(uint16_t);    void SHL(uint16_t);     void SNEY(uint16_t);
		void LDI(uint16_t); 	void RJMP(uint16_t); 	void RND(uint16_t);     void DRW(uint16_t);
		void KEY(uint16_t);     void NKEY(uint16_t);    void TGET(uint16_t);   	void WKEY(uint16_t);
		void TSET(uint16_t);   	void STSET(uint16_t);   void ADDI(uint16_t);   	void FONT(uint16_t);
		void DEC(uint16_t);    	void STX(uint16_t);   	void LDX(uint16_t);
	

private:

		struct LOOKUP_INFO {
			uint16_t opcode;
			uint16_t mask;
			void (Chip8::*instruction)(uint16_t);
		};

		const uint8_t FONTSET[80] = {
			0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
			0x20, 0x60, 0x20, 0x20, 0x70, // 1
			0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
			0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
			0x90, 0x90, 0xF0, 0x10, 0x10, // 4
			0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
			0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
			0xF0, 0x10, 0x20, 0x40, 0x40, // 7
			0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
			0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
			0xF0, 0x90, 0xF0, 0x90, 0x90, // A
			0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
			0xF0, 0x80, 0x80, 0x80, 0xF0, // C
			0xE0, 0x90, 0x90, 0x90, 0xE0, // D
			0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
			0xF0, 0x80, 0xF0, 0x80, 0x80  // F
		};

		// opcodes that don't have any data bits should have the mask as 0xFFFF
		const LOOKUP_INFO LOOKUP_TABLE[35] = {
			{ 0x00E0, 0xFFFF, &Chip8::CLS }, { 0x00EE, 0xFFFF, &Chip8::RET }, { 0x0000, 0xFFFF, &Chip8::SYS }, { 0x1000, 0xF000, &Chip8::JMP }, { 0x2000, 0xF000, &Chip8::CALL },
			{ 0x3000, 0xF000, &Chip8::SE  }, { 0x4000, 0xF000, &Chip8::SNE }, { 0x5000, 0xF00F, &Chip8::SEY }, { 0x6000, 0xF000, &Chip8::LD  }, { 0x7000, 0xF000, &Chip8::ADD  },
			{ 0x8000, 0xF00F, &Chip8::LDY }, { 0x8001, 0xF00F, &Chip8::OR  }, { 0x8002, 0xF00F, &Chip8::AND }, { 0x8003, 0xF00F, &Chip8::XOR }, { 0x8004, 0xF00F, &Chip8::ADDY },
			{ 0x8005, 0xF00F, &Chip8::SUB }, { 0x8006, 0xF00F, &Chip8::SHR }, { 0x8007, 0xF00F, &Chip8::SUBN}, { 0x800E, 0xF00F, &Chip8::SHL }, { 0x9000, 0xF00F, &Chip8::SNEY },

			{ 0xA000, 0xF000, &Chip8::LDI }, { 0xB000, 0xF000, &Chip8::RJMP }, { 0xC000, 0xF000, &Chip8::RND }, { 0xD000, 0xF000, &Chip8::DRW }, { 0xE09E, 0xF0FF, &Chip8::KEY },
			{ 0xE0A1, 0xF0FF, &Chip8::NKEY }, { 0xF007, 0xF0FF, &Chip8::TGET }, { 0xF00A, 0xF0FF, &Chip8::WKEY }, { 0xF015, 0xF0FF, &Chip8::TSET }, { 0xF018, 0xF0FF, &Chip8::STSET },
			{ 0xF01E, 0xF0FF, &Chip8::ADDI}, { 0xF029, 0xF0FF, &Chip8::FONT  }, { 0xF033, 0xF0FF, &Chip8::DEC }, { 0xF055, 0xF0FF, &Chip8::STX }, { 0xF065, 0xF0FF, &Chip8::LDX }

		};

};