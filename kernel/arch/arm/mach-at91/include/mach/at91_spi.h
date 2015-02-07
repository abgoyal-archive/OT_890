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
 * arch/arm/mach-at91/include/mach/at91_spi.h
 *
 * Copyright (C) 2005 Ivan Kokshaysky
 * Copyright (C) SAN People
 *
 * Serial Peripheral Interface (SPI) registers.
 * Based on AT91RM9200 datasheet revision E.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef AT91_SPI_H
#define AT91_SPI_H

#define AT91_SPI_CR			0x00		/* Control Register */
#define		AT91_SPI_SPIEN		(1 <<  0)		/* SPI Enable */
#define		AT91_SPI_SPIDIS		(1 <<  1)		/* SPI Disable */
#define		AT91_SPI_SWRST		(1 <<  7)		/* SPI Software Reset */
#define		AT91_SPI_LASTXFER	(1 << 24)		/* Last Transfer [SAM9261 only] */

#define AT91_SPI_MR			0x04		/* Mode Register */
#define		AT91_SPI_MSTR		(1    <<  0)		/* Master/Slave Mode */
#define		AT91_SPI_PS		(1    <<  1)		/* Peripheral Select */
#define			AT91_SPI_PS_FIXED	(0 << 1)
#define			AT91_SPI_PS_VARIABLE	(1 << 1)
#define		AT91_SPI_PCSDEC		(1    <<  2)		/* Chip Select Decode */
#define		AT91_SPI_DIV32		(1    <<  3)		/* Clock Selection [AT91RM9200 only] */
#define		AT91_SPI_MODFDIS	(1    <<  4)		/* Mode Fault Detection */
#define		AT91_SPI_LLB		(1    <<  7)		/* Local Loopback Enable */
#define		AT91_SPI_PCS		(0xf  << 16)		/* Peripheral Chip Select */
#define		AT91_SPI_DLYBCS		(0xff << 24)		/* Delay Between Chip Selects */

#define AT91_SPI_RDR		0x08			/* Receive Data Register */
#define		AT91_SPI_RD		(0xffff <<  0)		/* Receive Data */
#define		AT91_SPI_PCS		(0xf	<< 16)		/* Peripheral Chip Select */

#define AT91_SPI_TDR		0x0c			/* Transmit Data Register */
#define		AT91_SPI_TD		(0xffff <<  0)		/* Transmit Data */
#define		AT91_SPI_PCS		(0xf	<< 16)		/* Peripheral Chip Select */
#define		AT91_SPI_LASTXFER	(1	<< 24)		/* Last Transfer [SAM9261 only] */

#define AT91_SPI_SR		0x10			/* Status Register */
#define		AT91_SPI_RDRF		(1 <<  0)		/* Receive Data Register Full */
#define		AT91_SPI_TDRE		(1 <<  1)		/* Transmit Data Register Full */
#define		AT91_SPI_MODF		(1 <<  2)		/* Mode Fault Error */
#define		AT91_SPI_OVRES		(1 <<  3)		/* Overrun Error Status */
#define		AT91_SPI_ENDRX		(1 <<  4)		/* End of RX buffer */
#define		AT91_SPI_ENDTX		(1 <<  5)		/* End of TX buffer */
#define		AT91_SPI_RXBUFF		(1 <<  6)		/* RX Buffer Full */
#define		AT91_SPI_TXBUFE		(1 <<  7)		/* TX Buffer Empty */
#define		AT91_SPI_NSSR		(1 <<  8)		/* NSS Rising [SAM9261 only] */
#define		AT91_SPI_TXEMPTY	(1 <<  9)		/* Transmission Register Empty [SAM9261 only] */
#define		AT91_SPI_SPIENS		(1 << 16)		/* SPI Enable Status */

#define AT91_SPI_IER		0x14			/* Interrupt Enable Register */
#define AT91_SPI_IDR		0x18			/* Interrupt Disable Register */
#define AT91_SPI_IMR		0x1c			/* Interrupt Mask Register */

#define AT91_SPI_CSR(n)		(0x30 + ((n) * 4))	/* Chip Select Registers 0-3 */
#define		AT91_SPI_CPOL		(1    <<  0)		/* Clock Polarity */
#define		AT91_SPI_NCPHA		(1    <<  1)		/* Clock Phase */
#define		AT91_SPI_CSAAT		(1    <<  3)		/* Chip Select Active After Transfer [SAM9261 only] */
#define		AT91_SPI_BITS		(0xf  <<  4)		/* Bits Per Transfer */
#define			AT91_SPI_BITS_8		(0 << 4)
#define			AT91_SPI_BITS_9		(1 << 4)
#define			AT91_SPI_BITS_10	(2 << 4)
#define			AT91_SPI_BITS_11	(3 << 4)
#define			AT91_SPI_BITS_12	(4 << 4)
#define			AT91_SPI_BITS_13	(5 << 4)
#define			AT91_SPI_BITS_14	(6 << 4)
#define			AT91_SPI_BITS_15	(7 << 4)
#define			AT91_SPI_BITS_16	(8 << 4)
#define		AT91_SPI_SCBR		(0xff <<  8)		/* Serial Clock Baud Rate */
#define		AT91_SPI_DLYBS		(0xff << 16)		/* Delay before SPCK */
#define		AT91_SPI_DLYBCT		(0xff << 24)		/* Delay between Consecutive Transfers */

#endif
