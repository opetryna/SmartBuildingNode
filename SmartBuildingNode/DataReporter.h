#ifndef SMARTBUILDINGNODE_DATAREPORTER_H
#define SMARTBUILDINGNODE_DATAREPORTER_H

#define MAX_MSG_SIZE 255

class DataReporter {

private:

    struct CharsCount {
        unsigned int total = 0;
        unsigned int special = 0;
    };

    char msg[MAX_MSG_SIZE];
    unsigned int p;

    bool autoSubmit;
    void (*send) (char*, unsigned int);

    void init();
    static void countChars(char* s, struct CharsCount* c);
    static void formatString(char* dst, char* src, bool escape, bool quoted);
    bool add(char* key, char* value, bool isString);

public:

    DataReporter(void (*send) (char*, unsigned int), bool autoSubmit);

    bool add(char* key, char* value);
    bool add(char* key, int value);
    bool add(char* key, float value);
    void submit();

};

#endif //SMARTBUILDINGNODE_DATAREPORTER_H
