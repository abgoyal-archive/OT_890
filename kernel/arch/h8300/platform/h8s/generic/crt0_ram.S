/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/*
 *  linux/arch/h8300/platform/h8s/edosk2674/crt0_ram.S
 *
 *  Yoshinori Sato <ysato@users.sourceforge.jp>
 *
 *  Platform depend startup
 *  Target Archtecture:	generic
 *  Memory Layout     :	RAM
 */

#define ASSEMBLY

#include <asm/linkage.h>
#include <asm/regs267x.h>
			
#if !defined(CONFIG_BLKDEV_RESERVE)
#if defined(CONFIG_GDB_DEBUG)
#define RAMEND (__ramend - 0xc000)
#else
#define RAMEND __ramend
#endif
#else
#define RAMEND CONFIG_BLKDEV_RESERVE_ADDRESS
#endif
	
	.global SYMBOL_NAME(_start)
	.global SYMBOL_NAME(_command_line)
	.global SYMBOL_NAME(_platform_gpio_table)
	.global SYMBOL_NAME(_target_name)
	
	.h8300s

	.section .text
	.file	"crt0_ram.S"

	/* CPU Reset entry */
SYMBOL_NAME_LABEL(_start)
	mov.l	#RAMEND,sp
	ldc	#0x80,ccr
	ldc	#0x00,exr

	/* Peripheral Setup */
	bclr	#4,@INTCR:8	/* interrupt mode 2 */
	bset	#5,@INTCR:8

#if defined(CONFIG_MTD_UCLINUX)
	/* move romfs image */
	jsr	@__move_romfs	
#endif
	
	/* .bss clear */
	mov.l	#__sbss,er5
	mov.l	er5,er6
	mov.l	#__ebss,er4
	sub.l	er5,er4
	shlr	#2,er4
	sub.l	er0,er0
1:	
	mov.l	er0,@er5
	adds	#4,er5
	dec.l	#1,er4
	bne	1b

	/* copy kernel commandline */
	mov.l	#COMMAND_START,er5
	mov.l	#SYMBOL_NAME(command_line),er6
	mov.w	#512,r4
	eepmov.w

	/* uClinux kernel start */
	ldc	#0x90,ccr	/* running kernel */
	mov.l	#SYMBOL_NAME(init_thread_union),sp
	add.l	#0x2000,sp
	jsr	@_start_kernel
_exit:

	jmp	_exit

	rts

	/* I/O port assign information */
__platform_gpio_table:	
	mov.l	#gpio_table,er0
	rts

gpio_table:
	;; P1DDR
	;;      used,ddr
	.byte	0x00,0x00
	;; P2DDR
	.byte	0x00,0x00
	;; P3DDR
	.byte	0x00,0x00
	;; dummy
	.byte	0x00,0x00
	;; P5DDR
	.byte	0x00,0x00
	;; P6DDR
	.byte	0x00,0x00
	;; P7DDR
	.byte	0x00,0x00
	;; P8DDR
	.byte	0x00,0x00
	;; dummy
	.byte	0x00,0x00
	;; PADDR
	.byte	0x00,0x00
	;; PBDDR
	.byte	0x00,0x00
	;; PCDDR
	.byte	0x00,0x00
	;; PDDDR
	.byte	0x00,0x00
	;; PEDDR
	.byte	0x00,0x00
	;; PFDDR
	.byte	0x00,0x00
	;; PGDDR
	.byte	0x00,0x00
	;; PHDDR
	.byte	0x00,0x00

__target_name:	
	.asciz	"generic"
	
	.section .bootvec,"ax"
	jmp	@SYMBOL_NAME(_start)
