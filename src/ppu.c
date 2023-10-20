#include "ppu.h"

uint8_t read_vram(ppu2C02 *ppu, uint16_t addr) {

	//TODO MAKE THIS BETTER
	addr &= 0x3FFF; // mirroring
	if (addr < 0x2000) {
		return ppu->chr_rom[addr];
	}
	if (addr < 0x4000) {
		 return read_nametable(ppu, addr);
	}
	return -1;
}
void set_ppuctrl(ppu2C02 *ppu, uint8_t data) {
	// TODO: Implement correct nmi and vbl logic
	ppu->ctrl = data;

	ppu->temp_vramaddr &= ~0x0c00; // clear bits for nametable
	ppu->temp_vramaddr |= (data & 0x03) << 10;

	ppu->flags->vramaddr_increment = ((ppu->ctrl & 0x04) == 0x04) ? 32 : 1;
	ppu->flags->sprite_pattern = ((ppu->ctrl & 0x08) == 0x08) ? 0x1000 : 0;
	ppu->flags->bg_pattern = ((ppu->ctrl & 0x10) == 0x10) ? 0x1000 : 0;
	ppu->flags->sprite_size = (ppu->ctrl & 0x20) == 0x20;
	ppu->flags->gen_NMI = (ppu->ctrl & 0x80) == 0x80;
}

void set_ppumask(ppu2C02 *ppu, uint8_t data) {
	ppu->mask = data;
	ppu->flags->greyscale = (ppu->mask & 0x01) == 0x01; // look nicer
	ppu->flags->show_leftbg = (ppu->mask & 0x02) == 0x02;
	ppu->flags->show_leftsprites = (ppu->mask & 0x04) == 0x04;
	ppu->flags->show_bg = (ppu->mask & 0x08) == 0x08;
	ppu->flags->show_sprites = (ppu->mask & 0x10) == 0x10;
	ppu->flags->emph_red = (ppu->mask & 0x20) == 0x20;
	ppu->flags->emph_green = (ppu->mask & 0x40) == 0x40;
	ppu->flags->emph_blue = (ppu->mask & 0x80) == 0x80;
}

uint8_t get_ppustatus(ppu2C02 *ppu) {
	// first 4 bits are junk
	ppu->status = (ppu->flags->vblank << 7) | 
				  (ppu->flags->sprite0_hit << 6) | 
				  (ppu->flags->sprite_overflow << 5);

	ppu->write_toggle = 0;
	ppu->flags->vblank = 0;
	return ppu->status;
}

void set_oamaddr(ppu2C02 *ppu, uint8_t data) {
	ppu->oamaddr = data;
};

uint8_t get_oamdata(ppu2C02 *ppu) {

	return ppu->OAM[ppu->oamaddr];
}

void set_oamdata(ppu2C02 *ppu, uint8_t data) {
	// TODO: Implement correct increment logic
	ppu->OAM[ppu->oamaddr++] = data;
}

void set_ppuscroll(ppu2C02 *ppu, uint8_t data) {
	// TODO: Implement 2005 glitch
	if (ppu->write_toggle == 0) { // first write
		ppu->temp_vramaddr = (ppu->temp_vramaddr & ~0x1F) | (data >> 3);
		ppu->fine_xscroll = data & 0x7; 
		ppu->write_toggle = 1;
	}
	if (ppu->write_toggle == 1) { // second write
		ppu->temp_vramaddr = (ppu->temp_vramaddr & ~0x73E0) | 
							 ((data & 0x07) << 12) | 
							 ((data & 0xF8) << 2);
		ppu->write_toggle = 0;
	}
}

void set_ppuaddr(ppu2C02 *ppu, uint8_t data) {
	if (ppu->write_toggle == 0) { // first write
		ppu->temp_vramaddr = (ppu->temp_vramaddr & ~0xFF00) | ((data & 0x3F) << 8);
		ppu->write_toggle = 1;
	}
	if (ppu->write_toggle == 1) { // second write
		ppu->temp_vramaddr = (ppu->temp_vramaddr & ~0x00FF) | data;
		ppu->vramaddr = ppu->temp_vramaddr;
		ppu->write_toggle = 0;
	}
}

uint8_t get_ppudata(ppu2C02 *ppu) {
	uint16_t addr = ppu->vramaddr & 0x3FFF; // mirrored cpu memory
	uint8_t return_value = ppu->buffer;
	ppu->buffer = read_vram(ppu, addr);

	if(addr >= 0x3F00)
		return_value = read_palette(ppu, addr);

	increment_vram(ppu);
	return return_value;
}