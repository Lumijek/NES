#ifndef MAPPER_H_
#define MAPPER_H_
#include <stdint.h>
struct mapper;

struct mapper_vtable {
	uint8_t (*read_cpu)(struct mapper *mapper, uint16_t addr);
	void (*write_cpu)(struct mapper *mapper, uint16_t addr, uint8_t data);
	uint8_t (*read_ppu)(struct mapper *mapper, uint16_t addr);
	void (*write_ppu)(struct mapper *mapper, uint16_t addr, uint8_t data);
};

struct mapper {
	struct mapper_vtable *vtable;
};

uint8_t mapper_read_cpu(struct mapper *mapper, uint16_t addr);
void mapper_write_cpu(struct mapper *mapper, uint16_t addr, uint8_t data);
uint8_t mapper_read_ppu(struct mapper *mapper, uint16_t addr);
void mapper_write_ppu(struct mapper *mapper, uint16_t addr, uint8_t data);
#endif