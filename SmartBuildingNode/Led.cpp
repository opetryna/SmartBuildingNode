#include "Arduino.h"

#include "Led.h"

Led::Led(volatile uint8_t* DDRx,
         uint8_t DDxn,
         volatile uint8_t* PORTx,
         uint8_t PORTxn,
         volatile uint8_t* PINx,
         uint8_t PINxn) {

    this->DDRx = DDRx;
    this->DDxn = DDxn;
    this->PORTx = PORTx;
    this->PORTxn = PORTxn;
    this->PINx = PINx;
    this->PINxn = PINxn;

    this->mode = 0;
    this->blinkReference = 0;

    *this->DDRx |= 1 << this->DDxn;
}

void Led::setMode(unsigned int mode) {
    this->mode = mode;
}

unsigned int Led::getMode() {
    return mode;
}

void Led::perform() {

    if (mode == 0) {
        *PORTx &= ~(1 << PORTxn);

    } else if (mode == 1) {
        *PORTx |= 1 << PORTxn;
    } else if ((millis() - blinkReference) > mode) {
        *PINx = 1 << PINxn;
        blinkReference = millis();
    }

}
