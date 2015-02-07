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

#ifndef __IP3106_UART_H
#define __IP3106_UART_H

#include <int.h>

/* early macros for kgdb use. fixme: clean this up */

#define UART_BASE		0xbbe4a000	/* PNX8550 */

#define PNX8550_UART_PORT0	(UART_BASE)
#define PNX8550_UART_PORT1	(UART_BASE + 0x1000)

#define PNX8550_UART_INT(x)		(PNX8550_INT_GIC_MIN+19+x)
#define IRQ_TO_UART(x)			(x-PNX8550_INT_GIC_MIN-19)

/* early macros needed for prom/kgdb */

#define ip3106_lcr(base, port)    *(volatile u32 *)(base+(port*0x1000) + 0x000)
#define ip3106_mcr(base, port)   *(volatile u32 *)(base+(port*0x1000) + 0x004)
#define ip3106_baud(base, port)  *(volatile u32 *)(base+(port*0x1000) + 0x008)
#define ip3106_cfg(base, port)   *(volatile u32 *)(base+(port*0x1000) + 0x00C)
#define ip3106_fifo(base, port)	 *(volatile u32 *)(base+(port*0x1000) + 0x028)
#define ip3106_istat(base, port) *(volatile u32 *)(base+(port*0x1000) + 0xFE0)
#define ip3106_ien(base, port)   *(volatile u32 *)(base+(port*0x1000) + 0xFE4)
#define ip3106_iclr(base, port)  *(volatile u32 *)(base+(port*0x1000) + 0xFE8)
#define ip3106_iset(base, port)  *(volatile u32 *)(base+(port*0x1000) + 0xFEC)
#define ip3106_pd(base, port)    *(volatile u32 *)(base+(port*0x1000) + 0xFF4)
#define ip3106_mid(base, port)   *(volatile u32 *)(base+(port*0x1000) + 0xFFC)

#endif
