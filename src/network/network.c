#include <stdio.h>
#include <string.h>

#include "nRF52_Drivers/rtc/rtc.h"
#include "packets/packets.h"
#include "nRF52_Drivers/radio/radio.h"
#include "nRF52_Drivers/uart/uart.h"
#include "packets/packets.h"
#include "network.h"



static millis_t last_sent_packet_timestamp;
static uint32_t packet_counter;
static volatile char debug_buffer[100];


void network_tick(void)
{
    if (rtc_time_ellapsed(last_sent_packet_timestamp, 100))
    {
        // put a packet in the queue
        struct packet outcoming_packet;
        memset((struct packet *)&outcoming_packet, (uint8_t)0, sizeof(struct packet));
        outcoming_packet.destination_address = 0xAAAA;
        outcoming_packet.sender_address = 0x5555;
        outcoming_packet.uid = packet_counter++;
        outcoming_packet.payload_length = (uint8_t)sprintf(outcoming_packet.payload, "Hello world: %d", packet_counter);
        
        uint8_t tx_buffer[80];
        memset(tx_buffer, (uint8_t)0, sizeof(tx_buffer));
        uint32_t packet_size = packet_create(&outcoming_packet, tx_buffer);
        if (packet_size > 0)
        {
            radio_send_packet(tx_buffer, packet_size);         
        } 
        last_sent_packet_timestamp = rtc_get_millis();
    }
    else
    {
        // wait for packets
        uint8_t * rx_data_ptr = radio_get_packet();
        if (rx_data_ptr != NULL)
        {
            struct packet incoming_packet;
            memset((struct packet *)&incoming_packet, (uint8_t)0, sizeof(struct packet));

            packet_decode(rx_data_ptr, &incoming_packet);
            uart_write_string("Receive: \r\n", 12);
            uart_write_string("payload: ", 10);
            uart_write_string(incoming_packet.payload, incoming_packet.payload_length);
            uart_write_string("\r\n", 2);
        }
        
    }
}