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
 * File:         arch/blackfin/kernel/entry.S
 * Based on:
 * Author:
 *
 * Created:
 * Description:
 *
 * Modified:
 *               Copyright 2004-2006 Analog Devices Inc.
 *
 * Bugs:         Enter bugs at http://blackfin.uclinux.org/
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
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <linux/linkage.h>
#include <asm/thread_info.h>
#include <asm/errno.h>
#include <asm/blackfin.h>
#include <asm/asm-offsets.h>

#include <asm/context.S>

#ifdef CONFIG_EXCPT_IRQ_SYSC_L1
.section .l1.text
#else
.text
#endif

ENTRY(_ret_from_fork)
#ifdef CONFIG_IPIPE
	[--sp] = reti; 		/* IRQs on. */
	SP += 4;
#endif /* CONFIG_IPIPE */
	SP += -12;
	call _schedule_tail;
	SP += 12;
	r0 = [sp + PT_IPEND];
	cc = bittst(r0,1);
	if cc jump .Lin_kernel;
	RESTORE_CONTEXT
	rti;
.Lin_kernel:
	bitclr(r0,1);
	[sp + PT_IPEND] = r0;
	/* do a 'fake' RTI by jumping to [RETI]
	 * to avoid clearing supervisor mode in child
	 */
	r0 = [sp + PT_PC];
	[sp + PT_P0] = r0;

	RESTORE_ALL_SYS
	jump (p0);
ENDPROC(_ret_from_fork)

ENTRY(_sys_fork)
	r0 = -EINVAL;
#if (ANOMALY_05000371)
	nop;
	nop;
	nop;
#endif
	rts;
ENDPROC(_sys_fork)

ENTRY(_sys_vfork)
	r0 = sp;
	r0 += 24;
	[--sp] = rets;
	SP += -12;
	call _bfin_vfork;
	SP += 12;
	rets = [sp++];
	rts;
ENDPROC(_sys_vfork)

ENTRY(_sys_clone)
	r0 = sp;
	r0 += 24;
	[--sp] = rets;
	SP += -12;
	call _bfin_clone;
	SP += 12;
	rets = [sp++];
	rts;
ENDPROC(_sys_clone)

ENTRY(_sys_rt_sigreturn)
	r0 = sp;
	r0 += 24;
	[--sp] = rets;
	SP += -12;
	call _do_rt_sigreturn;
	SP += 12;
	rets = [sp++];
	rts;
ENDPROC(_sys_rt_sigreturn)
