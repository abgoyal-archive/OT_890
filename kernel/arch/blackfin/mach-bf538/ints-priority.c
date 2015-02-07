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
 * File:         arch/blackfin/mach-bf538/ints-priority.c
 * Based on:     arch/blackfin/mach-bf533/ints-priority.c
 * Author:       Michael Hennerich
 *
 * Created:
 * Description:  Set up the interrupt priorities
 *
 * Modified:
 *               Copyright 2008 Analog Devices Inc.
 *
 * Bugs:         Enter bugs at http://blackfin.uclinux.org/
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
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <linux/module.h>
#include <linux/irq.h>
#include <asm/blackfin.h>

void __init program_IAR(void)
{

	/* Program the IAR0 Register with the configured priority */
	bfin_write_SIC_IAR0(((CONFIG_IRQ_PLL_WAKEUP - 7) << IRQ_PLL_WAKEUP_POS) |
			((CONFIG_IRQ_DMA0_ERROR - 7) << IRQ_DMA0_ERROR_POS) |
			((CONFIG_IRQ_PPI_ERROR - 7) << IRQ_PPI_ERROR_POS) |
			((CONFIG_IRQ_SPORT0_ERROR - 7) << IRQ_SPORT0_ERROR_POS) |
			((CONFIG_IRQ_SPORT1_ERROR - 7) << IRQ_SPORT1_ERROR_POS) |
			((CONFIG_IRQ_SPI0_ERROR - 7) << IRQ_SPI0_ERROR_POS) |
			((CONFIG_IRQ_UART0_ERROR - 7) << IRQ_UART0_ERROR_POS) |
			((CONFIG_IRQ_RTC - 7) << IRQ_RTC_POS));

	bfin_write_SIC_IAR1(((CONFIG_IRQ_PPI - 7) << IRQ_PPI_POS) |
			((CONFIG_IRQ_SPORT0_RX - 7) << IRQ_SPORT0_RX_POS) |
			((CONFIG_IRQ_SPORT0_TX - 7) << IRQ_SPORT0_TX_POS) |
			((CONFIG_IRQ_SPORT1_RX - 7) << IRQ_SPORT1_RX_POS) |
			((CONFIG_IRQ_SPORT1_TX - 7) << IRQ_SPORT1_TX_POS) |
			((CONFIG_IRQ_SPI0 - 7) << IRQ_SPI0_POS) |
			((CONFIG_IRQ_UART0_RX - 7) << IRQ_UART0_RX_POS) |
			((CONFIG_IRQ_UART0_TX - 7) << IRQ_UART0_TX_POS));

	bfin_write_SIC_IAR2(((CONFIG_IRQ_TIMER0 - 7) << IRQ_TIMER0_POS) |
			((CONFIG_IRQ_TIMER1 - 7) << IRQ_TIMER1_POS) |
			((CONFIG_IRQ_TIMER2 - 7) << IRQ_TIMER2_POS) |
			((CONFIG_IRQ_PORTF_INTA - 7) << IRQ_PORTF_INTA_POS) |
			((CONFIG_IRQ_PORTF_INTB - 7) << IRQ_PORTF_INTB_POS) |
			((CONFIG_IRQ_MEM0_DMA0 - 7) << IRQ_MEM0_DMA0_POS) |
			((CONFIG_IRQ_MEM0_DMA1 - 7) << IRQ_MEM0_DMA1_POS) |
			((CONFIG_IRQ_WATCH - 7) << IRQ_WATCH_POS));

	bfin_write_SIC_IAR3(((CONFIG_IRQ_DMA1_ERROR - 7) << IRQ_DMA1_ERROR_POS) |
			((CONFIG_IRQ_SPORT2_ERROR - 7) << IRQ_SPORT2_ERROR_POS) |
			((CONFIG_IRQ_SPORT3_ERROR - 7) << IRQ_SPORT3_ERROR_POS) |
			((CONFIG_IRQ_SPI1_ERROR - 7) << IRQ_SPI1_ERROR_POS) |
			((CONFIG_IRQ_SPI2_ERROR - 7) << IRQ_SPI2_ERROR_POS) |
			((CONFIG_IRQ_UART1_ERROR - 7) << IRQ_UART1_ERROR_POS) |
			((CONFIG_IRQ_UART2_ERROR - 7) << IRQ_UART2_ERROR_POS));

	bfin_write_SIC_IAR4(((CONFIG_IRQ_CAN_ERROR - 7) << IRQ_CAN_ERROR_POS) |
			((CONFIG_IRQ_SPORT2_RX - 7) << IRQ_SPORT2_RX_POS) |
			((CONFIG_IRQ_SPORT2_TX - 7) << IRQ_SPORT2_TX_POS) |
			((CONFIG_IRQ_SPORT3_RX - 7) << IRQ_SPORT3_RX_POS) |
			((CONFIG_IRQ_SPORT3_TX - 7) << IRQ_SPORT3_TX_POS) |
			((CONFIG_IRQ_SPI1 - 7) << IRQ_SPI1_POS));

	bfin_write_SIC_IAR5(((CONFIG_IRQ_SPI2 - 7) << IRQ_SPI2_POS) |
			((CONFIG_IRQ_UART1_RX - 7) << IRQ_UART1_RX_POS) |
			((CONFIG_IRQ_UART1_TX - 7) << IRQ_UART1_TX_POS) |
			((CONFIG_IRQ_UART2_RX - 7) << IRQ_UART2_RX_POS) |
			((CONFIG_IRQ_UART2_TX - 7) << IRQ_UART2_TX_POS) |
			((CONFIG_IRQ_TWI0 - 7) << IRQ_TWI0_POS) |
			((CONFIG_IRQ_TWI1 - 7) << IRQ_TWI1_POS) |
			((CONFIG_IRQ_CAN_RX - 7) << IRQ_CAN_RX_POS));

	bfin_write_SIC_IAR6(((CONFIG_IRQ_CAN_TX - 7) << IRQ_CAN_TX_POS) |
			((CONFIG_IRQ_MEM1_DMA0 - 7) << IRQ_MEM1_DMA0_POS) |
			((CONFIG_IRQ_MEM1_DMA1 - 7) << IRQ_MEM1_DMA1_POS));

	SSYNC();
}
