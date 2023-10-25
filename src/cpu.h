#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "mapper.h"

#define STACK_START 0x100
#define NIL_OP {XXX, IMP, 0}

struct Instruction;
// from https://dwheeler.com/6502/oneelkruns/asm1step.html
typedef enum {
	N = 1 << 7,
	V = 1 << 6,
	U = 1 << 5,
	B = 1 << 4,
	D = 1 << 3,
	I = 1 << 2,
	Z = 1 << 1,
	C = 1 << 0,
} ProcessorStatus;

typedef struct {
	uint32_t cycles_t;
	uint8_t cycles;
	uint16_t addr;

	uint16_t pc;
	uint8_t sp;
	uint8_t sr;
	uint8_t a;
	uint8_t x;
	uint8_t y;

	uint8_t internal_ram[0x800];
	struct mapper *mapper;
	uint8_t memory[0x10000];
	struct Instruction *instruction;
} cpu6502;

struct Instruction{
	void (*instruction)(cpu6502 *cpu);
	uint8_t (*mode)(cpu6502 *cpu);
	uint8_t cycles;
};

uint8_t read_ppuflags(cpu6502 *cpu, uint16_t addr);
uint8_t read_apuflags(cpu6502 *cpu, uint16_t addr);
void write_ppuflags(cpu6502 *cpu, uint16_t addr, uint8_t data);
void write_apuflags(cpu6502 *cpu, uint16_t addr, uint8_t data);

uint8_t read_cpu(cpu6502 *cpu, uint16_t addr);
void write_cpu(cpu6502 *cpu, uint16_t addr, uint8_t data);

void setFlag(cpu6502 *cpu, ProcessorStatus f, bool b);
uint8_t getFlag(cpu6502 *cpu, ProcessorStatus f);

void initialize_cpu(cpu6502 *cpu);
void clock_cycle(cpu6502 *cpu);

void execute_cpu(cpu6502 *cpu);

uint8_t IMP(cpu6502 *cpu); // Implicit
uint8_t ACC(cpu6502 *cpu); // Accumulator
uint8_t IMM(cpu6502 *cpu); // Immediate
uint8_t ZPG(cpu6502 *cpu); // Zero Page
uint8_t ZPX(cpu6502 *cpu); // Zero Page X
uint8_t ZPY(cpu6502 *cpu); // Zero Page Y
uint8_t REL(cpu6502 *cpu); // Relative
uint8_t ABS(cpu6502 *cpu); // Absolute
uint8_t ABX(cpu6502 *cpu); // Absolute X
uint8_t ABY(cpu6502 *cpu); // Absolute Y
uint8_t IND(cpu6502 *cpu); // Indirect
uint8_t IDX(cpu6502 *cpu); // Indexed Indirect (X)
uint8_t IDY(cpu6502 *cpu); // Indirect Indexed (Y)



	// Load/Store Operations
void LDA(cpu6502 *cpu);
void LDX(cpu6502 *cpu);
void LDY(cpu6502 *cpu);
void STA(cpu6502 *cpu);
void STX(cpu6502 *cpu);
void STY(cpu6502 *cpu);

	// Register Transfers
void TAX(cpu6502 *cpu);
void TAY(cpu6502 *cpu);
void TXA(cpu6502 *cpu);
void TYA(cpu6502 *cpu);

	// Stack Operations
void TSX(cpu6502 *cpu);
void TXS(cpu6502 *cpu);
void PHA(cpu6502 *cpu);
void PHP(cpu6502 *cpu);
void PLA(cpu6502 *cpu);
void PLP(cpu6502 *cpu);

	// Logical
void AND(cpu6502 *cpu);
void EOR(cpu6502 *cpu);
void ORA(cpu6502 *cpu);
void BIT(cpu6502 *cpu);

	// Arithmetic
void ADC(cpu6502 *cpu);
void SBC(cpu6502 *cpu);
void CMP(cpu6502 *cpu);
void CPX(cpu6502 *cpu);
void CPY(cpu6502 *cpu);

	// Increments and Decrements
void INC(cpu6502 *cpu);
void INX(cpu6502 *cpu);
void INY(cpu6502 *cpu);
void DEC(cpu6502 *cpu);
void DEX(cpu6502 *cpu);
void DEY(cpu6502 *cpu);

	// Shifts
void ASL(cpu6502 *cpu);
void LSR(cpu6502 *cpu);
void ROL(cpu6502 *cpu);
void ROR(cpu6502 *cpu);

void JMP(cpu6502 *cpu);
void JSR(cpu6502 *cpu);
void RTS(cpu6502 *cpu);
	// Branches
void BCC(cpu6502 *cpu);
void BCS(cpu6502 *cpu);
void BEQ(cpu6502 *cpu);
void BMI(cpu6502 *cpu);
void BNE(cpu6502 *cpu);
void BPL(cpu6502 *cpu);
void BVC(cpu6502 *cpu);
void BVS(cpu6502 *cpu);

	// Status Flag Changes
void CLC(cpu6502 *cpu);
void CLD(cpu6502 *cpu);
void CLI(cpu6502 *cpu);
void CLV(cpu6502 *cpu);
void SEC(cpu6502 *cpu);
void SED(cpu6502 *cpu);
void SEI(cpu6502 *cpu);

	// System Functions
void BRK(cpu6502 *cpu);
void NOP(cpu6502 *cpu);
void RTI(cpu6502 *cpu);

void XXX(cpu6502 *cpu);

void initialize_cpu(cpu6502 *cpu);
void clock_cycle(cpu6502 *cpu);

#endif