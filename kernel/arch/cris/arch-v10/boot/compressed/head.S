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
 *  arch/cris/boot/compressed/head.S
 *
 *  Copyright (C) 1999, 2001 Axis Communications AB
 *
 *  Code that sets up the DRAM registers, calls the
 *  decompressor to unpack the piggybacked kernel, and jumps.
 *
 */

#define ASSEMBLER_MACROS_ONLY
#include <arch/sv_addr_ag.h>

#define RAM_INIT_MAGIC 0x56902387
#define COMMAND_LINE_MAGIC 0x87109563

	;; Exported symbols

	.globl	input_data


	.text

	nop
	di

;; We need to initialze DRAM registers before we start using the DRAM

	cmp.d	RAM_INIT_MAGIC, $r8	; Already initialized?
	beq	dram_init_finished
	nop

#include "../../lib/dram_init.S"

dram_init_finished:

	;; Initiate the PA and PB ports

	move.b   CONFIG_ETRAX_DEF_R_PORT_PA_DATA, $r0
	move.b   $r0, [R_PORT_PA_DATA]

	move.b   CONFIG_ETRAX_DEF_R_PORT_PA_DIR, $r0
	move.b   $r0, [R_PORT_PA_DIR]

	move.b   CONFIG_ETRAX_DEF_R_PORT_PB_DATA, $r0
	move.b   $r0, [R_PORT_PB_DATA]

	move.b   CONFIG_ETRAX_DEF_R_PORT_PB_DIR, $r0
	move.b   $r0, [R_PORT_PB_DIR]

	;; Setup the stack to a suitably high address.
	;; We assume 8 MB is the minimum DRAM in an eLinux
	;; product and put the sp at the top for now.

	move.d	0x40800000, $sp

	;; Figure out where the compressed piggyback image is
	;; in the flash (since we wont try to copy it to DRAM
	;; before unpacking). It is at _edata, but in flash.
	;; Use (_edata - basse) as offset to the current PC.

basse:	move.d	$pc, $r5
	and.d	0x7fffffff, $r5	; strip any non-cache bit
	subq	2, $r5		; compensate for the move.d $pc instr
	move.d	$r5, $r0		; save for later - flash address of 'basse'
	add.d	_edata, $r5
	sub.d	basse, $r5	; $r5 = flash address of '_edata'

	;; Copy text+data to DRAM

	move.d	basse, $r1	; destination
	move.d	_edata, $r2	; end destination
1:	move.w	[$r0+], $r3
	move.w	$r3, [$r1+]
	cmp.d	$r2, $r1
	bcs	1b
	nop

	move.d	$r5, [input_data] ; for the decompressor


	;; Clear the decompressors BSS (between _edata and _end)

	moveq	0, $r0
	move.d	_edata, $r1
	move.d	_end, $r2
1:	move.w	$r0, [$r1+]
	cmp.d	$r2, $r1
	bcs	1b
	nop

	;;  Save command line magic and address.
	move.d	_cmd_line_magic, $r12
	move.d  $r10, [$r12]
	move.d	_cmd_line_addr, $r12
	move.d  $r11, [$r12]

	;; Do the decompression and save compressed size in inptr

	jsr	decompress_kernel

	;; Put start address of root partition in $r9 so the kernel can use it
	;; when mounting from flash

	move.d	[input_data], $r9	; flash address of compressed kernel
	add.d	[inptr], $r9		; size of compressed kernel

	;; Restore command line magic and address.
	move.d  _cmd_line_magic, $r10
	move.d  [$r10], $r10
	move.d  _cmd_line_addr, $r11
	move.d  [$r11], $r11

	;; Enter the decompressed kernel
	move.d	RAM_INIT_MAGIC, $r8	; Tell kernel that DRAM is initialized
	jump	0x40004000	; kernel is linked to this address

	.data

input_data:
	.dword	0		; used by the decompressor
_cmd_line_magic:
	.dword 0
_cmd_line_addr:
	.dword 0
#include "../../lib/hw_settings.S"
