#include "mapper.h"

uint8_t mapper_read_cpu(struct mapper *mapper, uint16_t addr) {
	return mapper->vtable->read_cpu(mapper, addr);
}
void mapper_write_cpu(struct mapper *mapper, uint16_t addr, uint8_t data) {
	mapper->vtable->write_cpu(mapper, addr, data);
}
uint8_t mapper_read_ppu(struct mapper *mapper, uint16_t addr) {
	return mapper->vtable->read_ppu(mapper, addr);
}
void mapper_write_ppu(struct mapper *mapper, uint16_t addr, uint8_t data) {
	mapper->vtable->write_ppu(mapper, addr, data);
}