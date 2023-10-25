#include "mapper_finder.h"

struct mapper *get_mapper(uint16_t mapper_number, cartridge *cart, FILE *rom) {
	switch(mapper_number) {
		case 0: {
			nrom n;
			return init_nrom(&n, cart, rom);
		}
	}
	return NULL;
}
