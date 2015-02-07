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
 * IBM ASM Service Processor Device Driver
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Copyright (C) IBM Corporation, 2004
 *
 * Author: Max Asböck <amax@us.ibm.com>
 *
 */

/* Condor service processor specific hardware definitions */

#ifndef __IBMASM_CONDOR_H__
#define __IBMASM_CONDOR_H__

#include <asm/io.h>

#define VENDORID_IBM	0x1014
#define DEVICEID_RSA	0x010F

#define GET_MFA_ADDR(x)  (x & 0xFFFFFF00)

#define MAILBOX_FULL(x)  (x & 0x00000001)

#define NO_MFAS_AVAILABLE     0xFFFFFFFF


#define INBOUND_QUEUE_PORT   0x40  /* contains address of next free MFA */
#define OUTBOUND_QUEUE_PORT  0x44  /* contains address of posted MFA    */

#define SP_INTR_MASK	0x00000008
#define UART_INTR_MASK	0x00000010

#define INTR_STATUS_REGISTER   0x13A0
#define INTR_CONTROL_REGISTER  0x13A4

#define SCOUT_COM_A_BASE         0x0000
#define SCOUT_COM_B_BASE         0x0100
#define SCOUT_COM_C_BASE         0x0200
#define SCOUT_COM_D_BASE         0x0300

static inline int sp_interrupt_pending(void __iomem *base_address)
{
	return SP_INTR_MASK & readl(base_address + INTR_STATUS_REGISTER);
}

static inline int uart_interrupt_pending(void __iomem *base_address)
{
	return UART_INTR_MASK & readl(base_address + INTR_STATUS_REGISTER);
}

static inline void ibmasm_enable_interrupts(void __iomem *base_address, int mask)
{
	void __iomem *ctrl_reg = base_address + INTR_CONTROL_REGISTER;
	writel( readl(ctrl_reg) & ~mask, ctrl_reg);
}

static inline void ibmasm_disable_interrupts(void __iomem *base_address, int mask)
{
	void __iomem *ctrl_reg = base_address + INTR_CONTROL_REGISTER;
	writel( readl(ctrl_reg) | mask, ctrl_reg);
}

static inline void enable_sp_interrupts(void __iomem *base_address)
{
	ibmasm_enable_interrupts(base_address, SP_INTR_MASK);
}

static inline void disable_sp_interrupts(void __iomem *base_address)
{
	ibmasm_disable_interrupts(base_address, SP_INTR_MASK);
}

static inline void enable_uart_interrupts(void __iomem *base_address)
{
	ibmasm_enable_interrupts(base_address, UART_INTR_MASK);
}

static inline void disable_uart_interrupts(void __iomem *base_address)
{
	ibmasm_disable_interrupts(base_address, UART_INTR_MASK);
}

#define valid_mfa(mfa)	( (mfa) != NO_MFAS_AVAILABLE )

static inline u32 get_mfa_outbound(void __iomem *base_address)
{
	int retry;
	u32 mfa;

	for (retry=0; retry<=10; retry++) {
		mfa = readl(base_address + OUTBOUND_QUEUE_PORT);
		if (valid_mfa(mfa))
			break;
	}
	return mfa;
}

static inline void set_mfa_outbound(void __iomem *base_address, u32 mfa)
{
	writel(mfa, base_address + OUTBOUND_QUEUE_PORT);
}

static inline u32 get_mfa_inbound(void __iomem *base_address)
{
	u32 mfa = readl(base_address + INBOUND_QUEUE_PORT);

	if (MAILBOX_FULL(mfa))
		return 0;

	return mfa;
}

static inline void set_mfa_inbound(void __iomem *base_address, u32 mfa)
{
	writel(mfa, base_address + INBOUND_QUEUE_PORT);
}

static inline struct i2o_message *get_i2o_message(void __iomem *base_address, u32 mfa)
{
	return (struct i2o_message *)(GET_MFA_ADDR(mfa) + base_address);
}

#endif /* __IBMASM_CONDOR_H__ */
