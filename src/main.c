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
	char *filename = "../donkey kong.nes";
	FILE *rom = fopen(filename, "rb");
	if(rom == NULL) {
		printf("Could not open file.\n");
		exit(-1);
	}
	load_nes_header(cart, rom);
	//setup_nes(nes, cart, rom);
	//printf("%02x", mapper_read_cpu(nes->mapper, 0xFFFD));
	//free(((nrom *)(nes->mapper))->prg_rom);
	//free(((nrom *)(nes->mapper))->prg_ram);
	//free(((nrom *)(nes->mapper))->chr_rom);
	//free(nes);
	//free(cart);

}