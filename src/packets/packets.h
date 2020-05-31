#ifndef PACKETS_H
#define PACKETS_H


#define PAYLOAD_SIZE    64


struct packet
{
    uint16_t destination_address;
    uint16_t sender_address;
    uint32_t uid;
    uint8_t payload_length;
    uint8_t payload[PAYLOAD_SIZE];
};

void packet_create(struct packet * packet_ptr, uint8_t * buffer);

struct packet * packet_decode(uint8_t * data);

#endif