#ifndef NES_H_
#define NES_H_
#include "cpu.h"
#include "ppu.h"
#include "mapper.h"
#include "mapper0-nrom.h"
#include "nes_header.h"
#include "mapper_finder.h"

typedef struct {

	cpu6502 *cpu;
	ppu2C02 *ppu;
	struct mapper *mapper;

} NES;

void setup_nes(NES *nes, cartridge *cart, FILE *rom);
#endif