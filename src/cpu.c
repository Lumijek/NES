#include <unistd.h>

#include "cpu.h"


struct Instruction instructionLookup[256] =
{
	{&BRK, &IMP, 7}, {&ORA, &IDX, 6}, NIL_OP         , NIL_OP         , NIL_OP         , {&ORA, &ZPG, 3}, {&ASL, &ZPG, 5}, NIL_OP         , {&PHP, &IMP, 3}, {&ORA, &IMM, 2}, {&ASL, &ACC, 2}, NIL_OP         , NIL_OP         , {&ORA, &ABS, 4}, {&ASL, &ABS, 6}, NIL_OP,  
	{&BPL, &REL, 2}, {&ORA, &IDY, 5}, NIL_OP         , NIL_OP         , NIL_OP         , {&ORA, &ZPX, 4}, {&ASL, &ZPX, 6}, NIL_OP         , {&CLC, &IMP, 2}, {&ORA, &ABY, 4}, NIL_OP         , NIL_OP         , NIL_OP         , {&ORA, &ABX, 4}, {&ASL, &ABX, 7}, NIL_OP,
	{&JSR, &ABS, 6}, {&AND, &IDX, 6}, NIL_OP         , NIL_OP         , {&BIT, &ZPG, 3}, {&AND, &ZPG, 3}, {&ROL, &ZPG, 5}, NIL_OP         , {&PLP, &IMP, 4}, {&AND, &IMM, 2}, {&ROL, &ACC, 2}, NIL_OP         , {&BIT, &ABS, 4}, {&AND, &ABS, 4}, {&ROL, &ABS, 6}, NIL_OP,
	{&BMI, &REL, 2}, {&AND, &IDY, 5}, NIL_OP         , NIL_OP         , NIL_OP         , {&AND, &ZPX, 4}, {&ROL, &ZPX, 6}, NIL_OP         , {&SEC, &IMP, 2}, {&AND, &ABY, 4}, NIL_OP         , NIL_OP         , NIL_OP         , {&AND, &ABX, 4}, {&ROL, &ABX, 7}, NIL_OP,
	{&RTI, &IMP, 6}, {&EOR, &IDX, 6}, NIL_OP         , NIL_OP         , NIL_OP         , {&EOR, &ZPG, 3}, {&LSR, &ZPG, 5}, NIL_OP         , {&PHA, &IMP, 3}, {&EOR, &IMM, 2}, {&LSR, &ACC, 2}, NIL_OP         , {&JMP, &ABS, 3}, {&EOR, &ABS, 4}, {&LSR, &ABS, 6}, NIL_OP,
	{&BVC, &REL, 2}, {&EOR, &IDY, 5}, NIL_OP         , NIL_OP         , NIL_OP         , {&EOR, &ZPX, 4}, {&LSR, &ZPX, 6}, NIL_OP         , {&CLI, &IMP, 2}, {&EOR, &ABY, 4}, NIL_OP         , NIL_OP         , NIL_OP         , {&EOR, &ABX, 4}, {&LSR, &ABX, 7}, NIL_OP,
	{&RTS, &IMP, 6}, {&ADC, &IDX, 6}, NIL_OP         , NIL_OP         , NIL_OP         , {&ADC, &ZPG, 3}, {&ROR, &ZPG, 5}, NIL_OP         , {&PLA, &IMP, 4}, {&ADC, &IMM, 2}, {&ROR, &ACC, 2}, NIL_OP         , {&JMP, &IND, 5}, {&ADC, &ABS, 4}, {&ROR, &ABS, 6}, NIL_OP,
	{&BVS, &REL, 2}, {&ADC, &IDY, 5}, NIL_OP         , NIL_OP         , NIL_OP         , {&ADC, &ZPX, 4}, {&ROR, &ZPX, 6}, NIL_OP         , {&SEI, &IMP, 2}, {&ADC, &ABY, 4}, NIL_OP         , NIL_OP         , NIL_OP         , {&ADC, &ABX, 4}, {&ROR, &ABX, 7}, NIL_OP,
	NIL_OP         , {&STA, &IDX, 6}, NIL_OP         , NIL_OP         , {&STY, &ZPG, 3}, {&STA, &ZPG, 3}, {&STX, &ZPG, 3}, NIL_OP         , {&DEY, &IMP, 2}, NIL_OP         , {&TXA, &IMP, 2}, NIL_OP         , {&STY, &ABS, 4}, {&STA, &ABS, 4}, {&STX, &ABS, 4}, NIL_OP,
	{&BCC, &REL, 2}, {&STA, &IDY, 6}, NIL_OP         , NIL_OP         , {&STY, &ZPX, 4}, {&STA, &ZPX, 4}, {&STX, &ZPY, 4}, NIL_OP         , {&TYA, &IMP, 2}, {&STA, &ABY, 5}, {&TXS, &IMP, 2}, NIL_OP         , NIL_OP         , {&STA, &ABX, 5}, NIL_OP         , NIL_OP,
	{&LDY, &IMM, 2}, {&LDA, &IDX, 6}, {&LDX, &IMM, 2}, NIL_OP         , {&LDY, &ZPG, 3}, {&LDA, &ZPG, 3}, {&LDX, &ZPG, 3}, NIL_OP         , {&TAY, &IMP, 2}, {&LDA, &IMM, 2}, {&TAX, &IMP, 2}, NIL_OP         , {&LDY, &ABS, 4}, {&LDA, &ABS, 4}, {&LDX, &ABS, 4}, NIL_OP,
	{&BCS, &REL, 2}, {&LDA, &IDY, 5}, NIL_OP         , NIL_OP         , {&LDY, &ZPX, 4}, {&LDA, &ZPX, 4}, {&LDX, &ZPY, 4}, NIL_OP         , {&CLV, &IMP, 2}, {&LDA, &ABY, 4}, {&TSX, &IMP, 2}, NIL_OP         , {&LDY, &ABX, 4}, {&LDA, &ABX, 4}, {&LDX, &ABY, 4}, NIL_OP,
	{&CPY, &IMM, 2}, {&CMP, &IDX, 6}, NIL_OP         , NIL_OP         , {&CPY, &ZPG, 3}, {&CMP, &ZPG, 3}, {&DEC, &ZPG, 5}, NIL_OP         , {&INY, &IMP, 2}, {&CMP, &IMM, 2}, {&DEX, &IMP, 2}, NIL_OP         , {&CPY, &ABS, 4}, {&CMP, &ABS, 4}, {&DEC, &ABS, 6}, NIL_OP,
	{&BNE, &REL, 2}, {&CMP, &IDY, 5}, NIL_OP         , NIL_OP         , NIL_OP         , {&CMP, &ZPX, 4}, {&DEC, &ZPX, 6}, NIL_OP         , {&CLD, &IMP, 2}, {&CMP, &ABY, 4}, NIL_OP         , NIL_OP         , NIL_OP         , {&CMP, &ABX, 4}, {&DEC, &ABX, 7}, NIL_OP,
	{&CPX, &IMM, 2}, {&SBC, &IDX, 6}, NIL_OP         , NIL_OP         , {&CPX, &ZPG, 3}, {&SBC, &ZPG, 3}, {&INC, &ZPG, 5}, NIL_OP         , {&INX, &IMP, 2}, {&SBC, &IMM, 2}, {&NOP, &IMP, 2}, NIL_OP         , {&CPX, &ABS, 4}, {&SBC, &ABS, 4}, {&INC, &ABS, 6}, NIL_OP,
	{&BEQ, &REL, 2}, {&SBC, &IDY, 5}, NIL_OP         , NIL_OP         , NIL_OP         , {&SBC, &ZPX, 4}, {&INC, &ZPX, 6}, NIL_OP         , {&SED, &IMP, 2}, {&SBC, &ABY, 4}, NIL_OP         , NIL_OP         , NIL_OP         , {&SBC, &ABX, 4}, {&INC, &ABX, 7}, NIL_OP,
};

