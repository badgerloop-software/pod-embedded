#ifndef __BLOOP_PACKET_H__
#define __BLOOP_PACKET_H__

#include <stdlib.h>

#define FORMAT_HEADER(x) (x##|)

/* Badgerloop Packet! */
class BPacket {
    private:
        static const int MIN_DATA_PACKET_SIZE = 32;
        const int OFFSET = 2;
        char *data;
        char *payload;
        char header;
        
        enum Type {
            DATA    = 'd',
            CONTROL = 'c'
        };
        
        void init(BPacket::Type type, int size);
        void attachHeader();

    public:
        
        BPacket::Type type;
        
        enum Command {
            BRAKE   = 'b',
            UNBRAKE = 'u',
            ACK = 'a',
            ACKACK = 'k',
            INVALID = 'i'
        };
        
        BPacket(BPacket::Command cmd) {
            this->init(CONTROL, 3);
            this->payload[2] = (char) cmd;
        }

        ~BPacket() {
            free(this->payload);
        }
        
        char *getPayload() {
            return payload;
        }

        BPacket::Type getType() {
            return type;
        }

};

#endif
