#ifndef SMARTBUILDINGNODE_EEPROM_H
#define SMARTBUILDINGNODE_EEPROM_H


class Eeprom {

private:

    static unsigned char readByte(unsigned int addr);
    static void writeByte(unsigned int addr, unsigned char data);

public:

    static void read(unsigned int addr, unsigned int len, void* data);
    static void write(unsigned int addr, unsigned int len, void* data);

};

#endif //SMARTBUILDINGNODE_EEPROM_H
