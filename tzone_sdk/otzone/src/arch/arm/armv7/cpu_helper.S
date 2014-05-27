/**@file cpu_helper.S
 * @brief ARM generic helper routines
 */

.global inv_branch_predict
.func inv_branch_predict
inv_branch_predict:
        @ void inv_branch_predict(void)
        push    {r0}
        mov     r0, #0
        mcr     p15, 0, r0, c7, c5, 6
        isb
        pop     {r0}
        bx      lr
.endfunc

.global read_sctlr
.func read_sctlr
read_sctlr:
        @ u32 read_sctlr(void)
        mrc     p15, 0, r0, c1, c0, 0
        bx      lr
.endfunc

.global write_sctlr
.func write_sctlr
write_sctlr:
        @ void write_sctlr(u32)
        mcr     p15, 0, r0, c1, c0, 0
        isb
        bx      lr
.endfunc

.global get_mpid
.func get_mpid
get_mpid:
        @ u32 get_mpid(void)
        mrc     p15, 0, r0, c0, c0, 5
	isb
        bx      lr
.endfunc

.global clear_icache
.func clear_icache
clear_icache:
	@ void clear_icache(void)
	mcr     p15, 0, r0, c7, c5, 0
	isb
	bx lr
.endfunc

.global get_ttbcr
.func get_ttbcr
get_ttbcr:
        @ u32 get_ttbcr(void)
        mrc     p15, 0, r0, c2, c0, 0
        isb
        bx      lr
.endfunc

.global set_ttbcr
.func set_ttbcr
set_ttbcr:
        @ void set_ttbcr(u32)
        mcr     p15, 0, r0, c2, c0, 0
        isb
        bx      lr
.endfunc

.global read_ttbr0
.func read_ttbr0
read_ttbr0:
        @ u32 read_ttbr0(void)
        mrc     p15, 0, r0, c2, c0, 0
        bx      lr
.endfunc

.global write_ttbr0
.func write_ttbr0
write_ttbr0:
        @ void write_ttbr0(u32)
        mcr     p15, 0, r0, c2, c0, 0
        isb
        bx      lr
.endfunc

.global read_ttbr1
.func read_ttbr1
read_ttbr1:
        @ u32 read_ttbr1(void)
        mrc     p15, 0, r0, c2, c0, 1
        bx      lr
.endfunc

.global write_ttbr1
.func write_ttbr1
write_ttbr1:
        @ void write_ttbr1(u32)
        mcr     p15, 0, r0, c2, c0, 1
        isb
        bx      lr
.endfunc

.global read_dfsr
.func read_dfsr
read_dfsr:
        @ u32 read_dfsr(void)
        mrc     p15, 0, r0, c5, c0, 0
        bx      lr
.endfunc

.global read_dfar
.func read_dfar
read_dfar:
        @ u32 read_dfar(void)
        mrc     p15, 0, r0, c6, c0, 0
        bx      lr
.endfunc


.global read_ifsr
.func read_ifsr
read_ifsr:
        @ u32 read_ifsr(void)
        mrc     p15, 0, r0, c5, c0, 1
        bx      lr
.endfunc

.global read_ifar
.func read_ifar
read_ifar:
        @ u32 read_ifar(void)
        mrc     p15, 0, r0, c6, c0, 2
        bx      lr
.endfunc

.global read_domain
.func read_domain
read_domain:
	@ u32 read_domain(void)
	mrc	p15, 0, r0, c3, c0, 0
	bx 	lr
.endfunc

.global write_domain
.func write_domain
write_domain:
	@ void write_domain(u32)
	mcr	p15, 0, r0, c3, c0, 0
	bx 	lr
.endfunc

.global read_vbar
.func read_vbar
read_vbar:
	@ u32 read_vbar(void)
	mrc 	p15, 0, r0, c12, c0, 0
	bx	lr
.endfunc

.global read_thread_id
.func read_thread_id
read_thread_id:
	@ u32 read_thread_id(void)
	mrc 	p15, 0, r0, c13, c0, 4
	bx	lr
.endfunc

.global write_thread_id
.func write_thread_id
write_thread_id:
	@ void write_thread_id(u32)
	mcr	p15, 0, r0, c13, c0, 4
	bx	lr
.endfunc

