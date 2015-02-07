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
 *  cx18 driver PCI memory mapped IO access routines
 *
 *  Copyright (C) 2007  Hans Verkuil <hverkuil@xs4all.nl>
 *  Copyright (C) 2008  Andy Walls <awalls@radix.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA
 */

#include "cx18-driver.h"
#include "cx18-io.h"
#include "cx18-irq.h"

void cx18_memset_io(struct cx18 *cx, void __iomem *addr, int val, size_t count)
{
	u8 __iomem *dst = addr;
	u16 val2 = val | (val << 8);
	u32 val4 = val2 | (val2 << 16);

	/* Align writes on the CX23418's addresses */
	if ((count > 0) && ((unsigned long)dst & 1)) {
		cx18_writeb(cx, (u8) val, dst);
		count--;
		dst++;
	}
	if ((count > 1) && ((unsigned long)dst & 2)) {
		cx18_writew(cx, val2, dst);
		count -= 2;
		dst += 2;
	}
	while (count > 3) {
		cx18_writel(cx, val4, dst);
		count -= 4;
		dst += 4;
	}
	if (count > 1) {
		cx18_writew(cx, val2, dst);
		count -= 2;
		dst += 2;
	}
	if (count > 0)
		cx18_writeb(cx, (u8) val, dst);
}

void cx18_sw1_irq_enable(struct cx18 *cx, u32 val)
{
	cx18_write_reg_expect(cx, val, SW1_INT_STATUS, ~val, val);
	cx->sw1_irq_mask = cx18_read_reg(cx, SW1_INT_ENABLE_PCI) | val;
	cx18_write_reg(cx, cx->sw1_irq_mask, SW1_INT_ENABLE_PCI);
}

void cx18_sw1_irq_disable(struct cx18 *cx, u32 val)
{
	cx->sw1_irq_mask = cx18_read_reg(cx, SW1_INT_ENABLE_PCI) & ~val;
	cx18_write_reg(cx, cx->sw1_irq_mask, SW1_INT_ENABLE_PCI);
}

void cx18_sw2_irq_enable(struct cx18 *cx, u32 val)
{
	cx18_write_reg_expect(cx, val, SW2_INT_STATUS, ~val, val);
	cx->sw2_irq_mask = cx18_read_reg(cx, SW2_INT_ENABLE_PCI) | val;
	cx18_write_reg(cx, cx->sw2_irq_mask, SW2_INT_ENABLE_PCI);
}

void cx18_sw2_irq_disable(struct cx18 *cx, u32 val)
{
	cx->sw2_irq_mask = cx18_read_reg(cx, SW2_INT_ENABLE_PCI) & ~val;
	cx18_write_reg(cx, cx->sw2_irq_mask, SW2_INT_ENABLE_PCI);
}

void cx18_sw2_irq_disable_cpu(struct cx18 *cx, u32 val)
{
	u32 r;
	r = cx18_read_reg(cx, SW2_INT_ENABLE_CPU);
	cx18_write_reg(cx, r & ~val, SW2_INT_ENABLE_CPU);
}

void cx18_setup_page(struct cx18 *cx, u32 addr)
{
	u32 val;
	val = cx18_read_reg(cx, 0xD000F8);
	val = (val & ~0x1f00) | ((addr >> 17) & 0x1f00);
	cx18_write_reg(cx, val, 0xD000F8);
}
