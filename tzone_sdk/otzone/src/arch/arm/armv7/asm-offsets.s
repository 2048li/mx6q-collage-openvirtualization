	.cpu cortex-a9
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 18, 4
	.file	"asm-offsets.c"
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.align	2
	.global	main
	.type	main, %function
main:
.LFB8:
	.file 1 "/home/shenchun/github/mx6q-collage-openvirtualization/tzone_sdk/otzone/src/arch/arm/armv7/asm-offsets.c"
	.loc 1 43 0
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
.LCFI0:
	.cfi_def_cfa_offset 4
	.cfi_offset 11, -4
	add	fp, sp, #0
.LCFI1:
	.cfi_def_cfa_register 11
	.loc 1 44 0
#APP
@ 44 "/home/shenchun/github/mx6q-collage-openvirtualization/tzone_sdk/otzone/src/arch/arm/armv7/asm-offsets.c" 1
	
->TASK_PC_OFFSET #64 offsetof(struct sw_task_cpu_regs, pc)
@ 0 "" 2
	.loc 1 45 0
@ 45 "/home/shenchun/github/mx6q-collage-openvirtualization/tzone_sdk/otzone/src/arch/arm/armv7/asm-offsets.c" 1
	
->TASK_SPSR_OFFSET #60 offsetof(struct sw_task_cpu_regs, spsr)
@ 0 "" 2
	.loc 1 46 0
@ 46 "/home/shenchun/github/mx6q-collage-openvirtualization/tzone_sdk/otzone/src/arch/arm/armv7/asm-offsets.c" 1
	
->TEMP_SWI_REGS_LR_OFFSET #56 offsetof(struct swi_temp_regs, lr)
@ 0 "" 2
	.loc 1 47 0
@ 47 "/home/shenchun/github/mx6q-collage-openvirtualization/tzone_sdk/otzone/src/arch/arm/armv7/asm-offsets.c" 1
	
->TEMP_SWI_REGS_SPSR_OFFSET #0 offsetof(struct swi_temp_regs, spsr)
@ 0 "" 2
	.loc 1 48 0
@ 48 "/home/shenchun/github/mx6q-collage-openvirtualization/tzone_sdk/otzone/src/arch/arm/armv7/asm-offsets.c" 1
	
->TEMP_SWI_REGS_R0_OFFSET #4 offsetof(struct swi_temp_regs, regs)
@ 0 "" 2
	.loc 1 52 0
	mov	r3, #0
	.loc 1 53 0
	mov	r0, r3
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.cfi_endproc
.LFE8:
	.size	main, .-main
