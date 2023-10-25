#ifndef MAPPER_FINDER_H_
#define MAPPER_FINDER_H_
#include <stdint.h>
#include <stdio.h>
#include "mapper0-nrom.h"
#include "nes_header.h"
#include "mapper.h"

struct mapper *get_mapper(uint16_t mapper_number, cartridge *cart, FILE *rom);

#endif