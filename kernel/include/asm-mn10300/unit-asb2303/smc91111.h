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

/* Support for the SMC91C111 NIC on an ASB2303
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */
#ifndef _ASM_UNIT_SMC91111_H
#define _ASM_UNIT_SMC91111_H

#include <asm/intctl-regs.h>

#define SMC91111_BASE		0xAA000300UL
#define SMC91111_BASE_END	0xAA000400UL
#define SMC91111_IRQ		XIRQ3

#define SMC_CAN_USE_8BIT	0
#define SMC_CAN_USE_16BIT	1
#define SMC_CAN_USE_32BIT	0
#define SMC_NOWAIT		1
#define SMC_IRQ_FLAGS		(0)

#if SMC_CAN_USE_8BIT
#define SMC_inb(a, r)		inb((unsigned long) ((a) + (r)))
#define SMC_outb(v, a, r)	outb(v, (unsigned long) ((a) + (r)))
#endif

#if SMC_CAN_USE_16BIT
#define SMC_inw(a, r)		inw((unsigned long) ((a) + (r)))
#define SMC_outw(v, a, r)	outw(v, (unsigned long) ((a) + (r)))
#define SMC_insw(a, r, p, l)	insw((unsigned long) ((a) + (r)), (p), (l))
#define SMC_outsw(a, r, p, l)	outsw((unsigned long) ((a) + (r)), (p), (l))
#endif

#if SMC_CAN_USE_32BIT
#define SMC_inl(a, r)		inl((unsigned long) ((a) + (r)))
#define SMC_outl(v, a, r)	outl(v, (unsigned long) ((a) + (r)))
#define SMC_insl(a, r, p, l)	insl((unsigned long) ((a) + (r)), (p), (l))
#define SMC_outsl(a, r, p, l)	outsl((unsigned long) ((a) + (r)), (p), (l))
#endif

#define RPC_LSA_DEFAULT		RPC_LED_100_10
#define RPC_LSB_DEFAULT		RPC_LED_TX_RX

#define set_irq_type(irq, type)

#endif /*  _ASM_UNIT_SMC91111_H */
