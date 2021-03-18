#include "packet.h"
#include <stdlib.h>

void BPacket::init(BPacket::Type type, int size) {
    this->type = type;
    this->size = size;
    this->payload = (char *) malloc(this->size * sizeof(char));
    BPacket::attachHeader();
}

void BPacket::attachHeader() {
    this->payload[0] = (char) this->type;
    this->payload[1] = '|';
}

