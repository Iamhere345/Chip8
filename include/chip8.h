#include <stdint.h>
#include <string>
#include <vector>

#pragma once

class Chip8 {
	public:
	
		Chip8();
		bool load_game(char*);
		void cycle();

		uint16_t opcode;

		uint8_t memory[4096];
		uint8_t gfx[64 * 32];

		uint8_t V[16];
		uint16_t I;
		uint16_t pc;

		uint8_t delay_timer;
		uint8_t sound_timer;

		uint8_t stack[16];
		uint8_t sp;

		uint8_t key[16];
private:
		void CLS(uint16_t);		void RET(uint16_t);		void SYS(uint16_t); 	void JMP(uint16_t);
		void CALL(uint16_t);	void SE(uint16_t);		void SNE(uint16_t);		void SEY(uint16_t);
		void LD(uint16_t);		void ADD(uint16_t);		void LDY(uint16_t);		void OR(uint16_t);
		void AND(uint16_t);		void XOR(uint16_t);		void ADDY(uint16_t);	void SUB(uint16_t);
		void SHR(uint16_t);		void SUBN(uint16_t);    void SHL(uint16_t);     void SNEY(uint16_t);
		void ILDADDR(uint16_t); void JMPADDR(uint16_t); void RND(uint16_t);     void DRW(uint16_t);
		void SKP(uint16_t);     void SKNP(uint16_t);    void XLDDT(uint16_t);   void XLDK(uint16_t);
		void DTLDX(uint16_t);   void STLDX(uint16_t);   void IADDX(uint16_t);   void FLDX(uint16_t);
		void BLDX(uint16_t);    void DILDX(uint16_t);   void XLDDI(uint16_t);

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

		// TODO refactor so it looks pretty
		const LOOKUP_INFO LOOKUP_TABLE[35] = {
			{ 0x00E0, 0x00F0, &Chip8::CLS }, { 0x00EE, 0x00FF, &Chip8::RET }, { 0x0000, 0xF000, &Chip8::SYS }, { 0x1000, 0xF000, &Chip8::JMP }, { 0x2000, 0xF000, &Chip8::CALL },
			{ 0x3000, 0xF000, &Chip8::SE  }, { 0x4000, 0xF000, &Chip8::SNE }, { 0x5000, 0xF00F, &Chip8::SEY }, { 0x6000, 0xF000, &Chip8::LD  }, { 0x7000, 0xF000, &Chip8::ADD  },
			{ 0x8000, 0xF00F, &Chip8::LDY }, { 0x8001, 0xF00F, &Chip8::OR  }, { 0x8002, 0xF00F, &Chip8::ADD }, { 0x8003, 0xF00F, &Chip8::XOR }, { 0x8004, 0xF00F, &Chip8::ADDY },
			{ 0x8005, 0xF00F, &Chip8::SUB }, { 0x8006, 0xF00F, &Chip8::SHR }, { 0x8007, 0xF00F, &Chip8::SUBN}, { 0x800E, 0xF00F, &Chip8::SHL }, { 0x9000, 0xF00F, &Chip8::SNEY },

			{ 0xA000, 0xF000, &Chip8::ILDADDR }, { 0xB000, 0xF000, &Chip8::JMPADDR }, { 0xC000, 0xF000, &Chip8::RND }, { 0xD000, 0xF000, &Chip8::DRW }, { 0xE00E, 0xF00F, &Chip8::SKP },
			{ 0xE0A1, 0xF0FF, &Chip8::SKNP }, { 0xF007, 0xF0FF, &Chip8::XLDDT }, { 0xF00A, 0xF0FF, &Chip8::XLDK }, { 0xF015, 0xF0FF, &Chip8::DTLDX }, { 0xF018, 0xF0FF, &Chip8::STLDX },
			{ 0xF01E, 0xF0FF, &Chip8::IADDX}, { 0xF029, 0xF0FF, &Chip8::FLDX  }, { 0xF033, 0xF0FF, &Chip8::BLDX }, { 0xF055, 0xF0FF, &Chip8::DILDX }, { 0xF065, 0xF0FF, &Chip8::XLDDI }

		};

};