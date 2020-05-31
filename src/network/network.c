
#include <stdio.h>
#include <string.h>

#include "nRF52_Drivers/rtc/rtc.h"
#include "packets/packets.h"
#include "nRF52_Drivers/radio/radio.h"
#include "nRF52_Drivers/uart/uart.h"
#include "network.h"



static millis_t last_sent_packet_timestamp;
static char debug_buffer[100];


void network_tick(void)
{
    if (rtc_time_ellapsed(last_sent_packet_timestamp, 100))
    {
        // put a packet in the queue
        radio_send_packet("Hello world");
        uart_write_string("Packet sent\r\n", 13);
        last_sent_packet_timestamp = rtc_get_millis();
    }
    else
    {
        // wait for packets
        uint8_t * rx_data_ptr = radio_get_packet();
        if (rx_data_ptr != NULL)
        {
            uart_write_string(rx_data_ptr, strlen(rx_data_ptr));
            uart_write_string("\r\r\n", 3);
            sprintf(debug_buffer, "RSSI: %d\r\n", radio_get_rssi());
            uart_write_string(debug_buffer, strlen(debug_buffer));
        }
        
    }
}