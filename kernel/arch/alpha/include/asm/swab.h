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

#ifndef _ALPHA_SWAB_H
#define _ALPHA_SWAB_H

#include <asm/types.h>
#include <linux/compiler.h>
#include <asm/compiler.h>

#ifdef __GNUC__

static inline __attribute_const__ __u32 __arch_swab32(__u32 x)
{
	/*
	 * Unfortunately, we can't use the 6 instruction sequence
	 * on ev6 since the latency of the UNPKBW is 3, which is
	 * pretty hard to hide.  Just in case a future implementation
	 * has a lower latency, here's the sequence (also by Mike Burrows)
	 *
	 * UNPKBW a0, v0       v0: 00AA00BB00CC00DD
	 * SLL v0, 24, a0      a0: BB00CC00DD000000
	 * BIS v0, a0, a0      a0: BBAACCBBDDCC00DD
	 * EXTWL a0, 6, v0     v0: 000000000000BBAA
	 * ZAP a0, 0xf3, a0    a0: 00000000DDCC0000
	 * ADDL a0, v0, v0     v0: ssssssssDDCCBBAA
	 */

	__u64 t0, t1, t2, t3;

	t0 = __kernel_inslh(x, 7);	/* t0 : 0000000000AABBCC */
	t1 = __kernel_inswl(x, 3);	/* t1 : 000000CCDD000000 */
	t1 |= t0;			/* t1 : 000000CCDDAABBCC */
	t2 = t1 >> 16;			/* t2 : 0000000000CCDDAA */
	t0 = t1 & 0xFF00FF00;		/* t0 : 00000000DD00BB00 */
	t3 = t2 & 0x00FF00FF;		/* t3 : 0000000000CC00AA */
	t1 = t0 + t3;			/* t1 : ssssssssDDCCBBAA */

	return t1;
}
#define __arch_swab32 __arch_swab32

#endif /* __GNUC__ */

#endif /* _ALPHA_SWAB_H */
