#define TASK_PC_OFFSET 64 /* offsetof(struct sw_task_cpu_regs, pc) */
#define TASK_SPSR_OFFSET 60 /* offsetof(struct sw_task_cpu_regs, spsr) */
#define TEMP_SWI_REGS_LR_OFFSET 56 /* offsetof(struct swi_temp_regs, lr) */
#define TEMP_SWI_REGS_SPSR_OFFSET 0 /* offsetof(struct swi_temp_regs, spsr) */
#define TEMP_SWI_REGS_R0_OFFSET 4 /* offsetof(struct swi_temp_regs, regs) */
