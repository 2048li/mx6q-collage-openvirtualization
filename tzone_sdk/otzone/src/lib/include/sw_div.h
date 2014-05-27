#ifndef __SW_DIV_H_
#define __SW_DIV_H_

#include <sw_types.h>


/**
 * @brief 
 * The layout of this structure must not be altered. 
 * The return type and member order is defined by
 * the ARM eabi. Quotient is returned in r0-r1 and remainder in r2-r3. 
 */
typedef struct
{
  u64 quot;
  u64 rem;
} ulldiv_t;

/**
 * @brief 
 * Simple unsigned int division with minimal optimizations.
 * For best results, use a tail-call optimizing seasoning.
 *
 * @param dividend
 * @param divisor
 *
 * @return 
 */
u32 __aeabi_uidiv(u32 dividend, u32 divisor);

/**
 * @brief 
 * Does 64 bit unsigned int division, returning a struct
 * containing the quotient and remainder 
 *
 * @param dividend
 * @param divisor
 *
 * @return 
 */
ulldiv_t __aeabi_uldivmod(u64 dividend, u64 divisor);

/**
* @brief 
*
* @param dividend
* @param divisor
*
* @return 
*/
u32 __aeabi_idivmod(u32 dividend, u32 divisor);

#endif
