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
 * Derived from include/asm-powerpc/iommu.h
 *
 * Copyright IBM Corporation, 2006-2007
 *
 * Author: Jon Mason <jdmason@us.ibm.com>
 * Author: Muli Ben-Yehuda <muli@il.ibm.com>
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#ifndef _ASM_X86_CALGARY_H
#define _ASM_X86_CALGARY_H

#include <linux/spinlock.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/timer.h>
#include <asm/types.h>

struct iommu_table {
	struct cal_chipset_ops *chip_ops; /* chipset specific funcs */
	unsigned long  it_base;      /* mapped address of tce table */
	unsigned long  it_hint;      /* Hint for next alloc */
	unsigned long *it_map;       /* A simple allocation bitmap for now */
	void __iomem  *bbar;         /* Bridge BAR */
	u64	       tar_val;      /* Table Address Register */
	struct timer_list watchdog_timer;
	spinlock_t     it_lock;      /* Protects it_map */
	unsigned int   it_size;      /* Size of iommu table in entries */
	unsigned char  it_busno;     /* Bus number this table belongs to */
};

struct cal_chipset_ops {
	void (*handle_quirks)(struct iommu_table *tbl, struct pci_dev *dev);
	void (*tce_cache_blast)(struct iommu_table *tbl);
	void (*dump_error_regs)(struct iommu_table *tbl);
};

#define TCE_TABLE_SIZE_UNSPECIFIED	~0
#define TCE_TABLE_SIZE_64K		0
#define TCE_TABLE_SIZE_128K		1
#define TCE_TABLE_SIZE_256K		2
#define TCE_TABLE_SIZE_512K		3
#define TCE_TABLE_SIZE_1M		4
#define TCE_TABLE_SIZE_2M		5
#define TCE_TABLE_SIZE_4M		6
#define TCE_TABLE_SIZE_8M		7

extern int use_calgary;

#ifdef CONFIG_CALGARY_IOMMU
extern int calgary_iommu_init(void);
extern void detect_calgary(void);
#else
static inline int calgary_iommu_init(void) { return 1; }
static inline void detect_calgary(void) { return; }
#endif

#endif /* _ASM_X86_CALGARY_H */
