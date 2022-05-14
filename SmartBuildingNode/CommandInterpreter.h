#ifndef SMARTBUILDINGNODE_COMMANDINTERPRETER_H
#define SMARTBUILDINGNODE_COMMANDINTERPRETER_H

#define MAX_CMD_SIZE 32

class CommandInterpreter {

private:

    static void (*execute) (char* command, char* value);

public:

    static void interpret(char* msg);
    static void setCallback(void (*callback) (char*, char*));

};

#endif //SMARTBUILDINGNODE_COMMANDINTERPRETER_H
