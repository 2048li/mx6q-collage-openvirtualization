#include <uart.h>

/**
* @brief write a series of characters to UART 
*
* @param c
*/
void serial_puts(char * c)
{
}

/**
* @brief write a character to UART
*
* @param c
*/
void serial_putc(char c)
{
}

/**
* @brief UART initialization
*
* @param uart_id
*/
void serial_init(u32 uart_id)
{
}

/**
* @brief Gets the uart base address and returns it
*
* @param reg_offset
* @param uartid
*
* @return 
*/
u32 read_uart(u32 reg_offset, u32 uartid)
{
}

/**
* @brief Gets the base address of UART and writes the value onto the reg_ptr 
*
* @param reg_offset
* @param value
* @param uartid
*/
void write_uart(u32 reg_offset, u32 value, u32 uartid)
{
}

