/* 
 * OpenVirtualization: 
 * For additional details and support contact developer@sierraware.com.
 * Additional documentation can be found at www.openvirtualization.org
 * 
 * Copyright (C) 2011 SierraWare
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 * cpu info implementation
 */

#include <cache.h>
#include <cpu.h>

/**
 * @brief enable cache by setting the cache bit high 
 *
 * @param cache_bit
 */
static void cache_enable(u32 cache_bit)
{
    u32 reg;

    reg = read_sctlr();  /* get control reg. */
    write_sctlr(reg | cache_bit);
}

/* cache_bit must be either SCTLR_I or SCTLR_C */
/**
 * @brief disable cache by setting the cache bit low
 *
 * @param cache_bit
 */
static void cache_disable(u32 cache_bit)
{
    u32 reg;

    reg = read_sctlr();
    write_sctlr(reg & ~cache_bit);
}

/**
 * @brief enable instruction cache 
 */
void icache_enable(void)
{
    cache_enable(SCTLR_I);
}

/**
 * @brief disable instruction cache
 */
void icache_disable(void)
{
    cache_disable(SCTLR_I);
}

/**
 * @brief enable data cache
 */
void dcache_enable(void)
{
    cache_enable(SCTLR_C);
}

/**
 * @brief disable data cache
 */
void dcache_disable(void)
{
    cache_disable(SCTLR_C);
}

/**
 * @brief enable level 1 cache
 */
void enable_l1_cache(void)
{
    icache_enable();
    dcache_enable();
}

/**
 * @brief disable level 1 cache
 */
void disable_l1_cache(void)
{
    icache_disable();
    dcache_disable();
}

/**
 * @brief clean and invalidate the data cache 
 */
void clear_data_cache(void)
{
    clean_invalidate_dcache();
}

/**
 * @brief saves interrupt request
 *
 * @return 
 */
irq_flags_t cpu_irq_save(void)
{
    unsigned long retval;

    __asm volatile (" mrs     %0, cpsr\n\t" " cpsid   if"
                                /* Syntax CPSID <iflags> {, #<p_mode>}
                                 * Note: This instruction is supported 
                                 * from ARM6 and above
                                 */
              :"=r" (retval)::"memory", "cc");

    return retval;
}

/**
 * @brief restores the interrupt request
 *
 * @param flags
 */
void cpu_irq_restore(irq_flags_t flags)
{
    __asm volatile (" msr     cpsr_c, %0"::"r" (flags)
              :"memory", "cc");
}

/**
 * @brief 
 */
void cpu_wait_for_irq(void)
{
        __asm volatile (" wfi ");
}

/**
 * @brief cpu id is found and returned  
 *
 * @return 
 */
u32 get_cpuid(void)
{
    u32 value;
    value = get_mpid();
    return (value & 0x3);   
}

/**
 * @brief branch prediction is enabled  
 */
void enable_branch_prediction(void)
{
    u32 reg;
    reg = read_sctlr();  /* get control reg. */
    reg |= SCTLR_Z;
    write_sctlr(reg);
}

/**
 * @brief enable interrupt request
 */
void arm_irq_enable(void)
{
    __asm( "cpsie if" );
}

/**
 * @brief disable interrupt request
 */
void arm_irq_disable(void)
{
    __asm( "cpsid if" );
}

/**
 * @brief calls the data memory barrier instruction
 */
void data_memory_barrier(void)
{
    dmb();
}

/**
 * @brief calls the data synchronization barrier instruction
 */
void data_sync_barrier(void)
{
    dsb();
}

/**
 * @brief calls the instruction synchronization barrier instruction
 */
void instruction_sync_barrier(void)
{
    isb();
}
