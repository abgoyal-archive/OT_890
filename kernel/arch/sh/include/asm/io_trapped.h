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

#ifndef __ASM_SH_IO_TRAPPED_H
#define __ASM_SH_IO_TRAPPED_H

#include <linux/list.h>
#include <linux/ioport.h>
#include <asm/page.h>

#define IO_TRAPPED_MAGIC 0xfeedbeef

struct trapped_io {
	unsigned int magic;
	struct resource *resource;
	unsigned int num_resources;
	unsigned int minimum_bus_width;
	struct list_head list;
	void __iomem *virt_base;
} __aligned(PAGE_SIZE);

#ifdef CONFIG_IO_TRAPPED
int register_trapped_io(struct trapped_io *tiop);
int handle_trapped_io(struct pt_regs *regs, unsigned long address);

void __iomem *match_trapped_io_handler(struct list_head *list,
				       unsigned long offset,
				       unsigned long size);

#ifdef CONFIG_HAS_IOMEM
extern struct list_head trapped_mem;

static inline void __iomem *
__ioremap_trapped(unsigned long offset, unsigned long size)
{
	return match_trapped_io_handler(&trapped_mem, offset, size);
}
#else
#define __ioremap_trapped(offset, size) NULL
#endif

#ifdef CONFIG_HAS_IOPORT
extern struct list_head trapped_io;

static inline void __iomem *
__ioport_map_trapped(unsigned long offset, unsigned long size)
{
	return match_trapped_io_handler(&trapped_io, offset, size);
}
#else
#define __ioport_map_trapped(offset, size) NULL
#endif

#else
#define register_trapped_io(tiop) (-1)
#define handle_trapped_io(tiop, address) 0
#define __ioremap_trapped(offset, size) NULL
#define __ioport_map_trapped(offset, size) NULL
#endif

#endif /* __ASM_SH_IO_TRAPPED_H */
