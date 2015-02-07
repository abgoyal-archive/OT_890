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
 * arch/arm/mach-netx/include/mach/irqs.h
 *
 * Copyright (C) 2005 Sascha Hauer <s.hauer@pengutronix.de>, Pengutronix
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#define NETX_IRQ_VIC_START   0
#define NETX_IRQ_SOFTINT     0
#define NETX_IRQ_TIMER0      1
#define NETX_IRQ_TIMER1      2
#define NETX_IRQ_TIMER2      3
#define NETX_IRQ_SYSTIME_NS  4
#define NETX_IRQ_SYSTIME_S   5
#define NETX_IRQ_GPIO_15     6
#define NETX_IRQ_WATCHDOG    7
#define NETX_IRQ_UART0       8
#define NETX_IRQ_UART1       9
#define NETX_IRQ_UART2      10
#define NETX_IRQ_USB        11
#define NETX_IRQ_SPI        12
#define NETX_IRQ_I2C        13
#define NETX_IRQ_LCD        14
#define NETX_IRQ_HIF        15
#define NETX_IRQ_GPIO_0_14  16
#define NETX_IRQ_XPEC0      17
#define NETX_IRQ_XPEC1      18
#define NETX_IRQ_XPEC2      19
#define NETX_IRQ_XPEC3      20
#define NETX_IRQ_XPEC(no)   (17 + (no))
#define NETX_IRQ_MSYNC0     21
#define NETX_IRQ_MSYNC1     22
#define NETX_IRQ_MSYNC2     23
#define NETX_IRQ_MSYNC3     24
#define NETX_IRQ_IRQ_PHY    25
#define NETX_IRQ_ISO_AREA   26
/* int 27 is reserved */
/* int 28 is reserved */
#define NETX_IRQ_TIMER3     29
#define NETX_IRQ_TIMER4     30
/* int 31 is reserved */

#define NETX_IRQS 32

/* for multiplexed irqs on gpio 0..14 */
#define NETX_IRQ_GPIO(x) (NETX_IRQS + (x))
#define NETX_IRQ_GPIO_LAST NETX_IRQ_GPIO(14)

/* Host interface interrupts */
#define NETX_IRQ_HIF_CHAINED(x)    (NETX_IRQ_GPIO_LAST + 1 + (x))
#define NETX_IRQ_HIF_PIO35         NETX_IRQ_HIF_CHAINED(0)
#define NETX_IRQ_HIF_PIO36         NETX_IRQ_HIF_CHAINED(1)
#define NETX_IRQ_HIF_PIO40         NETX_IRQ_HIF_CHAINED(2)
#define NETX_IRQ_HIF_PIO47         NETX_IRQ_HIF_CHAINED(3)
#define NETX_IRQ_HIF_PIO72         NETX_IRQ_HIF_CHAINED(4)
#define NETX_IRQ_HIF_LAST          NETX_IRQ_HIF_CHAINED(4)

#define NR_IRQS (NETX_IRQ_HIF_LAST + 1)
