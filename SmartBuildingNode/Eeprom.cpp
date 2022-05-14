#include <avr/io.h>

#include "Eeprom.h"


unsigned char Eeprom::readByte(unsigned int addr) {
    while (0 != (EECR & (1 << EEPE)));
    EEAR = addr;
    EECR |= (1 << EERE);
    return EEDR;
}

void Eeprom::writeByte(unsigned int addr, unsigned char data) {
    while (EECR & (1 << EEPE));
    EEAR = addr;
    EEDR = data;
    EECR |= (1 << EEMPE);
    EECR |= (1 << EEPE);
}

void Eeprom::read(unsigned int addr, unsigned int len, void* data_ptr) {
    unsigned char* data = (unsigned char*) data_ptr;
    for (unsigned int i = 0; i < len; ++i) {
        data[i] = readByte(addr + i);
    }
}

void Eeprom::write(unsigned int addr, unsigned int len, void* data_ptr) {
    unsigned char* data = (unsigned char*) data_ptr;
    for (unsigned int i = 0; i < len; ++i) {
        writeByte(addr + i, data[i]);
    }
}