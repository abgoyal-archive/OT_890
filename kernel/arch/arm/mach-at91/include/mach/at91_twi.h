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
 * arch/arm/mach-at91/include/mach/at91_twi.h
 *
 * Copyright (C) 2005 Ivan Kokshaysky
 * Copyright (C) SAN People
 *
 * Two-wire Interface (TWI) registers.
 * Based on AT91RM9200 datasheet revision E.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef AT91_TWI_H
#define AT91_TWI_H

#define	AT91_TWI_CR		0x00		/* Control Register */
#define		AT91_TWI_START		(1 <<  0)	/* Send a Start Condition */
#define		AT91_TWI_STOP		(1 <<  1)	/* Send a Stop Condition */
#define		AT91_TWI_MSEN		(1 <<  2)	/* Master Transfer Enable */
#define		AT91_TWI_MSDIS		(1 <<  3)	/* Master Transfer Disable */
#define		AT91_TWI_SVEN		(1 <<  4)	/* Slave Transfer Enable [SAM9260 only] */
#define		AT91_TWI_SVDIS		(1 <<  5)	/* Slave Transfer Disable [SAM9260 only] */
#define		AT91_TWI_SWRST		(1 <<  7)	/* Software Reset */

#define	AT91_TWI_MMR		0x04		/* Master Mode Register */
#define		AT91_TWI_IADRSZ		(3    <<  8)	/* Internal Device Address Size */
#define			AT91_TWI_IADRSZ_NO		(0 << 8)
#define			AT91_TWI_IADRSZ_1		(1 << 8)
#define			AT91_TWI_IADRSZ_2		(2 << 8)
#define			AT91_TWI_IADRSZ_3		(3 << 8)
#define		AT91_TWI_MREAD		(1    << 12)	/* Master Read Direction */
#define		AT91_TWI_DADR		(0x7f << 16)	/* Device Address */

#define	AT91_TWI_SMR		0x08		/* Slave Mode Register [SAM9260 only] */
#define		AT91_TWI_SADR		(0x7f << 16)	/* Slave Address */

#define	AT91_TWI_IADR		0x0c		/* Internal Address Register */

#define	AT91_TWI_CWGR		0x10		/* Clock Waveform Generator Register */
#define		AT91_TWI_CLDIV		(0xff <<  0)	/* Clock Low Divisor */
#define		AT91_TWI_CHDIV		(0xff <<  8)	/* Clock High Divisor */
#define		AT91_TWI_CKDIV		(7    << 16)	/* Clock Divider */

#define	AT91_TWI_SR		0x20		/* Status Register */
#define		AT91_TWI_TXCOMP		(1 <<  0)	/* Transmission Complete */
#define		AT91_TWI_RXRDY		(1 <<  1)	/* Receive Holding Register Ready */
#define		AT91_TWI_TXRDY		(1 <<  2)	/* Transmit Holding Register Ready */
#define		AT91_TWI_SVREAD		(1 <<  3)	/* Slave Read [SAM9260 only] */
#define		AT91_TWI_SVACC		(1 <<  4)	/* Slave Access [SAM9260 only] */
#define		AT91_TWI_GACC		(1 <<  5)	/* General Call Access [SAM9260 only] */
#define		AT91_TWI_OVRE		(1 <<  6)	/* Overrun Error [AT91RM9200 only] */
#define		AT91_TWI_UNRE		(1 <<  7)	/* Underrun Error [AT91RM9200 only] */
#define		AT91_TWI_NACK		(1 <<  8)	/* Not Acknowledged */
#define		AT91_TWI_ARBLST		(1 <<  9)	/* Arbitration Lost [SAM9260 only] */
#define		AT91_TWI_SCLWS		(1 << 10)	/* Clock Wait State [SAM9260 only] */
#define		AT91_TWI_EOSACC		(1 << 11)	/* End of Slave Address [SAM9260 only] */

#define	AT91_TWI_IER		0x24		/* Interrupt Enable Register */
#define	AT91_TWI_IDR		0x28		/* Interrupt Disable Register */
#define	AT91_TWI_IMR		0x2c		/* Interrupt Mask Register */
#define	AT91_TWI_RHR		0x30		/* Receive Holding Register */
#define	AT91_TWI_THR		0x34		/* Transmit Holding Register */

#endif

