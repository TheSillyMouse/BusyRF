/**
 * BusyRF main file
 *
 * @author  TheSillyMouse
 *
 * @date    2020-05-31
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "nrf52.h"
#include "nRF52_Drivers/clock/clock.h"
#include "nRF52_Drivers/radio/radio.h"
#include "nRF52_Drivers/uart/uart.h"
#include "nRF52_Drivers/gpio/gpio.h"
#include "nRF52_Drivers/rtc/rtc.h"
#include "serial_port/serial_port.h"
#include "network/network.h"


int main(void)
{
    clock_init();
    rtc_init();

    serial_port_init();

    gpio_out_set(6);
    gpio_config_output(6);

    gpio_config_input(8, GPIO_PIN_CNF_PULL_Disabled);
    
    uart_init();
    radio_init();
    
    __enable_irq();
    
    uart_write_string("Started...\r\n", 12);

    while (true)
    {
        radio_tick();
        network_tick();   
    }
}


/** @} */
