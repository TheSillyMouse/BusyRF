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

uint32_t packet_create(struct packet * packet_ptr, uint8_t * buffer);

uint32_t packet_decode(uint8_t * data, struct packet * incoming_packet);

#endif