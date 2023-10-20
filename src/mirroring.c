#include "mirroring.h"

uint16_t horizontal(uint16_t addr) {
	if(addr >= 0x3000) {
		addr = addr - 0x1000;
	}

	if(addr < 0x2800) {
		return (addr & 0x3FFF);
	}
	else if(addr < 0x3000) {
		return (addr & 0x3FFF) + 0x400;
	}
	return 0xFFFF;
}

uint16_t vertical(uint16_t addr) {
	if(addr >= 0x3000) {
		addr = addr - 0x1000;
	}

	if(addr < 0x2400) {
		return (addr & 0x3FFF);
	}
	else if(addr < 0x2800) {
		return (addr & 0x3FFF) + 0x400;
	}
	if(addr < 0x2C00) {
		return (addr & 0x3FFF);
	}
	else if(addr < 0x3000) {
		return (addr & 0x3FFF) + 0x400;
	}
	return 0xFFFF;

}

uint16_t single_screen(uint16_t addr) {
	if(addr >= 0x3000) {
		addr = addr - 0x1000;
	}
	if(addr < 0x3000) {
		return (addr & 0x3FFF);
	}
	return 0xFFFF;
}

uint16_t four_screen(uint16_t addr) {
	if(addr >= 0x3000) {
		addr = addr - 0x1000;
	}
	
	if(addr < 0x3000) {
		return (addr & 0x0FFF);
	}
	return 0xFFFF;
}