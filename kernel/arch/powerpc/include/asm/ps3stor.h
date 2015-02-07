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
 * PS3 Storage Devices
 *
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * Copyright 2007 Sony Corp.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _ASM_POWERPC_PS3STOR_H_
#define _ASM_POWERPC_PS3STOR_H_

#include <linux/interrupt.h>

#include <asm/ps3.h>


struct ps3_storage_region {
	unsigned int id;
	u64 start;
	u64 size;
};

struct ps3_storage_device {
	struct ps3_system_bus_device sbd;

	struct ps3_dma_region dma_region;
	unsigned int irq;
	u64 blk_size;

	u64 tag;
	u64 lv1_status;
	struct completion done;

	unsigned long bounce_size;
	void *bounce_buf;
	u64 bounce_lpar;
	dma_addr_t bounce_dma;

	unsigned int num_regions;
	unsigned long accessible_regions;
	unsigned int region_idx;		/* first accessible region */
	struct ps3_storage_region regions[0];	/* Must be last */
};

static inline struct ps3_storage_device *to_ps3_storage_device(struct device *dev)
{
	return container_of(dev, struct ps3_storage_device, sbd.core);
}

extern int ps3stor_setup(struct ps3_storage_device *dev,
			 irq_handler_t handler);
extern void ps3stor_teardown(struct ps3_storage_device *dev);
extern u64 ps3stor_read_write_sectors(struct ps3_storage_device *dev, u64 lpar,
				      u64 start_sector, u64 sectors,
				      int write);
extern u64 ps3stor_send_command(struct ps3_storage_device *dev, u64 cmd,
				u64 arg1, u64 arg2, u64 arg3, u64 arg4);

#endif /* _ASM_POWERPC_PS3STOR_H_ */
