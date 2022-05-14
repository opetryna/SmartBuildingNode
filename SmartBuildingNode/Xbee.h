#ifndef SMARTBUILDINGNODE_XBEE_H
#define SMARTBUILDINGNODE_XBEE_H


#define XBEE_MAX_PAYLOAD_SIZE 255


class Xbee {

private:

    using Frame = struct {
        unsigned int pos = 0;
        unsigned int length = 0;
        unsigned char type = 0;
        unsigned long checksum = 0;
        char payload[XBEE_MAX_PAYLOAD_SIZE + 1];
    };

    static Frame frame;
    static void receive(char c);
    static void (*write) (char);

public:

    static void init(void (*write) (char), void (*setCallback) (void (*callback) (char)));
    static void setCallback(void (*callback) (char*));
    static void send(char* payload, unsigned int length);

};

#endif //SMARTBUILDINGNODE_XBEE_H
