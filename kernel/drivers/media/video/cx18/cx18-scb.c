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
 *  cx18 System Control Block initialization
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
#include "cx18-scb.h"

void cx18_init_scb(struct cx18 *cx)
{
	cx18_setup_page(cx, SCB_OFFSET);
	cx18_memset_io(cx, cx->scb, 0, 0x10000);

	cx18_writel(cx, IRQ_APU_TO_CPU,     &cx->scb->apu2cpu_irq);
	cx18_writel(cx, IRQ_CPU_TO_APU_ACK, &cx->scb->cpu2apu_irq_ack);
	cx18_writel(cx, IRQ_HPU_TO_CPU,     &cx->scb->hpu2cpu_irq);
	cx18_writel(cx, IRQ_CPU_TO_HPU_ACK, &cx->scb->cpu2hpu_irq_ack);
	cx18_writel(cx, IRQ_PPU_TO_CPU,     &cx->scb->ppu2cpu_irq);
	cx18_writel(cx, IRQ_CPU_TO_PPU_ACK, &cx->scb->cpu2ppu_irq_ack);
	cx18_writel(cx, IRQ_EPU_TO_CPU,     &cx->scb->epu2cpu_irq);
	cx18_writel(cx, IRQ_CPU_TO_EPU_ACK, &cx->scb->cpu2epu_irq_ack);

	cx18_writel(cx, IRQ_CPU_TO_APU,     &cx->scb->cpu2apu_irq);
	cx18_writel(cx, IRQ_APU_TO_CPU_ACK, &cx->scb->apu2cpu_irq_ack);
	cx18_writel(cx, IRQ_HPU_TO_APU,     &cx->scb->hpu2apu_irq);
	cx18_writel(cx, IRQ_APU_TO_HPU_ACK, &cx->scb->apu2hpu_irq_ack);
	cx18_writel(cx, IRQ_PPU_TO_APU,     &cx->scb->ppu2apu_irq);
	cx18_writel(cx, IRQ_APU_TO_PPU_ACK, &cx->scb->apu2ppu_irq_ack);
	cx18_writel(cx, IRQ_EPU_TO_APU,     &cx->scb->epu2apu_irq);
	cx18_writel(cx, IRQ_APU_TO_EPU_ACK, &cx->scb->apu2epu_irq_ack);

	cx18_writel(cx, IRQ_CPU_TO_HPU,     &cx->scb->cpu2hpu_irq);
	cx18_writel(cx, IRQ_HPU_TO_CPU_ACK, &cx->scb->hpu2cpu_irq_ack);
	cx18_writel(cx, IRQ_APU_TO_HPU,     &cx->scb->apu2hpu_irq);
	cx18_writel(cx, IRQ_HPU_TO_APU_ACK, &cx->scb->hpu2apu_irq_ack);
	cx18_writel(cx, IRQ_PPU_TO_HPU,     &cx->scb->ppu2hpu_irq);
	cx18_writel(cx, IRQ_HPU_TO_PPU_ACK, &cx->scb->hpu2ppu_irq_ack);
	cx18_writel(cx, IRQ_EPU_TO_HPU,     &cx->scb->epu2hpu_irq);
	cx18_writel(cx, IRQ_HPU_TO_EPU_ACK, &cx->scb->hpu2epu_irq_ack);

	cx18_writel(cx, IRQ_CPU_TO_PPU,     &cx->scb->cpu2ppu_irq);
	cx18_writel(cx, IRQ_PPU_TO_CPU_ACK, &cx->scb->ppu2cpu_irq_ack);
	cx18_writel(cx, IRQ_APU_TO_PPU,     &cx->scb->apu2ppu_irq);
	cx18_writel(cx, IRQ_PPU_TO_APU_ACK, &cx->scb->ppu2apu_irq_ack);
	cx18_writel(cx, IRQ_HPU_TO_PPU,     &cx->scb->hpu2ppu_irq);
	cx18_writel(cx, IRQ_PPU_TO_HPU_ACK, &cx->scb->ppu2hpu_irq_ack);
	cx18_writel(cx, IRQ_EPU_TO_PPU,     &cx->scb->epu2ppu_irq);
	cx18_writel(cx, IRQ_PPU_TO_EPU_ACK, &cx->scb->ppu2epu_irq_ack);

	cx18_writel(cx, IRQ_CPU_TO_EPU,     &cx->scb->cpu2epu_irq);
	cx18_writel(cx, IRQ_EPU_TO_CPU_ACK, &cx->scb->epu2cpu_irq_ack);
	cx18_writel(cx, IRQ_APU_TO_EPU,     &cx->scb->apu2epu_irq);
	cx18_writel(cx, IRQ_EPU_TO_APU_ACK, &cx->scb->epu2apu_irq_ack);
	cx18_writel(cx, IRQ_HPU_TO_EPU,     &cx->scb->hpu2epu_irq);
	cx18_writel(cx, IRQ_EPU_TO_HPU_ACK, &cx->scb->epu2hpu_irq_ack);
	cx18_writel(cx, IRQ_PPU_TO_EPU,     &cx->scb->ppu2epu_irq);
	cx18_writel(cx, IRQ_EPU_TO_PPU_ACK, &cx->scb->epu2ppu_irq_ack);

	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, apu2cpu_mb),
			&cx->scb->apu2cpu_mb_offset);
	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, hpu2cpu_mb),
			&cx->scb->hpu2cpu_mb_offset);
	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, ppu2cpu_mb),
			&cx->scb->ppu2cpu_mb_offset);
	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, epu2cpu_mb),
			&cx->scb->epu2cpu_mb_offset);
	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, cpu2apu_mb),
			&cx->scb->cpu2apu_mb_offset);
	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, hpu2apu_mb),
			&cx->scb->hpu2apu_mb_offset);
	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, ppu2apu_mb),
			&cx->scb->ppu2apu_mb_offset);
	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, epu2apu_mb),
			&cx->scb->epu2apu_mb_offset);
	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, cpu2hpu_mb),
			&cx->scb->cpu2hpu_mb_offset);
	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, apu2hpu_mb),
			&cx->scb->apu2hpu_mb_offset);
	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, ppu2hpu_mb),
			&cx->scb->ppu2hpu_mb_offset);
	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, epu2hpu_mb),
			&cx->scb->epu2hpu_mb_offset);
	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, cpu2ppu_mb),
			&cx->scb->cpu2ppu_mb_offset);
	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, apu2ppu_mb),
			&cx->scb->apu2ppu_mb_offset);
	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, hpu2ppu_mb),
			&cx->scb->hpu2ppu_mb_offset);
	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, epu2ppu_mb),
			&cx->scb->epu2ppu_mb_offset);
	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, cpu2epu_mb),
			&cx->scb->cpu2epu_mb_offset);
	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, apu2epu_mb),
			&cx->scb->apu2epu_mb_offset);
	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, hpu2epu_mb),
			&cx->scb->hpu2epu_mb_offset);
	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, ppu2epu_mb),
			&cx->scb->ppu2epu_mb_offset);

	cx18_writel(cx, SCB_OFFSET + offsetof(struct cx18_scb, cpu_state),
			&cx->scb->ipc_offset);

	cx18_writel(cx, 1, &cx->scb->epu_state);
}
