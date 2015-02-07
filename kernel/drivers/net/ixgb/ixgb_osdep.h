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

/*******************************************************************************

  Intel PRO/10GbE Linux driver
  Copyright(c) 1999 - 2008 Intel Corporation.

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".

  Contact Information:
  Linux NICS <linux.nics@intel.com>
  e1000-devel Mailing List <e1000-devel@lists.sourceforge.net>
  Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497

*******************************************************************************/

/* glue for the OS independent part of ixgb
 * includes register access macros
 */

#ifndef _IXGB_OSDEP_H_
#define _IXGB_OSDEP_H_

#include <linux/types.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/sched.h>

#undef ASSERT
#define ASSERT(x)	if (!(x)) BUG()
#define MSGOUT(S, A, B)	printk(KERN_DEBUG S "\n", A, B)

#ifdef DBG
#define DEBUGOUT(S)		printk(KERN_DEBUG S "\n")
#define DEBUGOUT1(S, A...)	printk(KERN_DEBUG S "\n", A)
#else
#define DEBUGOUT(S)
#define DEBUGOUT1(S, A...)
#endif

#define DEBUGFUNC(F) DEBUGOUT(F)
#define DEBUGOUT2 DEBUGOUT1
#define DEBUGOUT3 DEBUGOUT2
#define DEBUGOUT7 DEBUGOUT3

#define IXGB_WRITE_REG(a, reg, value) ( \
	writel((value), ((a)->hw_addr + IXGB_##reg)))

#define IXGB_READ_REG(a, reg) ( \
	readl((a)->hw_addr + IXGB_##reg))

#define IXGB_WRITE_REG_ARRAY(a, reg, offset, value) ( \
	writel((value), ((a)->hw_addr + IXGB_##reg + ((offset) << 2))))

#define IXGB_READ_REG_ARRAY(a, reg, offset) ( \
	readl((a)->hw_addr + IXGB_##reg + ((offset) << 2)))

#define IXGB_WRITE_FLUSH(a) IXGB_READ_REG(a, STATUS)

#define IXGB_MEMCPY memcpy

#endif /* _IXGB_OSDEP_H_ */
