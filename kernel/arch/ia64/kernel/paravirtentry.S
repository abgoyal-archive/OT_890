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

/******************************************************************************
 * linux/arch/ia64/xen/paravirtentry.S
 *
 * Copyright (c) 2008 Isaku Yamahata <yamahata at valinux co jp>
 *                    VA Linux Systems Japan K.K.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <asm/asmmacro.h>
#include <asm/asm-offsets.h>
#include "entry.h"

#define DATA8(sym, init_value)			\
	.pushsection .data.read_mostly ;	\
	.align 8 ;				\
	.global sym ;				\
	sym: ;					\
	data8 init_value ;			\
	.popsection

#define BRANCH(targ, reg, breg)		\
	movl reg=targ ;			\
	;;				\
	ld8 reg=[reg] ;			\
	;;				\
	mov breg=reg ;			\
	br.cond.sptk.many breg

#define BRANCH_PROC(sym, reg, breg)				\
	DATA8(paravirt_ ## sym ## _targ, ia64_native_ ## sym) ; \
	GLOBAL_ENTRY(paravirt_ ## sym) ;			\
		BRANCH(paravirt_ ## sym ## _targ, reg, breg) ;	\
	END(paravirt_ ## sym)

#define BRANCH_PROC_UNWINFO(sym, reg, breg)			\
	DATA8(paravirt_ ## sym ## _targ, ia64_native_ ## sym) ; \
	GLOBAL_ENTRY(paravirt_ ## sym) ;			\
		PT_REGS_UNWIND_INFO(0) ;			\
		BRANCH(paravirt_ ## sym ## _targ, reg, breg) ;	\
	END(paravirt_ ## sym)


BRANCH_PROC(switch_to, r22, b7)
BRANCH_PROC_UNWINFO(leave_syscall, r22, b7)
BRANCH_PROC(work_processed_syscall, r2, b7)
BRANCH_PROC_UNWINFO(leave_kernel, r22, b7)
