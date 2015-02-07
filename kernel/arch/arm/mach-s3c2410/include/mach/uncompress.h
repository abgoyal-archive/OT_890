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


/* arch/arm/mach-s3c2410/include/mach/uncompress.h
 *
 * Copyright (c) 2003, 2007 Simtec Electronics
 *	http://armlinux.simtec.co.uk/
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * S3C2410 - uncompress code
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ASM_ARCH_UNCOMPRESS_H
#define __ASM_ARCH_UNCOMPRESS_H

#include <mach/regs-gpio.h>
#include <mach/map.h>

/* working in physical space... */
#undef S3C2410_GPIOREG
#define S3C2410_GPIOREG(x) ((S3C24XX_PA_GPIO + (x)))

#include <plat/uncompress.h>

static inline int is_arm926(void)
{
	unsigned int cpuid;

	asm volatile ("mrc p15, 0, %0, c1, c0, 0" : "=r" (cpuid));

	return ((cpuid & 0xff0) == 0x260);
}

static void arch_detect_cpu(void)
{
	unsigned int cpuid;

	cpuid = *((volatile unsigned int *)S3C2410_GSTATUS1);
	cpuid &= S3C2410_GSTATUS1_IDMASK;

	if (is_arm926() || cpuid == S3C2410_GSTATUS1_2440 ||
	    cpuid == S3C2410_GSTATUS1_2442) {
		fifo_mask = S3C2440_UFSTAT_TXMASK;
		fifo_max = 63 << S3C2440_UFSTAT_TXSHIFT;
	} else {
		fifo_mask = S3C2410_UFSTAT_TXMASK;
		fifo_max = 15 << S3C2410_UFSTAT_TXSHIFT;
	}
}

#endif /* __ASM_ARCH_UNCOMPRESS_H */
