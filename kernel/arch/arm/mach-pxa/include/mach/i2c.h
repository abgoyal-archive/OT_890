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
 *  i2c_pxa.h
 *
 *  Copyright (C) 2002 Intrinsyc Software Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 */
#ifndef _I2C_PXA_H_
#define _I2C_PXA_H_

#if 0
#define DEF_TIMEOUT             3
#else
/* need a longer timeout if we're dealing with the fact we may well be
 * looking at a multi-master environment
*/
#define DEF_TIMEOUT             32
#endif

#define BUS_ERROR               (-EREMOTEIO)
#define XFER_NAKED              (-ECONNREFUSED)
#define I2C_RETRY               (-2000) /* an error has occurred retry transmit */

/* ICR initialize bit values
*
*  15. FM       0 (100 Khz operation)
*  14. UR       0 (No unit reset)
*  13. SADIE    0 (Disables the unit from interrupting on slave addresses
*                                       matching its slave address)
*  12. ALDIE    0 (Disables the unit from interrupt when it loses arbitration
*                                       in master mode)
*  11. SSDIE    0 (Disables interrupts from a slave stop detected, in slave mode)
*  10. BEIE     1 (Enable interrupts from detected bus errors, no ACK sent)
*  9.  IRFIE    1 (Enable interrupts from full buffer received)
*  8.  ITEIE    1 (Enables the I2C unit to interrupt when transmit buffer empty)
*  7.  GCD      1 (Disables i2c unit response to general call messages as a slave)
*  6.  IUE      0 (Disable unit until we change settings)
*  5.  SCLE     1 (Enables the i2c clock output for master mode (drives SCL)
*  4.  MA       0 (Only send stop with the ICR stop bit)
*  3.  TB       0 (We are not transmitting a byte initially)
*  2.  ACKNAK   0 (Send an ACK after the unit receives a byte)
*  1.  STOP     0 (Do not send a STOP)
*  0.  START    0 (Do not send a START)
*
*/
#define I2C_ICR_INIT	(ICR_BEIE | ICR_IRFIE | ICR_ITEIE | ICR_GCD | ICR_SCLE)

/* I2C status register init values
 *
 * 10. BED      1 (Clear bus error detected)
 * 9.  SAD      1 (Clear slave address detected)
 * 7.  IRF      1 (Clear IDBR Receive Full)
 * 6.  ITE      1 (Clear IDBR Transmit Empty)
 * 5.  ALD      1 (Clear Arbitration Loss Detected)
 * 4.  SSD      1 (Clear Slave Stop Detected)
 */
#define I2C_ISR_INIT	0x7FF  /* status register init */

struct i2c_slave_client;

struct i2c_pxa_platform_data {
	unsigned int		slave_addr;
	struct i2c_slave_client	*slave;
	unsigned int		class;
	unsigned int		use_pio :1;
	unsigned int		fast_mode :1;
};

extern void pxa_set_i2c_info(struct i2c_pxa_platform_data *info);

#ifdef CONFIG_PXA27x
extern void pxa27x_set_i2c_power_info(struct i2c_pxa_platform_data *info);
#endif

#ifdef CONFIG_PXA3xx
extern void pxa3xx_set_i2c_power_info(struct i2c_pxa_platform_data *info);
#endif

#endif