// HALF THESE INSTRUCTIONS COULD PROBABLY BE ABSTRACTED AWAY
// WITH FUNCTIONS BUT I WANTED THE OPCODES TO BE AS EXPLICIT
// AS POSSIBLE
uint16_t read_mem(cpu6502 *cpu, uint16_t addr) {
	return cpu->memory[addr];
}

void write_mem(cpu6502 *cpu, uint16_t addr, uint8_t data) {
	cpu->memory[addr] = data;
}

void setFlag(cpu6502 *cpu, ProcessorStatus f, bool b) {
	if(b)
		cpu->sr |= f;
	else
		cpu->sr &= ~f;
}
uint8_t getFlag(cpu6502 *cpu, ProcessorStatus f) {
	return (cpu->sr & f) > 0; 
}

uint8_t IMP(cpu6502 *cpu) {
	return 0;
}

uint8_t ACC(cpu6502 *cpu) {
	return 0;
}

uint8_t IMM(cpu6502 *cpu) {
	cpu->addr = cpu->pc++;
	return 0;
}

uint8_t ZPG(cpu6502 *cpu) {
	cpu->addr = read_mem(cpu, cpu->pc++) & 0x00FF;
	return 0;
}

uint8_t ZPX(cpu6502 *cpu) {
	cpu->addr = read_mem(cpu, cpu->pc++);
	cpu->addr = (cpu->addr + cpu->x) & 0x00FF;
	return 0;
}

