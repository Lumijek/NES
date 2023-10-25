#include "ppu.h"

uint8_t get_fine_y(ppu2C02 *ppu) {
	return (ppu->vramaddr >> 12) & 0x7;
}

uint8_t get_coarse_y(ppu2C02 *ppu) {
	return (ppu->vramaddr >> 5) & 0x1F;
}

uint8_t get_fine_x(ppu2C02 *ppu) {
	return ppu->fine_xscroll;
}

uint8_t get_coarse_x(ppu2C02 *ppu) {
	return ppu->vramaddr & 0x1F;
}

void coarse_x_increment(ppu2C02 *ppu) {
	if((ppu->vramaddr & 0x001F) == 31) {
		ppu->vramaddr &= ~0x001F;
		ppu->vramaddr ^= 0x0400;
	}
	else {
		ppu->vramaddr += 1;
	}
}

void fine_y_increment(ppu2C02 *ppu) {
	if((ppu->vramaddr & 0x7000) != 0x7000) {
		ppu->vramaddr += 0x1000;
	}
	else {
		ppu->vramaddr &= ~0x7000;
		uint16_t y = (ppu->vramaddr & 0x03E0) >> 5;
		if(y == 29) {
			y = 0;
			ppu->vramaddr ^= 0x0800;
		}
		else if(y == 31) {
			y = 0;
		}
		else {
			y += 1;
		}
		ppu->vramaddr = (ppu->vramaddr & ~0x03E0) | (y << 5);
	}
}

uint8_t read_palette(ppu2C02 *ppu, uint16_t addr) {
	addr = addr - 0x3F00;
	if(addr >= 0x10 && (addr & 4) == 1) {
		addr = addr - 0x10;
	}
	return ppu->palettes[addr];
}

void increment_vram(ppu2C02 *ppu) {
	if((ppu->render_state == PRE_RENDERING || ppu->render_state == RENDERING) && ppu->rendering) {
		coarse_x_increment(ppu);
		fine_y_increment(ppu);
	}
	else {
		ppu->vramaddr += ppu->flags->vramaddr_increment;	
	}
}
uint8_t read_ppu(ppu2C02 *ppu, uint16_t addr) {

	addr &= 0x3FFF; // mirroring
	return mapper_read_ppu(ppu->mapper, addr);
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
	ppu->rendering = ppu->flags->show_bg || ppu->flags->show_sprites;

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
	ppu->buffer = read_ppu(ppu, addr);

	if(addr >= 0x3F00)
		return_value = read_palette(ppu, addr);

	increment_vram(ppu);
	return return_value;
}

void get_bg_tiles(ppu2C02 *ppu) {
	switch(ppu->tile_cycle) {
		case 0:
			ppu->nt_tile_address = (ppu->vramaddr & 0x00FF); // latch first 8 bits

		case 1:
			ppu->nt_tile_address |= (0x2000 | (ppu->vramaddr & 0x0F00)); // upper 6 bits not latched during cycle 0
			ppu->tile_index = mapper_read_ppu(ppu->mapper, ppu->nt_tile_address);

		case 2:
			ppu->attribute_address = 0x00C0 |
			 						 ((ppu->vramaddr >> 4) & 0x0038) | 
			 						 ((ppu->vramaddr >> 2) & 0x0007); // latch first 8 bits

		case 3:
			ppu->attribute_address |= (0x2300 | (ppu->vramaddr & 0x0C00)); // upper 6 bits not latched during cycle 2
			ppu->attribute = mapper_read_ppu(ppu->mapper, ppu->attribute_address);

		case 4:
			ppu->pattern_low_address = ((ppu->tile_index & 0x0F) << 4) | get_fine_y(ppu); // latch first 8 bits

		case 5:
			ppu->pattern_low_address |= ppu->flags->bg_pattern | ((ppu->tile_index & 0xF0) << 4); // upper 6 bits not latched during cycle 4
			ppu->pattern_low = mapper_read_ppu(ppu->mapper, ppu->pattern_low_address);

		case 6:
			ppu->pattern_high_address = ((ppu->tile_index & 0x0F) << 4) | get_fine_y(ppu) | 0x0008; // latch first 8 bits

		case 7:
			ppu->pattern_high_address |= ppu->flags->bg_pattern | ((ppu->tile_index & 0xF0) << 4); // upper 6 bits not latched during cycle 6
			ppu->pattern_high = mapper_read_ppu(ppu->mapper, ppu->pattern_high_address);		
	}
}
void initialize_ppu(ppu2C02 *ppu) {
	;
}