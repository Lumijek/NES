#ifndef PPU_H
#define PPU_H
#include <stdint.h>
#include "mapper.h"

typedef struct {
	uint8_t vramaddr_increment;
	uint16_t sprite_pattern;
	uint16_t bg_pattern;
	uint8_t sprite_size;
	uint8_t gen_NMI;
	uint8_t greyscale;
	uint8_t show_leftbg;
	uint8_t show_leftsprites;
	uint8_t show_bg;
	uint8_t show_sprites;
	uint8_t emph_red;
	uint8_t emph_green;
	uint8_t emph_blue;
	uint8_t vblank;
	uint8_t sprite0_hit;
	uint8_t sprite_overflow;

} ppu_flags;

typedef enum {
	PRE_RENDERING,
	RENDERING,
	POST_RENDERING,
} render_states;

typedef struct {

	uint8_t rendering;
	// ppu registers
	uint8_t ctrl;
	uint8_t mask;
	uint8_t status;
	uint8_t oamaddr;
	uint8_t oamdata;
	uint8_t scroll;
	uint8_t addr; // not used
 	uint8_t data;

	// ppu internal backround registers
	uint16_t vramaddr;
	uint16_t temp_vramaddr;
	uint8_t fine_xscroll;
	uint8_t write_toggle;
	uint8_t buffer;

	// internal stuff
	uint16_t nt_tile_address;
	uint8_t tile_index;
	uint16_t attribute_address;
	uint8_t attribute;
	uint16_t pattern_low_address;
	uint16_t pattern_high_address;

	// shifters

	uint16_t low_tile_shift;
	uint16_t high_tile_shift;
	uint8_t low_palette_shift;
	uint8_t high_palette_shift;
	uint8_t low_palette_latch;
	uint8_t high_plaette_latch;
	uint8_t tile_cycle;

	//
	uint8_t pattern_low;
	uint8_t pattern_high;

	// ppu flags
	ppu_flags *flags;

	// 
	render_states render_state;


	struct mapper *mapper;
	uint8_t palettes[0x20];
	uint8_t OAM[0x100];
	uint8_t secondary_OAM[0x20];

	uint8_t nametable_mirroring;

}ppu2C02;

uint8_t get_fine_y(ppu2C02 *ppu);
uint8_t get_coarse_y(ppu2C02 *ppu);
uint8_t get_fine_x(ppu2C02 *ppu);
uint8_t get_coarse_x(ppu2C02 *ppu);

void coarse_x_increment(ppu2C02 *ppu);
void fine_y_increment(ppu2C02 *ppu);

uint8_t read_palette(ppu2C02 *ppu, uint16_t addr);
void increment_vram(ppu2C02 *ppu);
uint8_t read_ppu(ppu2C02 *ppu, uint16_t addr);

void set_ppuctrl(ppu2C02 *ppu, uint8_t data);
void set_ppumask(ppu2C02 *ppu, uint8_t data);
uint8_t get_ppustatus(ppu2C02 *ppu);
void set_oamaddr(ppu2C02 *ppu, uint8_t data);
uint8_t get_oamdata(ppu2C02 *ppu);
void set_oamdata(ppu2C02 *ppu, uint8_t data);
void set_ppuscroll(ppu2C02 *ppu, uint8_t data);
void set_ppuaddr(ppu2C02 *ppu, uint8_t data);
uint8_t get_ppudata(ppu2C02 *ppu);
void set_ppudata(ppu2C02 *ppu, uint8_t data); 

void get_bg_tiles(ppu2C02 *ppu);
void initialize_ppu(ppu2C02 *ppu);



#endif