.Letext0:
	.file 2 "/home/shenchun/github/mx6q-collage-openvirtualization/tzone_sdk/otzone/src/arch/arm/armv7/include/cpu_task.h"
	.file 3 "/home/shenchun/github/mx6q-collage-openvirtualization/tzone_sdk/otzone/src/arch/arm/armv7/include/cpu_data.h"
	.file 4 "/home/shenchun/github/mx6q-collage-openvirtualization/tzone_sdk/include/sw_common_types.h"
	.file 5 "/home/shenchun/github/mx6q-collage-openvirtualization/tzone_sdk/otzone/src/lib/include/sw_types.h"
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x129
	.2byte	0x2
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF14
	.byte	0x1
	.4byte	.LASF15
	.4byte	.Ltext0
	.4byte	.Letext0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.4byte	.LASF2
	.byte	0x4
	.byte	0x1e
	.4byte	0x2c
	.uleb128 0x3
	.byte	0x4
	.byte	0x7
	.4byte	.LASF0
	.uleb128 0x3
	.byte	0x1
	.byte	0x8
	.4byte	.LASF1
	.uleb128 0x4
	.ascii	"u32\000"
	.byte	0x5
	.byte	0x3d
	.4byte	0x2c
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.4byte	.LASF3
	.uleb128 0x3
	.byte	0x4
	.byte	0x7
	.4byte	.LASF4
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.4byte	.LASF5
	.uleb128 0x3
	.byte	0x2
	.byte	0x5
	.4byte	.LASF6
	.uleb128 0x5
	.byte	0x4
	.byte	0x5
	.ascii	"int\000"
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.4byte	.LASF7
	.uleb128 0x3
	.byte	0x2
	.byte	0x7
	.4byte	.LASF8
	.uleb128 0x3
	.byte	0x8
	.byte	0x5
	.4byte	.LASF9
	.uleb128 0x6
	.4byte	.LASF12
	.byte	0x44
	.byte	0x2
	.byte	0x1f
	.4byte	0xcd
	.uleb128 0x7
	.4byte	.LASF10
	.byte	0x2
	.byte	0x21
	.4byte	0xcd
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.uleb128 0x8
	.ascii	"sp\000"
	.byte	0x2
	.byte	0x23
	.4byte	0x3a
	.byte	0x2
	.byte	0x23
	.uleb128 0x34
	.uleb128 0x8
	.ascii	"lr\000"
	.byte	0x2
	.byte	0x25
	.4byte	0x3a
	.byte	0x2
	.byte	0x23
	.uleb128 0x38
	.uleb128 0x7
	.4byte	.LASF11
	.byte	0x2
	.byte	0x27
	.4byte	0x3a
	.byte	0x2
	.byte	0x23
	.uleb128 0x3c
	.uleb128 0x8
	.ascii	"pc\000"
	.byte	0x2
	.byte	0x29
	.4byte	0x3a
	.byte	0x2
	.byte	0x23
	.uleb128 0x40
	.byte	0
	.uleb128 0x9
	.4byte	0x3a
	.4byte	0xdd
	.uleb128 0xa
	.4byte	0x2c
	.byte	0xc
	.byte	0
	.uleb128 0x6
	.4byte	.LASF13
	.byte	0x3c
	.byte	0x3
	.byte	0x2d
	.4byte	0x113
	.uleb128 0x7
	.4byte	.LASF11
	.byte	0x3
	.byte	0x2f
	.4byte	0x3a
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.uleb128 0x7
	.4byte	.LASF10
	.byte	0x3
	.byte	0x31
	.4byte	0xcd
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x8
	.ascii	"lr\000"
	.byte	0x3
	.byte	0x33
	.4byte	0x3a
	.byte	0x2
	.byte	0x23
	.uleb128 0x38
	.byte	0
	.uleb128 0xb
	.byte	0x1
	.4byte	.LASF16
	.byte	0x1
	.byte	0x2a
	.byte	0x1
	.4byte	0x61
	.4byte	.LFB8
	.4byte	.LFE8
	.4byte	.LLST0
	.byte	0
	.section	.debug_abbrev,"",%progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_loc,"",%progbits
.Ldebug_loc0:
.LLST0:
	.4byte	.LFB8-.Ltext0
	.4byte	.LCFI0-.Ltext0
	.2byte	0x2
	.byte	0x7d
	.sleb128 0
	.4byte	.LCFI0-.Ltext0
	.4byte	.LCFI1-.Ltext0
	.2byte	0x2
	.byte	0x7d
	.sleb128 4
	.4byte	.LCFI1-.Ltext0
	.4byte	.LFE8-.Ltext0
	.2byte	0x2
	.byte	0x7b
	.sleb128 4
	.4byte	0
	.4byte	0
	.section	.debug_aranges,"",%progbits
	.4byte	0x1c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	0
	.4byte	0
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.section	.debug_str,"MS",%progbits,1
.LASF2:
	.ascii	"size_t\000"
.LASF11:
	.ascii	"spsr\000"
.LASF13:
	.ascii	"swi_temp_regs\000"
.LASF14:
	.ascii	"GNU C 4.6.2 20110630 (prerelease)\000"
.LASF1:
	.ascii	"unsigned char\000"
.LASF4:
	.ascii	"long unsigned int\000"
.LASF8:
	.ascii	"short unsigned int\000"
.LASF12:
	.ascii	"sw_task_cpu_regs\000"
.LASF16:
	.ascii	"main\000"
.LASF0:
	.ascii	"unsigned int\000"
.LASF3:
	.ascii	"long long unsigned int\000"
.LASF10:
	.ascii	"regs\000"
.LASF15:
	.ascii	"/home/shenchun/github/mx6q-collage-openvirtualizati"
	.ascii	"on/tzone_sdk/otzone/src/arch/arm/armv7/asm-offsets."
	.ascii	"c\000"
.LASF9:
	.ascii	"long long int\000"
.LASF6:
	.ascii	"short int\000"
.LASF7:
	.ascii	"long int\000"
.LASF5:
	.ascii	"signed char\000"
	.ident	"GCC: (Freescale MAD -- Linaro 2011.07 -- Built at 2011/08/10 09:20) 4.6.2 20110630 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
