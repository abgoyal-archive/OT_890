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

  Intel PRO/1000 Linux driver
  Copyright(c) 1999 - 2006 Intel Corporation.

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


/* glue for the OS independent part of e1000
 * includes register access macros
 */

#ifndef _E1000_OSDEP_H_
#define _E1000_OSDEP_H_

#include <linux/types.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/sched.h>

#ifdef DBG
#define DEBUGOUT(S)		printk(KERN_DEBUG S "\n")
#define DEBUGOUT1(S, A...)	printk(KERN_DEBUG S "\n", A)
#else
#define DEBUGOUT(S)
#define DEBUGOUT1(S, A...)
#endif

#define DEBUGFUNC(F) DEBUGOUT(F "\n")
#define DEBUGOUT2 DEBUGOUT1
#define DEBUGOUT3 DEBUGOUT2
#define DEBUGOUT7 DEBUGOUT3


#define er32(reg)							\
	(readl(hw->hw_addr + ((hw->mac_type >= e1000_82543)		\
			       ? E1000_##reg : E1000_82542_##reg)))

#define ew32(reg, value)						\
	(writel((value), (hw->hw_addr + ((hw->mac_type >= e1000_82543)	\
					 ? E1000_##reg : E1000_82542_##reg))))

#define E1000_WRITE_REG_ARRAY(a, reg, offset, value) ( \
    writel((value), ((a)->hw_addr + \
        (((a)->mac_type >= e1000_82543) ? E1000_##reg : E1000_82542_##reg) + \
        ((offset) << 2))))

#define E1000_READ_REG_ARRAY(a, reg, offset) ( \
    readl((a)->hw_addr + \
        (((a)->mac_type >= e1000_82543) ? E1000_##reg : E1000_82542_##reg) + \
        ((offset) << 2)))

#define E1000_READ_REG_ARRAY_DWORD E1000_READ_REG_ARRAY
#define E1000_WRITE_REG_ARRAY_DWORD E1000_WRITE_REG_ARRAY

#define E1000_WRITE_REG_ARRAY_WORD(a, reg, offset, value) ( \
    writew((value), ((a)->hw_addr + \
        (((a)->mac_type >= e1000_82543) ? E1000_##reg : E1000_82542_##reg) + \
        ((offset) << 1))))

#define E1000_READ_REG_ARRAY_WORD(a, reg, offset) ( \
    readw((a)->hw_addr + \
        (((a)->mac_type >= e1000_82543) ? E1000_##reg : E1000_82542_##reg) + \
        ((offset) << 1)))

#define E1000_WRITE_REG_ARRAY_BYTE(a, reg, offset, value) ( \
    writeb((value), ((a)->hw_addr + \
        (((a)->mac_type >= e1000_82543) ? E1000_##reg : E1000_82542_##reg) + \
        (offset))))

#define E1000_READ_REG_ARRAY_BYTE(a, reg, offset) ( \
    readb((a)->hw_addr + \
        (((a)->mac_type >= e1000_82543) ? E1000_##reg : E1000_82542_##reg) + \
        (offset)))

#define E1000_WRITE_FLUSH() er32(STATUS)

#define E1000_WRITE_ICH_FLASH_REG(a, reg, value) ( \
    writel((value), ((a)->flash_address + reg)))

#define E1000_READ_ICH_FLASH_REG(a, reg) ( \
    readl((a)->flash_address + reg))

#define E1000_WRITE_ICH_FLASH_REG16(a, reg, value) ( \
    writew((value), ((a)->flash_address + reg)))

#define E1000_READ_ICH_FLASH_REG16(a, reg) ( \
    readw((a)->flash_address + reg))

#endif /* _E1000_OSDEP_H_ */
