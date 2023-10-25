#include "NES.h"

void setup_nes(NES *nes, cartridge *cart, FILE *rom) {
	cpu6502 *cpu = malloc(sizeof(cpu));
	ppu2C02 *ppu = malloc(sizeof(ppu));
	struct mapper *mapper = get_mapper(cart->mapper_number, cart, rom);
	if(!mapper) {
		printf("Not valid mapper.\n");
		exit(-1);
	}

	nes->cpu = cpu;
	nes->ppu = ppu;
	nes->mapper = mapper;
	cpu->mapper = mapper;
	ppu->mapper = mapper;

	initialize_cpu(cpu);
	initialize_ppu(ppu);
}