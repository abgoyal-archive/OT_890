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
 * arch/arm/mach-at91/include/mach/at91_adc.h
 *
 * Copyright (C) SAN People
 *
 * Analog-to-Digital Converter (ADC) registers.
 * Based on AT91SAM9260 datasheet revision D.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef AT91_ADC_H
#define AT91_ADC_H

#define AT91_ADC_CR		0x00		/* Control Register */
#define		AT91_ADC_SWRST		(1 << 0)	/* Software Reset */
#define		AT91_ADC_START		(1 << 1)	/* Start Conversion */

#define AT91_ADC_MR		0x04		/* Mode Register */
#define		AT91_ADC_TRGEN		(1 << 0)	/* Trigger Enable */
#define		AT91_ADC_TRGSEL		(7 << 1)	/* Trigger Selection */
#define			AT91_ADC_TRGSEL_TC0		(0 << 1)
#define			AT91_ADC_TRGSEL_TC1		(1 << 1)
#define			AT91_ADC_TRGSEL_TC2		(2 << 1)
#define			AT91_ADC_TRGSEL_EXTERNAL	(6 << 1)
#define		AT91_ADC_LOWRES		(1 << 4)	/* Low Resolution */
#define		AT91_ADC_SLEEP		(1 << 5)	/* Sleep Mode */
#define		AT91_ADC_PRESCAL	(0x3f << 8)	/* Prescalar Rate Selection */
#define			AT91_ADC_PRESCAL_(x)	((x) << 8)
#define		AT91_ADC_STARTUP	(0x1f << 16)	/* Startup Up Time */
#define			AT91_ADC_STARTUP_(x)	((x) << 16)
#define		AT91_ADC_SHTIM		(0xf  << 24)	/* Sample & Hold Time */
#define			AT91_ADC_SHTIM_(x)	((x) << 24)

#define AT91_ADC_CHER		0x10		/* Channel Enable Register */
#define AT91_ADC_CHDR		0x14		/* Channel Disable Register */
#define AT91_ADC_CHSR		0x18		/* Channel Status Register */
#define		AT91_ADC_CH(n)		(1 << (n))	/* Channel Number */

#define AT91_ADC_SR		0x1C		/* Status Register */
#define		AT91_ADC_EOC(n)		(1 << (n))	/* End of Conversion on Channel N */
#define		AT91_ADC_OVRE(n)	(1 << ((n) + 8))/* Overrun Error on Channel N */
#define		AT91_ADC_DRDY		(1 << 16)	/* Data Ready */
#define		AT91_ADC_GOVRE		(1 << 17)	/* General Overrun Error */
#define		AT91_ADC_ENDRX		(1 << 18)	/* End of RX Buffer */
#define		AT91_ADC_RXFUFF		(1 << 19)	/* RX Buffer Full */

#define AT91_ADC_LCDR		0x20		/* Last Converted Data Register */
#define		AT91_ADC_LDATA		(0x3ff)

#define AT91_ADC_IER		0x24		/* Interrupt Enable Register */
#define AT91_ADC_IDR		0x28		/* Interrupt Disable Register */
#define AT91_ADC_IMR		0x2C		/* Interrupt Mask Register */

#define AT91_ADC_CHR(n)		(0x30 + ((n) * 4))	/* Channel Data Register N */
#define		AT91_ADC_DATA		(0x3ff)

#endif
