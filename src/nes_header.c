#include "nes_header.h"
#include <math.h>

const uint8_t HEADER_SIZE = 16;

void get_ines_type(cartridge *cart) {
	cart->iNES = true;
	cart->iNES20 = false;
	if((cart->header[7] & 0x0C) == 0x08) {
		cart->iNES = false;
		cart->iNES20 = true;
	}
}

void get_mirroring(cartridge *cart) {
	cart->mirroring = HORIZONTAL;
	if(cart->header[6] & 0x01) {
		cart->mirroring = VERTICAL;
	}
	if((cart->header[6] & 0x08) >> 3) {
		cart->mirroring = FOUR_SCREEN;
	}
}

void is_battery_present(cartridge *cart) {
	cart->battery_present = false;
	if((cart->header[6] >> 1) & 0x01) {
		cart->battery_present = true;
	}
}
void is_trainer_present(cartridge *cart) {
	cart->trainer_present = false;
	if((cart->header[6] >> 2) & 0x01) {
		cart->trainer_present = true;
	}
}

void get_mapper_number(cartridge *cart) {
	uint8_t mapper_number_base = ((cart->header[6] & 0xF0) >> 4) |
								 (cart->header[7] & 0xF0);
	if(cart->iNES) {
		cart->mapper_number = mapper_number_base;
	}
	else if(cart->iNES20) {
		cart->mapper_number = mapper_number_base | ((cart->header[8] & 0x0F) << 8);
	}
}
void get_submapper_number(cartridge *cart) {
	cart->submapper_number = 0;
	if(cart->iNES20) {
		cart->submapper_number = (cart->header[8] & 0xF0) >> 4;
	}
}

void get_prg_rom_size(cartridge *cart) {
	if(cart->iNES) {
		cart->prg_rom_size = cart->header[4] * 0x4000;
	}
	else if (cart->iNES20) {
		uint8_t LSB = cart->header[4];
		uint8_t MSB = cart->header[9] & 0x0F;
		if(MSB == 0xF) {
			uint8_t multiplier = (LSB & 0x3) * 2 + 1;
			uint8_t exponent = pow(2, LSB >> 2);
			cart->prg_rom_size = multiplier * exponent;
		}
		else {
			cart->prg_rom_size = ((MSB << 8) | LSB) * 0x4000;
		}
	}
}
void get_chr_rom_size(cartridge *cart) {
	if(cart->iNES) {
		cart->chr_rom_size = cart->header[5] * 0x2000;
	}
	else if (cart->iNES20) {
		uint8_t LSB = cart->header[5];
		uint8_t MSB = (cart->header[9] & 0xF0) >> 4;
		if(MSB == 0xF) {
			uint8_t multiplier = (LSB & 0x3) * 2 + 1;
			uint8_t exponent = pow(2, LSB >> 2);
			cart->chr_rom_size = multiplier * exponent;
		}
		else {
			cart->chr_rom_size = ((MSB << 8) | LSB) * 0x2000;
		}
	}
}

void get_prg_ram_size(cartridge *cart) {
	if(cart->iNES) {
		cart->prg_ram_size = cart->header[8] * 0x2000;
		if(cart->prg_ram_size == 0) {
			cart->prg_ram_size = 0x2000;
		}
	}
	else if(cart->iNES20) {
		uint8_t shift = cart->header[10] & 0x0F;
		if(shift == 0) {
			cart->prg_ram_size = 0;
		}
		else {
			cart->prg_ram_size = 64 << shift;
		}
	}
}

void get_chr_ram_size(cartridge *cart) {
	if(cart->iNES) {
		if(cart->chr_rom_size == 0) {
			cart->chr_ram_size = 0x2000;
		}
		else {
			cart->chr_ram_size = 0;
		}
	}
	else if(cart->iNES20) {
		uint8_t shift = cart->header[11] & 0x0F;
		if(shift == 0) {
			cart->chr_ram_size = 0;
		}
		else {
			cart->chr_ram_size = 64 << shift;
		}
	}
}

void get_prg_nvram_size(cartridge *cart) {
	if(cart->iNES) {
		cart->prg_nvram_size = 0;
		if(cart->battery_present) {
			cart->prg_nvram_size = 0x2000;
		}
	}
	else if(cart->iNES20) {
		uint8_t shift = (cart->header[10] & 0xF0) >> 4;
		if(shift == 0) {
			cart->prg_nvram_size = 0;
		}
		else {
			cart->prg_nvram_size = 64 << shift;
		}
	}
}
void get_chr_nvram_size(cartridge *cart) {
	if(cart->iNES) {
		cart->chr_nvram_size = 0;
	}
	else if(cart->iNES20) {
		uint8_t shift = (cart->header[11] & 0xF0) >> 4;
		if(shift == 0) {
			cart->chr_nvram_size = 0;
		}
		else {
			cart->chr_nvram_size = 64 << shift;
		}
	}
}

void load_nes_header(cartridge *cart, FILE *rom) {
	fread(cart->header, sizeof(uint8_t), HEADER_SIZE, rom);
	if((strncmp((char *)cart->header, "NES", 3) != 0) || (cart->header[3] != 0x1A)) {
		printf("Invalid NES ROM\n");
		exit(-1);
	}

	get_ines_type(cart);
	get_mirroring(cart);
	is_battery_present(cart);
	is_trainer_present(cart);
	get_mapper_number(cart);
	get_submapper_number(cart);
	
	get_prg_rom_size(cart);
	get_chr_rom_size(cart);
	get_prg_ram_size(cart);
	get_chr_ram_size(cart);
	get_prg_nvram_size(cart);
	get_chr_nvram_size(cart);
}