uint8_t ZPY(cpu6502 *cpu) {
	cpu->addr = read_mem(cpu, cpu->pc++);
	cpu->addr = (cpu->addr + cpu->y) & 0x00FF;
	return 0;
}

uint8_t REL(cpu6502 *cpu) {
	cpu->addr = read_mem(cpu, cpu->pc++);
	if(cpu->addr & 0x80) // if number is negative in 2's complement
		cpu->addr |= 0xFF00; //add 8 1 bits to keep it negative in 2's complement in 2 bytes
	return 0;
}

uint8_t ABS(cpu6502 *cpu) {
	int16_t lo = read_mem(cpu, cpu->pc++);
	int16_t hi = read_mem(cpu, cpu->pc++);
	cpu->addr = (hi << 8) | lo;
	return 0;
}

uint8_t ABX(cpu6502 *cpu) {
	int16_t lo = read_mem(cpu, cpu->pc++);
	int16_t hi = read_mem(cpu, cpu->pc++);
	cpu->addr = ((hi << 8) | lo) + cpu->x;
	if ((cpu->addr & 0xFF00) != (hi << 8))
		cpu->cycles++;
	return 0;
}

uint8_t ABY(cpu6502 *cpu) {
	int16_t lo = read_mem(cpu, cpu->pc++);
	int16_t hi = read_mem(cpu, cpu->pc++);
	cpu->addr = ((hi << 8) | lo) + cpu->y;
	if ((cpu->addr & 0xFF00) != (hi << 8))
		cpu->cycles++;
	return 0;
}

uint8_t IND(cpu6502 *cpu) {
	int16_t lo = read_mem(cpu, cpu->pc++);
	int16_t hi = read_mem(cpu, cpu->pc++);
	int16_t pointer = (hi << 8) | lo;

	if(lo == 0x00FF) {
		cpu->addr = (read_mem(cpu, pointer & 0xFF00) << 8) | read_mem(cpu, pointer);
	}
	else {
		cpu->addr = (read_mem(cpu, pointer + 1) << 8) | read_mem(cpu, pointer);
	}
	return 0;
}

uint8_t IDX(cpu6502 *cpu) {
	uint16_t temp_addr = (read_mem(cpu, cpu->pc++) + cpu->x) & 0x00FF;
	cpu->addr = (read_mem(cpu, (temp_addr + 1) & 0x00FF) << 8) | read_mem(cpu, temp_addr);
	return 0;
}

uint8_t IDY(cpu6502 *cpu) {
	uint16_t temp_addr = read_mem(cpu, cpu->pc++);
	uint16_t lo = read_mem(cpu, temp_addr);
	uint16_t hi = read_mem(cpu, (temp_addr + 1) & 0x00FF);
	cpu->addr = ((hi << 8) | lo) + cpu->y;
	if ((cpu->addr & 0xFF00) != (hi << 8))
		cpu->cycles++;
	return 0;

}

