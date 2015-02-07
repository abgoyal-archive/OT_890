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

	
	.global _start
	.global _stext

	.global _rambase
	.global _ramvec
	.global _ramstart
	.global _ramend

#ifdef CONFIG_INIT_LCD
	.global splash_bits
#endif

	.data

/*
 *      Set up the usable of RAM stuff. Size of RAM is determined then
 *      an initial stack set up at the end.
 */
.align 4
_ramvec:
.long   0
_rambase:
.long   0
_ramstart:
.long   0
_ramend:
.long   0

#define	RAMEND	(CONFIG_RAMBASE + CONFIG_RAMSIZE)

#ifdef CONFIG_INIT_LCD
splash_bits:
#include "bootlogo.rh"
#endif
	
	.text
_start:
_stext:	movew	#0x2700,%sr
#ifdef CONFIG_INIT_LCD
	movel	#splash_bits, 0xfffffA00 /* LSSA */
	moveb	#0x28,   0xfffffA05	/* LVPW */
	movew	#0x280,  0xFFFFFa08	/* LXMAX */
	movew	#0x1df,  0xFFFFFa0a	/* LYMAX */
	moveb	#0,      0xfffffa29	/* LBAR */
	moveb	#0,      0xfffffa25	/* LPXCD */
	moveb	#0x08,   0xFFFFFa20	/* LPICF */
	moveb	#0x01,   0xFFFFFA21	/* -ve pol */
	moveb	#0x81,   0xfffffA27	/* LCKCON */
	movew	#0xff00, 0xfffff412	/* LCD pins */
#endif
	moveal  #RAMEND-CONFIG_MEMORY_RESERVE*0x100000 - 0x10, %sp
	movew	#32767, %d0  /* PLL settle wait loop */
1:	subq	#1, %d0
	bne	1b

	/* Copy data segment from ROM to RAM */
	moveal	#_etext, %a0
	moveal	#_sdata, %a1
	moveal	#_edata, %a2

	/* Copy %a0 to %a1 until %a1 == %a2 */
1:	movel	%a0@+, %a1@+
	cmpal	%a1, %a2
	bhi	1b

	moveal	#_sbss, %a0
	moveal	#_ebss, %a1
	/* Copy 0 to %a0 until %a0 == %a1 */
	
1:
	clrl	%a0@+
	cmpal	%a0, %a1
	bhi	1b

        movel   #_sdata, %d0    
        movel   %d0, _rambase        
        movel   #_ebss, %d0
        movel   %d0, _ramstart
	movel	#RAMEND-CONFIG_MEMORY_RESERVE*0x100000, %d0
	movel	%d0, _ramend
	movel	#CONFIG_VECTORBASE,	%d0
	movel	%d0, _ramvec
	
/*
 * load the current task pointer and stack
 */
	lea	init_thread_union, %a0
	lea	0x2000(%a0), %sp

1:	jsr	start_kernel
        bra 1b
_exit:

	jmp	_exit


putc:
	moveb	%d7,0xfffff907
1:
	movew	0xfffff906, %d7
	andw	#0x2000, %d7
	beq	1b
	rts

	.data
env:
	.long	0
	.text

