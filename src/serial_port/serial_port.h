#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <inttypes.h>

void serial_port_init(void);

uint8_t * serial_port_get(void);

void serial_port_clear_rx_buffer(void);

void serial_port_put(uint8_t * data, size_t len);

#endif