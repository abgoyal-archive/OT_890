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
 * arch/arm/mach-at91/include/mach/at91_aic.h
 *
 * Copyright (C) 2005 Ivan Kokshaysky
 * Copyright (C) SAN People
 *
 * Advanced Interrupt Controller (AIC) - System peripherals registers.
 * Based on AT91RM9200 datasheet revision E.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef AT91_AIC_H
#define AT91_AIC_H

#define AT91_AIC_SMR(n)		(AT91_AIC + ((n) * 4))	/* Source Mode Registers 0-31 */
#define		AT91_AIC_PRIOR		(7 << 0)		/* Priority Level */
#define		AT91_AIC_SRCTYPE	(3 << 5)		/* Interrupt Source Type */
#define			AT91_AIC_SRCTYPE_LOW		(0 << 5)
#define			AT91_AIC_SRCTYPE_FALLING	(1 << 5)
#define			AT91_AIC_SRCTYPE_HIGH		(2 << 5)
#define			AT91_AIC_SRCTYPE_RISING		(3 << 5)

#define AT91_AIC_SVR(n)		(AT91_AIC + 0x80 + ((n) * 4))	/* Source Vector Registers 0-31 */
#define AT91_AIC_IVR		(AT91_AIC + 0x100)	/* Interrupt Vector Register */
#define AT91_AIC_FVR		(AT91_AIC + 0x104)	/* Fast Interrupt Vector Register */
#define AT91_AIC_ISR		(AT91_AIC + 0x108)	/* Interrupt Status Register */
#define		AT91_AIC_IRQID		(0x1f << 0)		/* Current Interrupt Identifier */

#define AT91_AIC_IPR		(AT91_AIC + 0x10c)	/* Interrupt Pending Register */
#define AT91_AIC_IMR		(AT91_AIC + 0x110)	/* Interrupt Mask Register */
#define AT91_AIC_CISR		(AT91_AIC + 0x114)	/* Core Interrupt Status Register */
#define		AT91_AIC_NFIQ		(1 << 0)		/* nFIQ Status */
#define		AT91_AIC_NIRQ		(1 << 1)		/* nIRQ Status */

#define AT91_AIC_IECR		(AT91_AIC + 0x120)	/* Interrupt Enable Command Register */
#define AT91_AIC_IDCR		(AT91_AIC + 0x124)	/* Interrupt Disable Command Register */
#define AT91_AIC_ICCR		(AT91_AIC + 0x128)	/* Interrupt Clear Command Register */
#define AT91_AIC_ISCR		(AT91_AIC + 0x12c)	/* Interrupt Set Command Register */
#define AT91_AIC_EOICR		(AT91_AIC + 0x130)	/* End of Interrupt Command Register */
#define AT91_AIC_SPU		(AT91_AIC + 0x134)	/* Spurious Interrupt Vector Register */
#define AT91_AIC_DCR		(AT91_AIC + 0x138)	/* Debug Control Register */
#define		AT91_AIC_DCR_PROT	(1 << 0)		/* Protection Mode */
#define		AT91_AIC_DCR_GMSK	(1 << 1)		/* General Mask */

#define AT91_AIC_FFER		(AT91_AIC + 0x140)	/* Fast Forcing Enable Register [SAM9 only] */
#define AT91_AIC_FFDR		(AT91_AIC + 0x144)	/* Fast Forcing Disable Register [SAM9 only] */
#define AT91_AIC_FFSR		(AT91_AIC + 0x148)	/* Fast Forcing Status Register [SAM9 only] */

#endif
