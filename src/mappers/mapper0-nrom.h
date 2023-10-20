#ifndef MAPPER0_NROM_H_
#define MAPPER0_NROM_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "mapper.h"
#include "mirroring.h"
#include "nes_header.h"

typedef struct {
	struct mapper super;

	uint8_t *prg_rom;
	uint8_t *prg_ram;
	uint8_t *chr_rom;

	uint8_t *prg_bank1;
	uint8_t *prg_bank2;
	uint8_t *prg_ram_bank;
	uint8_t *chr_bank;

	uint8_t mirroring;
	uint8_t vram[0x1000];
} nrom;




void set_cpu_mapping(nrom *nrom, uint8_t bank, uint8_t position, uint16_t range);
void set_ram_mapping(nrom *nrom, uint8_t position, uint16_t range);
void set_ppu_mapping(nrom *nrom, uint8_t position, uint16_t range);

struct mapper *init_nrom(nrom *nrom, cartridge *cart, FILE *rom);
uint8_t nrom_read_cpu(nrom *nrom, uint16_t addr);
void nrom_write_cpu(nrom *nrom, uint16_t addr, uint8_t data);
uint8_t nrom_read_ppu(nrom *nrom, uint16_t addr);
void nrom_write_ppu(nrom *nrom, uint16_t addr, uint8_t data);
#endif