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

/* serial-regs.h: serial port registers
 *
 * Copyright (C) 2003 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#ifndef _ASM_SERIAL_REGS_H
#define _ASM_SERIAL_REGS_H

#include <linux/serial_reg.h>
#include <asm/irc-regs.h>

#define SERIAL_ICLK	33333333	/* the target serial input clock */
#define UART0_BASE	0xfeff9c00
#define UART1_BASE	0xfeff9c40

#define __get_UART0(R) ({ __reg(UART0_BASE + (R) * 8) >> 24; })
#define __get_UART1(R) ({ __reg(UART1_BASE + (R) * 8) >> 24; })
#define __set_UART0(R,V) do { __reg(UART0_BASE + (R) * 8) = (V) << 24; } while(0)
#define __set_UART1(R,V) do { __reg(UART1_BASE + (R) * 8) = (V) << 24; } while(0)

#define __get_UART0_LSR() ({ __get_UART0(UART_LSR); })
#define __get_UART1_LSR() ({ __get_UART1(UART_LSR); })

#define __set_UART0_IER(V) __set_UART0(UART_IER,(V))
#define __set_UART1_IER(V) __set_UART1(UART_IER,(V))

/* serial prescaler select register */
#define __get_UCPSR()	({ *(volatile unsigned long *)(0xfeff9c90); })
#define __set_UCPSR(V)	do { *(volatile unsigned long *)(0xfeff9c90) = (V); } while(0)
#define UCPSR_SELECT0	0x07000000
#define UCPSR_SELECT1	0x38000000

/* serial prescaler base value register */
#define __get_UCPVR()	({ *(volatile unsigned long *)(0xfeff9c98); mb(); })
#define __set_UCPVR(V)	do { *(volatile unsigned long *)(0xfeff9c98) = (V) << 24; mb(); } while(0)


#endif /* _ASM_SERIAL_REGS_H */