// Load/Store Operations
void LDA(cpu6502 *cpu) {
	cpu->a = read_mem(cpu, cpu->addr);
	setFlag(cpu, Z, cpu->a == 0x00);
	setFlag(cpu, N, cpu->a & 0x80);
}

void LDX(cpu6502 *cpu) {
	cpu->x = read_mem(cpu, cpu->addr);
	setFlag(cpu, Z, cpu->x == 0x00);
	setFlag(cpu, N, cpu->x & 0x80);
}

void LDY(cpu6502 *cpu) {
	cpu->y = read_mem(cpu, cpu->addr);
	setFlag(cpu, Z, cpu->y == 0x00);
	setFlag(cpu, N, cpu->y & 0x80);
}

void STA(cpu6502 *cpu) {
	write_mem(cpu, cpu->addr, cpu->a);
}

void STX(cpu6502 *cpu) {
	write_mem(cpu, cpu->addr, cpu->x);
}

void STY(cpu6502 *cpu) {
	write_mem(cpu, cpu->addr, cpu->y);
}

// Register Transfers
void TAX(cpu6502 *cpu) {
	cpu->x = cpu->a;
	setFlag(cpu, Z, cpu->x == 0x00);
	setFlag(cpu, N, cpu->x & 0x80);
}

void TAY(cpu6502 *cpu) {
	cpu->y = cpu->a;
	setFlag(cpu, Z, cpu->y == 0x00);
	setFlag(cpu, N, cpu->y & 0x80);

}
void TXA(cpu6502 *cpu) {
	cpu->a = cpu->x;
	setFlag(cpu, Z, cpu->a == 0x00);
	setFlag(cpu, N, cpu->a & 0x80);

}
void TYA(cpu6502 *cpu) {
	cpu->a = cpu->y;
	setFlag(cpu, Z, cpu->a == 0x00);
	setFlag(cpu, N, cpu->a & 0x80);

}

// Stack Operations
void TSX(cpu6502 *cpu) {
	cpu->x = cpu->sp;
	setFlag(cpu, Z, cpu->x == 0x00);
	setFlag(cpu, N, cpu->x & 0x80);

}

void TXS(cpu6502 *cpu) {
	cpu->sp = cpu->x;
}

void PHA(cpu6502 *cpu) {
	write_mem(cpu, STACK_START + cpu->sp--, cpu->a);
}

void PHP(cpu6502 *cpu) {
	setFlag(cpu, U, 1);
	setFlag(cpu, B, 1);

	write_mem(cpu, STACK_START + cpu->sp--, cpu->sr);

	setFlag(cpu, B, 0); // Hardware bugs
}

void PLA(cpu6502 *cpu) {
	cpu->a = read_mem(cpu, STACK_START + ++cpu->sp);
	setFlag(cpu, Z, cpu->a == 0x00);
	setFlag(cpu, N, cpu->a & 0x80);
}

void PLP(cpu6502 *cpu) {
	cpu->sr = read_mem(cpu, STACK_START + ++cpu->sp);
	setFlag(cpu, U, 1);
	setFlag(cpu, B, 0);
}

// Logical
void AND(cpu6502 *cpu) {
	cpu->a &= read_mem(cpu, cpu->addr);
	setFlag(cpu, Z, cpu->a == 0x00);
	setFlag(cpu, N, cpu->a & 0x80);
}

void EOR(cpu6502 *cpu) {
	cpu->a ^= read_mem(cpu, cpu->addr);
	setFlag(cpu, Z, cpu->a == 0x00);
	setFlag(cpu, N, cpu->a & 0x80);
}

void ORA(cpu6502 *cpu) {
	cpu->a |= read_mem(cpu, cpu->addr);
	setFlag(cpu, Z, cpu->a == 0x00);
	setFlag(cpu, N, cpu->a & 0x80);
}

void BIT(cpu6502 *cpu) {
	uint8_t mem = read_mem(cpu, cpu->addr);
	setFlag(cpu, Z, (mem & cpu->a) == 0x00);
	setFlag(cpu, V, mem & (1 << 6));
	setFlag(cpu, N, mem & (1 << 7));
}

