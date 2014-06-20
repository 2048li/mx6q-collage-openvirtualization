#include <sw_timer.h>
#include <secure_timer.h>

/**
* @brief Secure timer initialization 
*
* @param timer_base
*/
void secure_timer_init(u32 timer_base)
{
}

/**
* @brief Clears the interrupt register of the secure timer
*
* @param timer_base
* @param timer_module_index
*/
void secure_timer_clearirq(u32 timer_base, u32 timer_module_index)
{
}

/**
* @brief Enable the Secure timer 
*
* @param timer_base
* @param timer_module_index
*/
void secure_timer_disable(u32 timer_base, u32 timer_module_index)
{
}

/**
* @brief Disable the secure timer
*
* @param timer_base
* @param timer_module_index
*/
void secure_timer_enable(u32 timer_base, u32 timer_module_index)
{
}

/**
* @brief Reads the masked interrupt status register of SP804 timer 
*
* @param timer_base
* @param tim_mod_index
*
* @return 
*/
u32 secure_timer_read_mis(u32 timer_base, u32 tim_mod_index)
{
}

/**
* @brief Reads the Raw interrupt status register of SP804 timer 
*
* @param timer_base
* @param tim_mod_index
*
* @return 
*/
u32 secure_timer_read_ris(u32 timer_base, u32 tim_mod_index)
{
}

/**
* @brief Sets the requested timer base value based on the mode 
*
* @param timer_base
* @param tim_mod_index
* @param mode
* @param usecs
*/
void secure_set_timer(u32 timer_base, u32 tim_mod_index, u32 mode, u32 usecs)
{
}

/**
* @brief Reads the timer value of the requested timer base 
*
* @param timer_base
* @param tim_mod_index
*
* @return 
*/
u32 secure_read_timer(u32 timer_base, u32 tim_mod_index)
{
	return 0;
}


/**
* @brief Handler for secure timer interrupt request 
*
* @param irq
*
* @return 
*/
void secure_timer_irq_handler(int irq)
{
}

/**
 * @brief This function retrieves the clock period in micro scale  
 *
 * @return 
 */
u32 get_clock_period_us(void)
{
	return 0; 
}

u64 get_clock_period(void)
{
	return 0; 
}

u64 get_timer_period(void)
{
	return 0;
}

u32 read_freerunning_cntr(void)
{
	return 0;
}



