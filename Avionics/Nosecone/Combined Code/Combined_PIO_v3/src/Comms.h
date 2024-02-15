#ifndef COMMS_H
#define COMMS_H

#include <Arduino.h>
// #include "HAL.h"
namespace Comms {

    #define MAX_PACKET_LEN 256

    
    struct Packet
    {
        uint8_t id;
        uint8_t len;
        uint8_t timestamp[4];
        uint8_t checksum[2];
        uint8_t data[MAX_PACKET_LEN];
    };


    int finishPacket(Packet* packet) {
        return 1;
    }

    void packetAddFloat(Packet *packet, float value)
    {
        uint32_t rawData = *(uint32_t *)&value;
        packet->data[packet->len] = rawData & 0xFF;
        packet->data[packet->len + 1] = rawData >> 8 & 0xFF;
        packet->data[packet->len + 2] = rawData >> 16 & 0xFF;
        packet->data[packet->len + 3] = rawData >> 24 & 0xFF;
        packet->len += 4;
    }

    void packetAddUint32(Packet *packet, uint32_t value)
    {
        packet->data[packet->len] = value & 0xFF;
        packet->data[packet->len + 1] = value >> 8 & 0xFF;
        packet->data[packet->len + 2] = value >> 16 & 0xFF;
        packet->data[packet->len + 3] = value >> 24 & 0xFF;
        packet->len += 4;
    }

    void packetAddUint16(Packet *packet, uint16_t value)
    {
        packet->data[packet->len] = value & 0xFF;
        packet->data[packet->len + 1] = value >> 8 & 0xFF;
        packet->len += 2;
    }

    void packetAddUint8(Packet *packet, uint8_t value)
    {
        packet->data[packet->len] = value;
        packet->len++;
    }

    float packetGetFloat(Packet *packet, uint8_t index)
    {
        uint32_t rawData = packet->data[index + 3];
        rawData <<= 8;
        rawData += packet->data[index + 2];
        rawData <<= 8;
        rawData += packet->data[index + 1];
        rawData <<= 8;
        rawData += packet->data[index];
        return *(float *)&rawData;
    }

    uint32_t packetGetUint32(Packet *packet, uint8_t index)
    {
        uint32_t rawData = packet->data[index + 3];
        rawData <<= 8;
        rawData += packet->data[index + 2];
        rawData <<= 8;
        rawData += packet->data[index + 1];
        rawData <<= 8;
        rawData += packet->data[index];
        return rawData;
    }

    uint32_t packetGetUint8(Packet *packet, uint8_t index)
    {
        return packet->data[index]; 
    }





};

#endif