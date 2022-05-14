#ifndef SMARTBUILDINGNODE_USART_H
#define SMARTBUILDINGNODE_USART_H

class Usart {

public:

    static void init(unsigned long baud);
    static void setCallback(void (*callback) (char));
    static void write(char c);
    static void write(char* s);

};

#endif //SMARTBUILDINGNODE_USART_H