// Arithmetic
void ADC(cpu6502 *cpu) {
	uint8_t mem = read_mem(cpu, cpu->addr);
	uint16_t temp = cpu->a + mem + getFlag(cpu, C);

	setFlag(cpu, C, temp > 255);
	setFlag(cpu, V, ((cpu->a ^ temp) & (mem ^ temp) & 0x80) > 0);

	cpu->a = temp & 0xFF;
	setFlag(cpu, Z, cpu->a == 0x00);
	setFlag(cpu, N, cpu->a & 0x80);


}

void SBC(cpu6502 *cpu) {
	uint8_t mem = read_mem(cpu, cpu->addr);
	mem = mem ^ 0xFF;
	uint16_t temp = cpu->a + mem + getFlag(cpu, C);

	setFlag(cpu, C, temp > 255);
	setFlag(cpu, V, ((cpu->a ^ temp) & (mem ^ temp) & 0x80) > 0);

	cpu->a = temp & 0xFF;
	setFlag(cpu, Z, cpu->a == 0x00);
	setFlag(cpu, N, cpu->a & 0x80);

}

void CMP(cpu6502 *cpu) {
	uint8_t mem = read_mem(cpu, cpu->addr);
	setFlag(cpu, C, cpu->a >= mem);
	setFlag(cpu, Z, cpu->a == mem);
	setFlag(cpu, N, (cpu->a - mem) & 0x80);

}

void CPX(cpu6502 *cpu) {
	uint8_t mem = read_mem(cpu, cpu->addr);
	setFlag(cpu, C, cpu->x >= mem);
	setFlag(cpu, Z, cpu->x == mem);
	setFlag(cpu, N, (cpu->x - mem) & 0x80);

}

void CPY(cpu6502 *cpu) {
	uint8_t mem = read_mem(cpu, cpu->addr);
	setFlag(cpu, C, cpu->y >= mem);
	setFlag(cpu, Z, cpu->y == mem);
	setFlag(cpu, N, (cpu->y - mem) & 0x80);
}

// Increments and Decrements
void INC(cpu6502 *cpu) {
	uint8_t mem = read_mem(cpu, cpu->addr) + 1;
	write_mem(cpu, cpu->addr, mem);
	setFlag(cpu, Z, mem == 0x00);
	setFlag(cpu, N, mem & 0x80);

}

void INX(cpu6502 *cpu) {
	cpu->x++;
	setFlag(cpu, Z, cpu->x == 0x00);
	setFlag(cpu, N, cpu->x & 0x80);
}

void INY(cpu6502 *cpu) {
	cpu->y++;
	setFlag(cpu, Z, cpu->y == 0x00);
	setFlag(cpu, N, cpu->y & 0x80);
}

void DEC(cpu6502 *cpu) {
	uint8_t mem = read_mem(cpu, cpu->addr) - 1;
	write_mem(cpu, cpu->addr, mem);
	setFlag(cpu, Z, mem == 0x00);
	setFlag(cpu, N, mem & 0x80);

}

void DEX(cpu6502 *cpu) {
	cpu->x--;
	setFlag(cpu, Z, cpu->x == 0x00);
	setFlag(cpu, N, cpu->x & 0x80);

}
void DEY(cpu6502 *cpu) {
	cpu->y--;
	setFlag(cpu, Z, cpu->y == 0x00);
	setFlag(cpu, N, cpu->y & 0x80);
}

// Shifts
void ASL(cpu6502 *cpu) {
	if(cpu->instruction->mode == &ACC) {
		setFlag(cpu, C, cpu->a & 0x80);
		cpu->a = cpu->a << 1;
		setFlag(cpu, Z, cpu->a == 0x00);
		setFlag(cpu, N, cpu->a & 0x80);
	}
	else {
		uint8_t mem = read_mem(cpu, cpu->addr);
		setFlag(cpu, C, mem & 0x80);
		mem = mem << 1;
		setFlag(cpu, Z, mem == 0x00);
		setFlag(cpu, N, mem & 0x80);
		write_mem(cpu, cpu->addr, mem);	
	}
}

