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
 * Access to the shared data page by the vDSO & syscall map
 *
 * Copyright (C) 2004 Benjamin Herrenschmuidt (benh@kernel.crashing.org), IBM Corp.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <asm/processor.h>
#include <asm/ppc_asm.h>
#include <asm/asm-offsets.h>
#include <asm/unistd.h>
#include <asm/vdso.h>

	.text
V_FUNCTION_BEGIN(__get_datapage)
  .cfi_startproc
	/* We don't want that exposed or overridable as we want other objects
	 * to be able to bl directly to here
	 */
	.protected __get_datapage
	.hidden __get_datapage

	mflr	r0
  .cfi_register lr,r0

	bcl	20,31,1f
	.global	__kernel_datapage_offset;
__kernel_datapage_offset:
	.long	0
1:
	mflr	r3
	mtlr	r0
	lwz	r0,0(r3)
	add	r3,r0,r3
	blr
  .cfi_endproc
V_FUNCTION_END(__get_datapage)

/*
 * void *__kernel_get_syscall_map(unsigned int *syscall_count) ;
 *
 * returns a pointer to the syscall map. the map is agnostic to the
 * size of "long", unlike kernel bitops, it stores bits from top to
 * bottom so that memory actually contains a linear bitmap
 * check for syscall N by testing bit (0x80000000 >> (N & 0x1f)) of
 * 32 bits int at N >> 5.
 */
V_FUNCTION_BEGIN(__kernel_get_syscall_map)
  .cfi_startproc
	mflr	r12
  .cfi_register lr,r12
	mr	r4,r3
	bl	V_LOCAL_FUNC(__get_datapage)
	mtlr	r12
	addi	r3,r3,CFG_SYSCALL_MAP64
	cmpli	cr0,r4,0
	crclr	cr0*4+so
	beqlr
	li	r0,__NR_syscalls
	stw	r0,0(r4)
	blr
  .cfi_endproc
V_FUNCTION_END(__kernel_get_syscall_map)


/*
 * void unsigned long  __kernel_get_tbfreq(void);
 *
 * returns the timebase frequency in HZ
 */
V_FUNCTION_BEGIN(__kernel_get_tbfreq)
  .cfi_startproc
	mflr	r12
  .cfi_register lr,r12
	bl	V_LOCAL_FUNC(__get_datapage)
	ld	r3,CFG_TB_TICKS_PER_SEC(r3)
	mtlr	r12
	crclr	cr0*4+so
	blr
  .cfi_endproc
V_FUNCTION_END(__kernel_get_tbfreq)
