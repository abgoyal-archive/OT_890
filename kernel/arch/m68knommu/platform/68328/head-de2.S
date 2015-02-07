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


#if defined(CONFIG_RAM32MB)
#define	MEM_END	0x02000000	/* Memory size 32Mb */
#elif defined(CONFIG_RAM16MB)
#define	MEM_END	0x01000000	/* Memory size 16Mb */
#else
#define	MEM_END	0x00800000	/* Memory size 8Mb */
#endif

#undef CRT_DEBUG

.macro	PUTC CHAR
#ifdef CRT_DEBUG
	moveq	#\CHAR, %d7
	jsr	putc
#endif
.endm

	.global	_start
	.global _rambase
	.global _ramvec
	.global	_ramstart
	.global	_ramend
	
	.data

/*
 *	Set up the usable of RAM stuff
 */
_rambase:
	.long	0
_ramvec:
	.long	0
_ramstart:
	.long	0
_ramend:
	.long	0

	.text

_start:

/*
 * Setup initial stack
 */
	/* disable all interrupts */
	movew	#0x2700, %sr
	movel	#-1, 0xfffff304
	movel	#MEM_END-4, %sp

	PUTC	'\r'
	PUTC	'\n'
	PUTC	'A'
	PUTC	'B'

/*
 *	Determine end of RAM
 */

	movel	#MEM_END, %a0
	movel	%a0, _ramend

	PUTC	'C'

/*
 *	Move ROM filesystem above bss :-)
 */

	moveal	#_sbss, %a0			/* romfs at the start of bss */
	moveal	#_ebss, %a1			/* Set up destination  */
	movel	%a0, %a2			/* Copy of bss start */

	movel	8(%a0), %d1			/* Get size of ROMFS */
	addql	#8, %d1				/* Allow for rounding */
	andl	#0xfffffffc, %d1	/* Whole words */

	addl	%d1, %a0			/* Copy from end */
	addl	%d1, %a1			/* Copy from end */
	movel	%a1, _ramstart		/* Set start of ram */

1:
	movel	-(%a0), %d0			/* Copy dword */
	movel	%d0, -(%a1)
	cmpl	%a0, %a2			/* Check if at end */
	bne	1b

	PUTC	'D'

/*
 * Initialize BSS segment to 0
 */

	lea	_sbss, %a0
	lea	_ebss, %a1

	/* Copy 0 to %a0 until %a0 == %a1 */
2:	cmpal	%a0, %a1
	beq	1f
	clrl	(%a0)+
	bra	2b
1:

	PUTC	'E'

/*
 * Load the current task pointer and stack
 */

	lea	init_thread_union, %a0
	lea	0x2000(%a0), %sp

	PUTC	'F'
	PUTC	'\r'
	PUTC	'\n'

/*
 * Go
 */

	jmp	start_kernel

/*
 * Local functions
 */
 
#ifdef CRT_DEBUG
putc:
	moveb	%d7, 0xfffff907
1:
	movew	0xfffff906, %d7
	andw	#0x2000, %d7
	beq	1b
	rts
#endif
