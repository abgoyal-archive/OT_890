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
 * Implementation of s390 diagnose codes
 *
 * Copyright IBM Corp. 2007
 * Author(s): Michael Holzheu <holzheu@de.ibm.com>
 */

#include <linux/module.h>
#include <asm/diag.h>

/*
 * Diagnose 10: Release pages
 */
void diag10(unsigned long addr)
{
	if (addr >= 0x7ff00000)
		return;
	asm volatile(
#ifdef CONFIG_64BIT
		"	sam31\n"
		"	diag	%0,%0,0x10\n"
		"0:	sam64\n"
#else
		"	diag	%0,%0,0x10\n"
		"0:\n"
#endif
		EX_TABLE(0b, 0b)
		: : "a" (addr));
}
EXPORT_SYMBOL(diag10);

/*
 * Diagnose 14: Input spool file manipulation
 */
int diag14(unsigned long rx, unsigned long ry1, unsigned long subcode)
{
	register unsigned long _ry1 asm("2") = ry1;
	register unsigned long _ry2 asm("3") = subcode;
	int rc = 0;

	asm volatile(
#ifdef CONFIG_64BIT
		"   sam31\n"
		"   diag    %2,2,0x14\n"
		"   sam64\n"
#else
		"   diag    %2,2,0x14\n"
#endif
		"   ipm     %0\n"
		"   srl     %0,28\n"
		: "=d" (rc), "+d" (_ry2)
		: "d" (rx), "d" (_ry1)
		: "cc");

	return rc;
}
EXPORT_SYMBOL(diag14);

/*
 * Diagnose 210: Get information about a virtual device
 */
int diag210(struct diag210 *addr)
{
	/*
	 * diag 210 needs its data below the 2GB border, so we
	 * use a static data area to be sure
	 */
	static struct diag210 diag210_tmp;
	static DEFINE_SPINLOCK(diag210_lock);
	unsigned long flags;
	int ccode;

	spin_lock_irqsave(&diag210_lock, flags);
	diag210_tmp = *addr;

#ifdef CONFIG_64BIT
	asm volatile(
		"	lhi	%0,-1\n"
		"	sam31\n"
		"	diag	%1,0,0x210\n"
		"0:	ipm	%0\n"
		"	srl	%0,28\n"
		"1:	sam64\n"
		EX_TABLE(0b, 1b)
		: "=&d" (ccode) : "a" (&diag210_tmp) : "cc", "memory");
#else
	asm volatile(
		"	lhi	%0,-1\n"
		"	diag	%1,0,0x210\n"
		"0:	ipm	%0\n"
		"	srl	%0,28\n"
		"1:\n"
		EX_TABLE(0b, 1b)
		: "=&d" (ccode) : "a" (&diag210_tmp) : "cc", "memory");
#endif

	*addr = diag210_tmp;
	spin_unlock_irqrestore(&diag210_lock, flags);

	return ccode;
}
EXPORT_SYMBOL(diag210);
