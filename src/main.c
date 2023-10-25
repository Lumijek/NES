#include <SDL2/SDL.h>
#include <unistd.h>

#include "cpu.h"
#include "mapper.h"
#include "mirroring.h"
#include "nes_header.h"
#include "ppu.h"
#include "mapper_finder.h"
#include "NES.h"


int main (void){
	setbuf(stdout, NULL);
    //printf("%ld\n", (long)getpid());

	NES *nes = malloc(sizeof(NES));
	cartridge *cart = malloc(sizeof(cartridge));
	char *filename = "../tr/08-ind_x.nes";
	FILE *rom = fopen(filename, "rb");
	if(rom == NULL) {
		printf("Could not open file.\n");
		exit(-1);
	}
	load_nes_header(cart, rom);
	setup_nes(nes, cart, rom);
	uint8_t ad = 20;
	write_cpu(nes->cpu, 0x6000, 0x20);
	for(int i = 0; i < 10000000; i++) {
		clock_cycle(nes->cpu);
		ad = read_cpu(nes->cpu, 0x6000);
		printf("%d: %02x\n", i, ad);
		if(ad == 0 || ad == 1) {
			break;
		}
		usleep(1);
	}
	for(int i = 0; i < 500; i++) {
		printf("%c", read_cpu(nes->cpu, 0x6004 + i));
	}


}