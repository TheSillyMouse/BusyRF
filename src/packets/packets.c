#include <stdint.h>
#include <string.h>
#include "packets.h"

#define OPEN_FLAG               (uint8_t)0xF7
#define CLOSE_FLAG              (uint8_t)0xF7

void packet_create(struct packet * packet_ptr, uint8_t * buffer)
{
    buffer[0] = OPEN_FLAG;

    buffer[1] = packet_ptr->destination_address & 0xFF;
    buffer[2] = (packet_ptr->destination_address >> 8) & 0xFF;

    buffer[3] = packet_ptr->sender_address & 0xFF;
    buffer[4] = (packet_ptr->sender_address >> 8) & 0xFF;
    
    buffer[5] = packet_ptr->uid & 0xFF;
    buffer[6] = (packet_ptr->uid >> 8) & 0xFF;
    buffer[7] = (packet_ptr->uid >> 16) & 0xFF;
    buffer[8] = (packet_ptr->uid >> 24) & 0xFF;

    buffer[9] = packet_ptr->payload_length;

    for (uint8_t i = 0; i < packet_ptr->payload_length; i++)
    {
        buffer[i + 10] = packet_ptr->payload[i];
    }

    buffer[10 + packet_ptr->payload_length] = CLOSE_FLAG;

    return;
}

struct packet * packet_decode(uint8_t * data)
{
    static struct packet packet;

    // if (data[0] != OPEN_FLAG)
    // {
    //     return NULL;
    // }

    // if (data[10 + data[9] + 1] != CLOSE_FLAG)
    // {
    //     return NULL;
    // }
    
    packet.destination_address = data[1];
    packet.destination_address |= (data[2] << 8);

    packet.sender_address = data[3];
    packet.sender_address |= (data[4] << 8);

    packet.uid = data[5];
    packet.uid |= (data[6] << 8);
    packet.uid |= (data[7] << 16);
    packet.uid |= (data[8] << 24);

    packet.payload_length = data[9];

    for (uint8_t i = 0; i < packet.payload_length; i++)
    {
        packet.payload[0] = data[10 + i];
    }

    return (struct packet *)&packet;
}