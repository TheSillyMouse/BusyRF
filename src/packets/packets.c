#include <stdint.h>
#include <string.h>
#include "packets.h"

#define OPEN_FLAG               (uint8_t)0xF7
#define CLOSE_FLAG              (uint8_t)0xF7

uint32_t packet_create(struct packet * packet_ptr, uint8_t * buffer)
{
    if (packet_ptr->payload_length == 0)
    {
        return 0;
    }
    
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

    return 11 + packet_ptr->payload_length;
}

uint32_t packet_decode(uint8_t * data, struct packet * incoming_packet)
{
    if (data[0] != OPEN_FLAG)
    {
        return 0;
    }

    if (data[9 + data[9] + 1] != CLOSE_FLAG)
    {
        return 0;
    }
    
    incoming_packet->destination_address = data[1];
    incoming_packet->destination_address |= (data[2] << 8);

    incoming_packet->sender_address = data[3];
    incoming_packet->sender_address |= (data[4] << 8);

    incoming_packet->uid = data[5];
    incoming_packet->uid |= (data[6] << 8);
    incoming_packet->uid |= (data[7] << 16);
    incoming_packet->uid |= (data[8] << 24);

    incoming_packet->payload_length = data[9];

    for (uint8_t i = 0; i < incoming_packet->payload_length; i++)
    {
        incoming_packet->payload[i] = data[10 + i];
    }

    return 0;
}