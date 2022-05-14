#include "Xbee.h"


Xbee::Frame Xbee::frame;
void (*Xbee::write) (char) = nullptr;
void (*Xbee_callback) (char*) = nullptr;

void Xbee::init(void (*write) (char), void (*setCallback_a) (void (*callback_a) (char))) {
    Xbee::write = write;
    setCallback_a(receive);
}

void Xbee::setCallback(void (*callback_a) (char*)) {
    Xbee_callback = callback_a;
}

void Xbee::send(char* payload, unsigned int length) {

    // checksum counter
    long checksum = 0;

    // frame start
    write(0x7E);

    // data length MSB
    write(((14 + length) >> 8) & 0xFF);
    // data length LSB
    write((14 + length) & 0xFF);

    // frame type Transmit Request
    unsigned char frame_type = 0x10;
    write(frame_type);
    checksum += frame_type;
    // frame ID
    unsigned char frame_id = 0x00;
    write(frame_id);
    checksum += frame_id;

    // 64-bit destination address Coordinator
    for (int i = 5; i <= 12; ++i) {
        unsigned char b = 0x00;
        write(b);
        checksum += b;
    }

    // 16-bit destination address Coordinator
    for (int i = 13; i <= 14; ++i) {
        unsigned char b = 0x00;
        write(b);
        checksum += b;
    }

    // broadcast radius
    unsigned char broadcast_radius = 0x00;
    write(broadcast_radius);
    checksum += broadcast_radius;

    // options
    unsigned char options = 0x00;
    write(options);
    checksum += options;

    // payload
    for (int i = 0; i < length; ++i) {
        write(payload[i]);
        checksum += payload[i];
    }

    // checksum
    write(0xFF - (checksum & 0xFF));
}

void Xbee::receive(char cc) {

    unsigned char c = (unsigned char) cc;

    if (c == 0x7E && frame.pos == 0) {
        frame.pos = 1;
        frame.length = 0;
        frame.type = 0;
        frame.checksum = 0;

    } else {
        if (frame.pos == 1) {
            frame.length = c << 8;

        } else if (frame.pos == 2) {
            frame.length += c;

        } else if (frame.pos == 3) {
            frame.type = c;
            frame.checksum += c;

        } else if (frame.type == 0x90) {

             if ((frame.pos >= 15) && (frame.pos < frame.length + 3)) {
                 if (frame.pos - 15 < XBEE_MAX_PAYLOAD_SIZE) {
                     frame.payload[frame.pos - 15] = c;
                     frame.checksum += c;
                 }

             } else if (frame.pos == frame.length + 3) {
                 if (frame.pos - 15 <= XBEE_MAX_PAYLOAD_SIZE) {
                     frame.payload[frame.pos - 15] = '\0';
                     if (c == (0xFF - (frame.checksum & 0xFF))) {
                         if (Xbee_callback != nullptr) {
                             Xbee_callback(frame.payload);
                         }
                     }
                 }

             } else {
                 frame.checksum += c;
             }
        }

        if (frame.pos != 0 && ++frame.pos >= frame.length + 4) {
            frame.pos = 0;
        }
    }
}