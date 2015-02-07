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
 * arch/arm/mach-l7200/include/mach/serial.h
 *
 * Copyright (c) 2000 Rob Scott (rscott@mtrob.fdns.net)
 *                    Steve Hill (sjhill@cotw.com)
 *
 * Changelog:
 *  03-20-2000  SJH     Created
 *  03-26-2000  SJH     Added flags for serial ports
 *  03-27-2000  SJH     Corrected BASE_BAUD value
 *  04-14-2000  RS      Made register addr dependent on IO_BASE
 *  05-03-2000  SJH     Complete rewrite
 *  05-09-2000	SJH	Stripped out architecture specific serial stuff
 *                      and placed it in a separate file
 *  07-28-2000	SJH	Moved base baud rate variable
 */
#ifndef __ASM_ARCH_SERIAL_H
#define __ASM_ARCH_SERIAL_H

/*
 * This assumes you have a 3.6864 MHz clock for your UART.
 */
#define BASE_BAUD	3686400

/*
 * Standard COM flags
 */
#define STD_COM_FLAGS (ASYNC_BOOT_AUTOCONF | ASYNC_SKIP_TEST)

#define STD_SERIAL_PORT_DEFNS		\
	/* MAGIC UART CLK   PORT       IRQ     FLAGS */			\
	{ 0, BASE_BAUD, UART1_BASE, IRQ_UART_1, STD_COM_FLAGS },  /* ttyLU0 */ \
	{ 0, BASE_BAUD, UART2_BASE, IRQ_UART_2, STD_COM_FLAGS },  /* ttyLU1 */ \

#define EXTRA_SERIAL_PORT_DEFNS

#endif
