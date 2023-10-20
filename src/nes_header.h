#ifndef NES_HEADER_H_
#define NES_HEADER_H_
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mirroring.h"

typedef struct {
	uint8_t header[16];

	bool iNES;
	bool iNES20;

	uint32_t prg_rom_size;
	uint32_t chr_rom_size;

	uint32_t prg_ram_size;
	uint32_t chr_ram_size;

	uint32_t prg_nvram_size;
	uint32_t chr_nvram_size;

	bool trainer_present;
	bool battery_present;

	uint16_t mapper_number;
	uint8_t submapper_number;

	mirroring_mode mirroring;

} cartridge;

void get_ines_type(cartridge *cart);

void get_mirroring(cartridge *cart);

void is_battery_present(cartridge *cart);
void is_trainer_present(cartridge *cart);

void get_prg_rom_size(cartridge *cart);
void get_chr_rom_size(cartridge *cart);

void get_prg_ram_size(cartridge *cart);
void get_chr_ram_size(cartridge *cart);

void get_prg_nvram_size(cartridge *cart);
void get_chr_nvram_size(cartridge *cart);

void get_mapper_number(cartridge *cart);
void get_submapper_number(cartridge *cart);

void load_nes_header(cartridge *cart, FILE *rom);

#endif