#include <string.h>
#include "serial_port.h"
#include "nRF52_Drivers/spis/spis.h"
#include "nrf52.h"

#define TX_BUFFER_SIZE      128
#define RX_BUFFER_SIZE      128


static void serial_port_irq(void);


static volatile uint8_t tx_buffer[TX_BUFFER_SIZE];
static volatile uint8_t rx_buffer[RX_BUFFER_SIZE];


void serial_port_init(void)
{
    //setup buffers
    memset((uint8_t *)tx_buffer, (uint8_t)0, TX_BUFFER_SIZE);
    memset((uint8_t *)rx_buffer, (uint8_t)0, RX_BUFFER_SIZE);

    //setup spis interrupt handler
    spis_set_irq_callback(serial_port_irq);

    //enable interrupt
    spis_interrupt_enable(SPIS_INTENSET_ACQUIRED_Msk);
    spis_interrupt_enable(SPIS_INTENSET_END_Msk);

    NVIC_SetPriority(SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn, 6);
    NVIC_ClearPendingIRQ(SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn);
    NVIC_EnableIRQ(SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn);
    
    //this should trigger an interrupt
    spis_enable_task_acquire();
}

uint8_t * serial_port_get(void)
{
    return  (uint8_t *)rx_buffer;
}

void serial_port_clear_rx_buffer(void)
{
    memset((uint8_t *)rx_buffer, (uint8_t)0, RX_BUFFER_SIZE);
}

void serial_port_put(uint8_t * data, size_t len)
{
    if (len >= TX_BUFFER_SIZE - 1)
        return;
    
    spis_interrupt_disable(SPIS_INTENCLR_ACQUIRED_Msk);
    memcpy((uint8_t *)tx_buffer, (uint8_t *)data, len);
    uint32_t dma_ptr = 0x0;
    spis_dma_get_rx_pointer(&dma_ptr);
    if (dma_ptr != (uint32_t)&tx_buffer[0])
    {
        spis_enable_task_acquire();
    }
    spis_interrupt_enable(SPIS_INTENSET_ACQUIRED_Msk);
}

static void serial_port_irq(void)
{
    if (spis_get_event_acquired())
    {
        spis_clear_event_acquired();

        spis_set_tx_buffer((uint8_t *)tx_buffer, TX_BUFFER_SIZE);
        spis_set_rx_buffer((uint8_t *)rx_buffer, RX_BUFFER_SIZE);

        spis_enable_task_release();
    }

    if (spis_get_event_end())
    {
        spis_clear_event_end();

        spis_enable_task_acquire();
    }

    if (spis_get_event_endrx())
    {
        spis_clear_event_endrx();

        spis_enable_task_acquire();
    }
}