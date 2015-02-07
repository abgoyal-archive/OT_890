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

#ifndef CX18_SCB_H
#define CX18_SCB_H

#include "cx18-mailbox.h"

/* NOTE: All ACK interrupts are in the SW2 register.  All non-ACK interrupts
   are in the SW1 register. */

#define IRQ_APU_TO_CPU         0x00000001
#define IRQ_CPU_TO_APU_ACK     0x00000001
#define IRQ_HPU_TO_CPU         0x00000002
#define IRQ_CPU_TO_HPU_ACK     0x00000002
#define IRQ_PPU_TO_CPU         0x00000004
#define IRQ_CPU_TO_PPU_ACK     0x00000004
#define IRQ_EPU_TO_CPU         0x00000008
#define IRQ_CPU_TO_EPU_ACK     0x00000008

#define IRQ_CPU_TO_APU         0x00000010
#define IRQ_APU_TO_CPU_ACK     0x00000010
#define IRQ_HPU_TO_APU         0x00000020
#define IRQ_APU_TO_HPU_ACK     0x00000020
#define IRQ_PPU_TO_APU         0x00000040
#define IRQ_APU_TO_PPU_ACK     0x00000040
#define IRQ_EPU_TO_APU         0x00000080
#define IRQ_APU_TO_EPU_ACK     0x00000080

#define IRQ_CPU_TO_HPU         0x00000100
#define IRQ_HPU_TO_CPU_ACK     0x00000100
#define IRQ_APU_TO_HPU         0x00000200
#define IRQ_HPU_TO_APU_ACK     0x00000200
#define IRQ_PPU_TO_HPU         0x00000400
#define IRQ_HPU_TO_PPU_ACK     0x00000400
#define IRQ_EPU_TO_HPU         0x00000800
#define IRQ_HPU_TO_EPU_ACK     0x00000800

#define IRQ_CPU_TO_PPU         0x00001000
#define IRQ_PPU_TO_CPU_ACK     0x00001000
#define IRQ_APU_TO_PPU         0x00002000
#define IRQ_PPU_TO_APU_ACK     0x00002000
#define IRQ_HPU_TO_PPU         0x00004000
#define IRQ_PPU_TO_HPU_ACK     0x00004000
#define IRQ_EPU_TO_PPU         0x00008000
#define IRQ_PPU_TO_EPU_ACK     0x00008000

#define IRQ_CPU_TO_EPU         0x00010000
#define IRQ_EPU_TO_CPU_ACK     0x00010000
#define IRQ_APU_TO_EPU         0x00020000
#define IRQ_EPU_TO_APU_ACK     0x00020000
#define IRQ_HPU_TO_EPU         0x00040000
#define IRQ_EPU_TO_HPU_ACK     0x00040000
#define IRQ_PPU_TO_EPU         0x00080000
#define IRQ_EPU_TO_PPU_ACK     0x00080000

#define SCB_OFFSET  0xDC0000

/* If Firmware uses fixed memory map, it shall not allocate the area
   between SCB_OFFSET and SCB_OFFSET+SCB_RESERVED_SIZE-1 inclusive */
#define SCB_RESERVED_SIZE 0x10000


/* This structure is used by EPU to provide memory descriptors in its memory */
struct cx18_mdl {
    u32 paddr;  /* Physical address of a buffer segment */
    u32 length; /* Length of the buffer segment */
};

struct cx18_scb {
	/* These fields form the System Control Block which is used at boot time
	   for localizing the IPC data as well as the code positions for all
	   processors. The offsets are from the start of this struct. */

	/* Offset where to find the Inter-Processor Communication data */
	u32 ipc_offset;
	u32 reserved01[7];
	/* Offset where to find the start of the CPU code */
	u32 cpu_code_offset;
	u32 reserved02[3];
	/* Offset where to find the start of the APU code */
	u32 apu_code_offset;
	u32 reserved03[3];
	/* Offset where to find the start of the HPU code */
	u32 hpu_code_offset;
	u32 reserved04[3];
	/* Offset where to find the start of the PPU code */
	u32 ppu_code_offset;
	u32 reserved05[3];

	/* These fields form Inter-Processor Communication data which is used
	   by all processors to locate the information needed for communicating
	   with other processors */

	/* Fields for CPU: */

	/* bit 0: 1/0 processor ready/not ready. Set other bits to 0. */
	u32 cpu_state;
	u32 reserved1[7];
	/* Offset to the mailbox used for sending commands from APU to CPU */
	u32 apu2cpu_mb_offset;
	/* Value to write to register SW1 register set (0xC7003100) after the
	   command is ready */
	u32 apu2cpu_irq;
	/* Value to write to register SW2 register set (0xC7003140) after the
	   command is cleared */
	u32 cpu2apu_irq_ack;
	u32 reserved2[13];

	u32 hpu2cpu_mb_offset;
	u32 hpu2cpu_irq;
	u32 cpu2hpu_irq_ack;
	u32 reserved3[13];

