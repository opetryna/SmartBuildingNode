#include <Arduino.h>

#include "Eeprom.h"
#include "Usart.h"
#include "Xbee.h"
#include "CommandInterpreter.h"
#include "DataReporter.h"
#include "Led.h"

#define MAIN_DELAY 1000

using State = struct {
    unsigned long tag1 = 1;
    unsigned int led13Mode = 0;
    unsigned long tag2 = 1;
};

State state;
DataReporter DataReporter(Xbee::send, true);
Led led13(&DDRB, DDB5, &PORTB, PORTB5, &PINB, PINB5);

void loadState() {
    State previousState;
    Eeprom::read(0, sizeof(previousState), &previousState);
    if (previousState.tag1 == previousState.tag2) {
        state = previousState;
        state.tag1 = state.tag2 = 1;
    }
}

void saveState() {
    state.tag1++; state.tag2++;
    Eeprom::write(0, sizeof(state), &state);
}

void executeCommand(char* command, char* value) {

    DataReporter.add(command, value);
    DataReporter.submit();

    if (strcmp(command, "mode") == 0) {
        // TODO

    } else if (strcmp(command, "led") == 0) {
        unsigned int mode = atoi(value);
        led13.setMode(mode);
        state.led13Mode = mode;
        saveState();

    }

}

void setup() {

    loadState();

    CommandInterpreter::setCallback(executeCommand);

    led13.setMode(state.led13Mode);

    Usart::init(9600);
    Xbee::init(Usart::write, Usart::setCallback);
    Xbee::setCallback(CommandInterpreter::interpret);

    asm("SEI");
}

void loop() {

    static unsigned long mainRef = 0;

    if ((millis() - mainRef) >= MAIN_DELAY) {

        // TODO

        mainRef = millis();
    }

    led13.perform();
}