void LSR(cpu6502 *cpu) {
	if(cpu->instruction->mode == &ACC) {
		setFlag(cpu, C, cpu->a & 0x01);
		cpu->a = cpu-> a >> 1;
		setFlag(cpu, Z, cpu->a == 0x00);
		setFlag(cpu, N, cpu->a & 0x80);
	}
	else {
		uint8_t mem = read_mem(cpu, cpu->addr);
		setFlag(cpu, C, mem & 0x01);
		mem = mem >> 1;
		setFlag(cpu, Z, mem == 0x00);
		setFlag(cpu, N, mem & 0x80);
		write_mem(cpu, cpu->addr, mem);	
	}
}

void ROL(cpu6502 *cpu) {
	if(cpu->instruction->mode == &ACC) {
		uint8_t carry = getFlag(cpu, C);
		setFlag(cpu, C, cpu->a & 0x80);
		cpu->a = cpu-> a << 1;
		cpu->a |= carry;
		setFlag(cpu, Z, cpu->a == 0x00);
		setFlag(cpu, N, cpu->a & 0x80);
	}
	else {
		uint8_t mem = read_mem(cpu, cpu->addr);
		uint8_t carry = getFlag(cpu, C);
		setFlag(cpu, C, mem & 0x80);
		mem = mem << 1;
		mem |= carry;
		setFlag(cpu, Z, mem == 0x00);
		setFlag(cpu, N, mem & 0x80);
		write_mem(cpu, cpu->addr, mem);	
	}
}

void ROR(cpu6502 *cpu) {
	if(cpu->instruction->mode == &ACC) {
		uint8_t carry = getFlag(cpu, C);
		setFlag(cpu, C, cpu->a & 0x01);
		cpu->a = cpu-> a >> 1;
		cpu->a |= (carry << 7);
		setFlag(cpu, Z, cpu->a == 0x00);
		setFlag(cpu, N, cpu->a & 0x80);
	}
	else {
		uint8_t mem = read_mem(cpu, cpu->addr);
		uint8_t carry = getFlag(cpu, C);
		setFlag(cpu, C, mem & 0x01);
		mem = mem >> 1;
		mem |= (carry << 7);
		setFlag(cpu, Z, mem == 0x00);
		setFlag(cpu, N, mem & 0x80);
		write_mem(cpu, cpu->addr, mem);	
	}
}

// Jumps and Calls
void JMP(cpu6502 *cpu) {
	cpu->pc = cpu->addr;
}

void JSR(cpu6502 *cpu) {
	cpu->pc--;
	write_mem(cpu, STACK_START + cpu->sp--, cpu->pc >> 8);
	write_mem(cpu, STACK_START + cpu->sp--, cpu->pc & 0x00FF);
	cpu->pc = cpu->addr;
}

void RTS(cpu6502 *cpu) {
	uint16_t lo = read_mem(cpu, STACK_START + ++cpu->sp);
	uint16_t hi = read_mem(cpu, STACK_START + ++cpu->sp);
	cpu->pc = ((hi << 8) | lo) + 1;
}

// Branches
void BCC(cpu6502 *cpu) {
	if(getFlag(cpu, C) == 0) {
		cpu->cycles++;
		uint16_t temp = cpu->pc + cpu->addr;
		if((temp & 0xFF00) != (cpu->pc & 0xFF00))
			cpu->cycles++;
		cpu->pc = temp;
	}

}

void BCS(cpu6502 *cpu) {
	if(getFlag(cpu, C) == 1) {
		cpu->cycles++;
		uint16_t temp = cpu->pc + cpu->addr;
		if((temp & 0xFF00) != (cpu->pc & 0xFF00))
			cpu->cycles++;
		cpu->pc = temp;
	}

}

void BEQ(cpu6502 *cpu) {
	if(getFlag(cpu, Z) == 1) {
		cpu->cycles++;
		uint16_t temp = cpu->pc + cpu->addr;
		if((temp & 0xFF00) != (cpu->pc & 0xFF00))
			cpu->cycles++;
		cpu->pc = temp;
	}

}

void BMI(cpu6502 *cpu) {
	if(getFlag(cpu, N) == 1) {
		cpu->cycles++;
		uint16_t temp = cpu->pc + cpu->addr;
		if((temp & 0xFF00) != (cpu->pc & 0xFF00))
			cpu->cycles++;
		cpu->pc = temp;
	}

}

