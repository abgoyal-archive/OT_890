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
 * arch/arm/mach-at91/include/mach/at91_pio.h
 *
 * Copyright (C) 2005 Ivan Kokshaysky
 * Copyright (C) SAN People
 *
 * Parallel I/O Controller (PIO) - System peripherals registers.
 * Based on AT91RM9200 datasheet revision E.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef AT91_PIO_H
#define AT91_PIO_H

#define PIO_PER		0x00	/* Enable Register */
#define PIO_PDR		0x04	/* Disable Register */
#define PIO_PSR		0x08	/* Status Register */
#define PIO_OER		0x10	/* Output Enable Register */
#define PIO_ODR		0x14	/* Output Disable Register */
#define PIO_OSR		0x18	/* Output Status Register */
#define PIO_IFER	0x20	/* Glitch Input Filter Enable */
#define PIO_IFDR	0x24	/* Glitch Input Filter Disable */
#define PIO_IFSR	0x28	/* Glitch Input Filter Status */
#define PIO_SODR	0x30	/* Set Output Data Register */
#define PIO_CODR	0x34	/* Clear Output Data Register */
#define PIO_ODSR	0x38	/* Output Data Status Register */
#define PIO_PDSR	0x3c	/* Pin Data Status Register */
#define PIO_IER		0x40	/* Interrupt Enable Register */
#define PIO_IDR		0x44	/* Interrupt Disable Register */
#define PIO_IMR		0x48	/* Interrupt Mask Register */
#define PIO_ISR		0x4c	/* Interrupt Status Register */
#define PIO_MDER	0x50	/* Multi-driver Enable Register */
#define PIO_MDDR	0x54	/* Multi-driver Disable Register */
#define PIO_MDSR	0x58	/* Multi-driver Status Register */
#define PIO_PUDR	0x60	/* Pull-up Disable Register */
#define PIO_PUER	0x64	/* Pull-up Enable Register */
#define PIO_PUSR	0x68	/* Pull-up Status Register */
#define PIO_ASR		0x70	/* Peripheral A Select Register */
#define PIO_BSR		0x74	/* Peripheral B Select Register */
#define PIO_ABSR	0x78	/* AB Status Register */
#define PIO_OWER	0xa0	/* Output Write Enable Register */
#define PIO_OWDR	0xa4	/* Output Write Disable Register */
#define PIO_OWSR	0xa8	/* Output Write Status Register */

#endif
