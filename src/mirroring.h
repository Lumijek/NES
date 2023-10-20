#ifndef MIRRORING_H_
#define MIRRORING_H_

#include <stdint.h>
typedef enum {
	HORIZONTAL = 0,
	VERTICAL = 1,
	SINGLE_SCREEN = 2,
	FOUR_SCREEN = 3
} mirroring_mode;

uint16_t horizontal(uint16_t addr);
uint16_t vertical(uint16_t addr);
uint16_t single_screen(uint16_t addr);
uint16_t four_screen(uint16_t addr);

#endif