void BNE(cpu6502 *cpu) {
	if(getFlag(cpu, Z) == 0) {
		cpu->cycles++;
		uint16_t temp = cpu->pc + cpu->addr;
		if((temp & 0xFF00) != (cpu->pc & 0xFF00))
			cpu->cycles++;
		cpu->pc = temp;
	}
}

void BPL(cpu6502 *cpu) {
	if(getFlag(cpu, N) == 0) {
		cpu->cycles++;
		uint16_t temp = cpu->pc + cpu->addr;
		if((temp & 0xFF00) != (cpu->pc & 0xFF00))
			cpu->cycles++;
		cpu->pc = temp;
	}
}

void BVC(cpu6502 *cpu) {
	if(getFlag(cpu, V) == 0) {
		cpu->cycles++;
		uint16_t temp = cpu->pc + cpu->addr;
		if((temp & 0xFF00) != (cpu->pc & 0xFF00))
			cpu->cycles++;
		cpu->pc = temp;
	}

}

void BVS(cpu6502 *cpu) {
	if(getFlag(cpu, V) == 1) {
		cpu->cycles++;
		uint16_t temp = cpu->pc + cpu->addr;
		if((temp & 0xFF00) != (cpu->pc & 0xFF00))
			cpu->cycles++;
		cpu->pc = temp;
	}
}

// Status Flag Changes
void CLC(cpu6502 *cpu) {
	setFlag(cpu, C, 0);
}

void CLD(cpu6502 *cpu) {
	setFlag(cpu, D, 0);
}

void CLI(cpu6502 *cpu) {
	setFlag(cpu, I, 0);
}

void CLV(cpu6502 *cpu) {
	setFlag(cpu, V, 0);
}

void SEC(cpu6502 *cpu) {
	setFlag(cpu, C, 1);
}

void SED(cpu6502 *cpu) {
	setFlag(cpu, D, 1);
}

void SEI(cpu6502 *cpu) {
	setFlag(cpu, I, 1);
}

// System Functions
void BRK(cpu6502 *cpu) {
	cpu->pc++; // Break instruction is 2 bytes so increment pc by 1 byte
	write_mem(cpu, STACK_START + cpu->sp--, cpu->pc >> 8);
	write_mem(cpu, STACK_START + cpu->sp--, cpu->pc & 0x00FF);
	setFlag(cpu, B, 1);
	write_mem(cpu, STACK_START + cpu->sp--, cpu->sr);
	setFlag(cpu, B, 0);

	uint16_t lo = read_mem(cpu, 0xFFFE);
	uint16_t hi = read_mem(cpu, 0xFFFF);
	cpu->pc = (hi << 8) | lo;
	setFlag(cpu, I, 1);
}
void NOP(cpu6502 *cpu) {
	
}
void RTI(cpu6502 *cpu) {
	cpu->sr = read_mem(cpu, STACK_START + ++cpu->sp);
	setFlag(cpu, U, 1);
	uint16_t lo = read_mem(cpu, STACK_START + ++cpu->sp);
	uint16_t hi = read_mem(cpu, STACK_START + ++ cpu->sp);
	cpu->pc = (hi << 8) | lo;
}

void XXX(cpu6502 *cpu) {

}

void initialize_cpu(cpu6502 *cpu) {
	cpu->cycles = 0;
	cpu->a = cpu->x = cpu->y = 0;
	cpu->sr = 0x24;
	cpu->sp = 0xFD;
	uint16_t lo = cpu->memory[0xFFFC];
	uint16_t hi = cpu->memory[0xFFFD];
	cpu->pc = (hi << 8) | lo;
	cpu->cycles_t = 0;
}

void clock_cycle(cpu6502 *cpu) {
	if(cpu->cycles == 0) {
		uint8_t opcode = read_mem(cpu, cpu->pc++);
		cpu->instruction = &instructionLookup[opcode];
		cpu->instruction->mode(cpu);
		cpu->instruction->instruction(cpu);
		cpu->cycles += cpu->instruction->cycles;

	}
	cpu->cycles--;
	cpu->cycles_t++;
}



