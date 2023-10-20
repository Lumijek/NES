#include "mapper0-nrom.h"

struct mapper_vtable nrom_vtable = {
	(uint8_t (*)(struct mapper *, uint16_t))&nrom_read_cpu,
	(void (*)(struct mapper *, uint16_t, uint8_t))&nrom_write_cpu,
	(uint8_t (*)(struct mapper *, uint16_t))&nrom_read_ppu,
	(void (*)(struct mapper *, uint16_t, uint8_t))&nrom_write_ppu
};


void set_cpu_mapping(nrom *nrom, uint8_t bank, uint8_t position, uint16_t range) {
	if(bank == 1) {
		nrom->prg_bank1 = &nrom->prg_rom[position * range];
	}	
	else if(bank == 2) {
		nrom->prg_bank2 = &nrom->prg_rom[position * range];	
	}
}

void set_ram_mapping(nrom *nrom, uint8_t position, uint16_t range) {
	nrom->prg_ram_bank = &nrom->prg_ram[position * range];
}

void set_ppu_mapping(nrom *nrom, uint8_t position, uint16_t range) {
	nrom->chr_bank = &nrom->chr_rom[position * range];
}



struct mapper *init_nrom(nrom *nrom, cartridge *cart, FILE *rom) {
	nrom->super.vtable = &nrom_vtable;

	nrom->mirroring = cart->mirroring;

	nrom->prg_rom = malloc(cart->prg_rom_size);
	nrom->prg_ram = malloc(cart->prg_ram_size);
	nrom->chr_rom = malloc(cart->chr_rom_size);

	if(cart->trainer_present) {
		fread(&nrom->prg_ram[0x1000], sizeof(uint8_t), 512, rom); // 0x7000 in cpu memory
	}

	fread(nrom->prg_rom, sizeof(uint8_t), cart->prg_rom_size, rom);
	fread(nrom->chr_rom, sizeof(uint8_t), cart->chr_rom_size, rom);

	// set prg_rom mappings
	set_cpu_mapping(nrom, 1, 0, 0x4000);
	if(cart->prg_rom_size == 0x4000) {
		set_cpu_mapping(nrom, 2, 0, 0x4000);
	}
	else {
		set_cpu_mapping(nrom, 2, 1, 0x4000);
	}
	set_ram_mapping(nrom, 0, 0x2000);
	set_ppu_mapping(nrom, 0, 0x2000);

	return (struct mapper *)nrom;
}


uint8_t nrom_read_cpu(nrom *nrom, uint16_t addr) {
	if(addr < 0x8000) {
		addr -= 0x6000;
		return nrom->prg_ram[addr];
	}
	else if (addr < 0xC000) {
		addr -= 0x8000;
		return nrom->prg_bank1[addr];
	}
	else if (addr <= 0xFFFF) {
		addr -= 0xC000;
		return nrom->prg_bank2[addr];
	}
	return -1;
}

void nrom_write_cpu(nrom *nrom, uint16_t addr, uint8_t data) {
	if(addr < 0x6000) {
		;
	}
	else if(addr < 0x8000) {
		addr -= 0x6000;
		nrom->prg_ram[addr] = data;
	}
}

uint8_t nrom_read_ppu(nrom *nrom, uint16_t addr) {
	if(addr < 0x2000) {
		return nrom->chr_bank[addr];
	}
	else if(addr < 0x4000) {
		if(nrom->mirroring == HORIZONTAL) {
			addr = horizontal(addr);
			return nrom->vram[addr];
		}
		else if(nrom->mirroring == VERTICAL) {
			addr = vertical(addr);
			return nrom->vram[addr];
		}
		else if(nrom->mirroring == FOUR_SCREEN) {
			addr = four_screen(addr);
			return nrom->vram[addr];
		}
	}
	return 0xFF;
}

void nrom_write_ppu(nrom *nrom, uint16_t addr, uint8_t data) {
	if(addr < 0x2000) {
		;
	}
	else if(addr < 0x4000) {
		if(nrom->mirroring == HORIZONTAL) {
			addr = horizontal(addr);
			nrom->vram[addr] = data;
		}
		else if(nrom->mirroring == VERTICAL) {
			addr = vertical(addr);
			nrom->vram[addr] = data;
		}
		else if(nrom->mirroring == FOUR_SCREEN) {
			addr = four_screen(addr);
			nrom->vram[addr] = data;
		}
	}
}
