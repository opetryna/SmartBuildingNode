#include <string.h>

#include "CommandInterpreter.h"

void (*CommandInterpreter::execute) (char*, char*) = nullptr;

void CommandInterpreter::interpret(char* msg) {

    char* line_ptr;
    char* line = strtok_r(msg, "\n", &line_ptr);
    while (line != nullptr) {

        char entry[MAX_CMD_SIZE];
        strncpy(entry, line, MAX_CMD_SIZE);
        char* entry_ptr;
        char* command = strtok_r(entry, "=", &entry_ptr);
        char* value = strtok_r(nullptr, "=", &entry_ptr);

        if (command != nullptr && value != nullptr)
            execute(command, value);

        line = strtok_r(nullptr, "\n", &line_ptr);
    }
}

void CommandInterpreter::setCallback(void (*callback)(char *, char *)) {
    execute = callback;
}