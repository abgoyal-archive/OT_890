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

#ifndef __MACH_SE_MRSHPC_H
#define __MACH_SE_MRSHPC_H

#include <linux/io.h>

static inline void __init mrshpc_setup_windows(void)
{
	if ((__raw_readw(MRSHPC_CSR) & 0x000c) != 0)
		return;	/* Not detected */

	if ((__raw_readw(MRSHPC_CSR) & 0x0080) == 0) {
		__raw_writew(0x0674, MRSHPC_CPWCR); /* Card Vcc is 3.3v? */
	} else {
		__raw_writew(0x0678, MRSHPC_CPWCR); /* Card Vcc is 5V */
	}

	/*
	 *  PC-Card window open
	 *  flag == COMMON/ATTRIBUTE/IO
	 */
	/* common window open */
	__raw_writew(0x8a84, MRSHPC_MW0CR1);
	if((__raw_readw(MRSHPC_CSR) & 0x4000) != 0)
		/* common mode & bus width 16bit SWAP = 1*/
		__raw_writew(0x0b00, MRSHPC_MW0CR2);
	else
		/* common mode & bus width 16bit SWAP = 0*/
		__raw_writew(0x0300, MRSHPC_MW0CR2);

	/* attribute window open */
	__raw_writew(0x8a85, MRSHPC_MW1CR1);
	if ((__raw_readw(MRSHPC_CSR) & 0x4000) != 0)
		/* attribute mode & bus width 16bit SWAP = 1*/
		__raw_writew(0x0a00, MRSHPC_MW1CR2);
	else
		/* attribute mode & bus width 16bit SWAP = 0*/
		__raw_writew(0x0200, MRSHPC_MW1CR2);

	/* I/O window open */
	__raw_writew(0x8a86, MRSHPC_IOWCR1);
	__raw_writew(0x0008, MRSHPC_CDCR);	 /* I/O card mode */
	if ((__raw_readw(MRSHPC_CSR) & 0x4000) != 0)
		__raw_writew(0x0a00, MRSHPC_IOWCR2); /* bus width 16bit SWAP = 1*/
	else
		__raw_writew(0x0200, MRSHPC_IOWCR2); /* bus width 16bit SWAP = 0*/

	__raw_writew(0x2000, MRSHPC_ICR);
	__raw_writeb(0x00, PA_MRSHPC_MW2 + 0x206);
	__raw_writeb(0x42, PA_MRSHPC_MW2 + 0x200);
}

#endif /* __MACH_SE_MRSHPC_H */
