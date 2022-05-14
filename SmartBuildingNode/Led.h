#ifndef SMARTBUILDINGNODE_LED_H
#define SMARTBUILDINGNODE_LED_H


#include <stdint.h>

class Led {

private:

    volatile uint8_t* DDRx;
    uint8_t DDxn;
    volatile uint8_t* PORTx;
    uint8_t PORTxn;
    volatile uint8_t* PINx;
    uint8_t PINxn;

    unsigned int mode;
    unsigned long blinkReference;

public:

    Led(volatile uint8_t* DDRx,
        uint8_t DDxn,
        volatile uint8_t* PORTx,
        uint8_t PORTxn,
        volatile uint8_t* PINx,
        uint8_t PINxn);

    void setMode(unsigned int mode);
    unsigned int getMode();
    void perform();

};

#endif //SMARTBUILDINGNODE_LED_H