	u32 ppu2cpu_mb_offset;
	u32 ppu2cpu_irq;
	u32 cpu2ppu_irq_ack;
	u32 reserved4[13];

	u32 epu2cpu_mb_offset;
	u32 epu2cpu_irq;
	u32 cpu2epu_irq_ack;
	u32 reserved5[13];
	u32 reserved6[8];

	/* Fields for APU: */

	u32 apu_state;
	u32 reserved11[7];
	u32 cpu2apu_mb_offset;
	u32 cpu2apu_irq;
	u32 apu2cpu_irq_ack;
	u32 reserved12[13];

	u32 hpu2apu_mb_offset;
	u32 hpu2apu_irq;
	u32 apu2hpu_irq_ack;
	u32 reserved13[13];

	u32 ppu2apu_mb_offset;
	u32 ppu2apu_irq;
	u32 apu2ppu_irq_ack;
	u32 reserved14[13];

	u32 epu2apu_mb_offset;
	u32 epu2apu_irq;
	u32 apu2epu_irq_ack;
	u32 reserved15[13];
	u32 reserved16[8];

	/* Fields for HPU: */

	u32 hpu_state;
	u32 reserved21[7];
	u32 cpu2hpu_mb_offset;
	u32 cpu2hpu_irq;
	u32 hpu2cpu_irq_ack;
	u32 reserved22[13];

	u32 apu2hpu_mb_offset;
	u32 apu2hpu_irq;
	u32 hpu2apu_irq_ack;
	u32 reserved23[13];

	u32 ppu2hpu_mb_offset;
	u32 ppu2hpu_irq;
	u32 hpu2ppu_irq_ack;
	u32 reserved24[13];

	u32 epu2hpu_mb_offset;
	u32 epu2hpu_irq;
	u32 hpu2epu_irq_ack;
	u32 reserved25[13];
	u32 reserved26[8];

	/* Fields for PPU: */

	u32 ppu_state;
	u32 reserved31[7];
	u32 cpu2ppu_mb_offset;
	u32 cpu2ppu_irq;
	u32 ppu2cpu_irq_ack;
	u32 reserved32[13];

	u32 apu2ppu_mb_offset;
	u32 apu2ppu_irq;
	u32 ppu2apu_irq_ack;
	u32 reserved33[13];

	u32 hpu2ppu_mb_offset;
	u32 hpu2ppu_irq;
	u32 ppu2hpu_irq_ack;
	u32 reserved34[13];

	u32 epu2ppu_mb_offset;
	u32 epu2ppu_irq;
	u32 ppu2epu_irq_ack;
	u32 reserved35[13];
	u32 reserved36[8];

	/* Fields for EPU: */

	u32 epu_state;
	u32 reserved41[7];
	u32 cpu2epu_mb_offset;
	u32 cpu2epu_irq;
	u32 epu2cpu_irq_ack;
	u32 reserved42[13];

	u32 apu2epu_mb_offset;
	u32 apu2epu_irq;
	u32 epu2apu_irq_ack;
	u32 reserved43[13];

	u32 hpu2epu_mb_offset;
	u32 hpu2epu_irq;
	u32 epu2hpu_irq_ack;
	u32 reserved44[13];

	u32 ppu2epu_mb_offset;
	u32 ppu2epu_irq;
	u32 epu2ppu_irq_ack;
	u32 reserved45[13];
	u32 reserved46[8];

	u32 semaphores[8];  /* Semaphores */

	u32 reserved50[32]; /* Reserved for future use */

	struct cx18_mailbox  apu2cpu_mb;
	struct cx18_mailbox  hpu2cpu_mb;
	struct cx18_mailbox  ppu2cpu_mb;
	struct cx18_mailbox  epu2cpu_mb;

	struct cx18_mailbox  cpu2apu_mb;
	struct cx18_mailbox  hpu2apu_mb;
	struct cx18_mailbox  ppu2apu_mb;
	struct cx18_mailbox  epu2apu_mb;

	struct cx18_mailbox  cpu2hpu_mb;
	struct cx18_mailbox  apu2hpu_mb;
	struct cx18_mailbox  ppu2hpu_mb;
	struct cx18_mailbox  epu2hpu_mb;

	struct cx18_mailbox  cpu2ppu_mb;
	struct cx18_mailbox  apu2ppu_mb;
	struct cx18_mailbox  hpu2ppu_mb;
	struct cx18_mailbox  epu2ppu_mb;

	struct cx18_mailbox  cpu2epu_mb;
	struct cx18_mailbox  apu2epu_mb;
	struct cx18_mailbox  hpu2epu_mb;
	struct cx18_mailbox  ppu2epu_mb;

	struct cx18_mdl_ack  cpu_mdl_ack[CX18_MAX_STREAMS][CX18_MAX_MDL_ACKS];
	struct cx18_mdl      cpu_mdl[1];
};

void cx18_init_scb(struct cx18 *cx);

#endif