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

/* arch/arm/mach-s3c2410/include/mach/h1940-latch.h
 *
 * Copyright (c) 2005 Simtec Electronics
 *	http://armlinux.simtec.co.uk/
 *	Ben Dooks <ben@simtec.co.uk>
 *
 *  iPAQ H1940 series - latch definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ASM_ARCH_H1940_LATCH_H
#define __ASM_ARCH_H1940_LATCH_H


#ifndef __ASSEMBLY__
#define H1940_LATCH		((void __force __iomem *)0xF8000000)
#else
#define H1940_LATCH		0xF8000000
#endif

#define H1940_PA_LATCH		(S3C2410_CS2)

/* SD layer latch */

#define H1940_LATCH_SDQ1		(1<<16)
#define H1940_LATCH_LCD_P1		(1<<17)
#define H1940_LATCH_LCD_P2		(1<<18)
#define H1940_LATCH_LCD_P3		(1<<19)
#define H1940_LATCH_MAX1698_nSHUTDOWN	(1<<20)		/* LCD backlight */
#define H1940_LATCH_LED_RED		(1<<21)
#define H1940_LATCH_SDQ7		(1<<22)
#define H1940_LATCH_USB_DP		(1<<23)

/* CPU layer latch */

#define H1940_LATCH_UDA_POWER		(1<<24)
#define H1940_LATCH_AUDIO_POWER		(1<<25)
#define H1940_LATCH_SM803_ENABLE	(1<<26)
#define H1940_LATCH_LCD_P4		(1<<27)
#define H1940_LATCH_CPUQ5		(1<<28)		/* untraced */
#define H1940_LATCH_BLUETOOTH_POWER	(1<<29)		/* active high */
#define H1940_LATCH_LED_GREEN		(1<<30)
#define H1940_LATCH_LED_FLASH		(1<<31)

/* default settings */

#define H1940_LATCH_DEFAULT		\
	H1940_LATCH_LCD_P4		| \
	H1940_LATCH_SM803_ENABLE	| \
	H1940_LATCH_SDQ1		| \
	H1940_LATCH_LCD_P1		| \
	H1940_LATCH_LCD_P2		| \
	H1940_LATCH_LCD_P3		| \
	H1940_LATCH_MAX1698_nSHUTDOWN   | \
	H1940_LATCH_CPUQ5

/* control functions */

extern void h1940_latch_control(unsigned int clear, unsigned int set);

#endif /* __ASM_ARCH_H1940_LATCH_H */
