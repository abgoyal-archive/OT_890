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

#ifndef _SPARC64_UPA_H
#define _SPARC64_UPA_H

#include <asm/asi.h>

/* UPA level registers and defines. */

/* UPA Config Register */
#define UPA_CONFIG_RESV		0xffffffffc0000000 /* Reserved.                    */
#define UPA_CONFIG_PCON		0x000000003fc00000 /* Depth of various sys queues. */
#define UPA_CONFIG_MID		0x00000000003e0000 /* Module ID.                   */
#define UPA_CONFIG_PCAP		0x000000000001ffff /* Port Capabilities.           */

/* UPA Port ID Register */
#define UPA_PORTID_FNP		0xff00000000000000 /* Hardcoded to 0xfc on ultra.  */
#define UPA_PORTID_RESV		0x00fffff800000000 /* Reserved.                    */
#define UPA_PORTID_ECCVALID     0x0000000400000000 /* Zero if mod can generate ECC */
#define UPA_PORTID_ONEREAD      0x0000000200000000 /* Set if mod generates P_RASB  */
#define UPA_PORTID_PINTRDQ      0x0000000180000000 /* # outstanding P_INT_REQ's    */
#define UPA_PORTID_PREQDQ       0x000000007e000000 /* slave-wr's to mod supported  */
#define UPA_PORTID_PREQRD       0x0000000001e00000 /* # incoming P_REQ's supported */
#define UPA_PORTID_UPACAP       0x00000000001f0000 /* UPA capabilities of mod      */
#define UPA_PORTID_ID           0x000000000000ffff /* Module Identification bits  */

/* UPA I/O space accessors */
#if defined(__KERNEL__) && !defined(__ASSEMBLY__)
static inline unsigned char _upa_readb(unsigned long addr)
{
	unsigned char ret;

	__asm__ __volatile__("lduba\t[%1] %2, %0\t/* upa_readb */"
			     : "=r" (ret)
			     : "r" (addr), "i" (ASI_PHYS_BYPASS_EC_E));

	return ret;
}

static inline unsigned short _upa_readw(unsigned long addr)
{
	unsigned short ret;

	__asm__ __volatile__("lduha\t[%1] %2, %0\t/* upa_readw */"
			     : "=r" (ret)
			     : "r" (addr), "i" (ASI_PHYS_BYPASS_EC_E));

	return ret;
}

static inline unsigned int _upa_readl(unsigned long addr)
{
	unsigned int ret;

	__asm__ __volatile__("lduwa\t[%1] %2, %0\t/* upa_readl */"
			     : "=r" (ret)
			     : "r" (addr), "i" (ASI_PHYS_BYPASS_EC_E));

	return ret;
}

static inline unsigned long _upa_readq(unsigned long addr)
{
	unsigned long ret;

	__asm__ __volatile__("ldxa\t[%1] %2, %0\t/* upa_readq */"
			     : "=r" (ret)
			     : "r" (addr), "i" (ASI_PHYS_BYPASS_EC_E));

	return ret;
}

static inline void _upa_writeb(unsigned char b, unsigned long addr)
{
	__asm__ __volatile__("stba\t%0, [%1] %2\t/* upa_writeb */"
			     : /* no outputs */
			     : "r" (b), "r" (addr), "i" (ASI_PHYS_BYPASS_EC_E));
}

static inline void _upa_writew(unsigned short w, unsigned long addr)
{
	__asm__ __volatile__("stha\t%0, [%1] %2\t/* upa_writew */"
			     : /* no outputs */
			     : "r" (w), "r" (addr), "i" (ASI_PHYS_BYPASS_EC_E));
}

static inline void _upa_writel(unsigned int l, unsigned long addr)
{
	__asm__ __volatile__("stwa\t%0, [%1] %2\t/* upa_writel */"
			     : /* no outputs */
			     : "r" (l), "r" (addr), "i" (ASI_PHYS_BYPASS_EC_E));
}

static inline void _upa_writeq(unsigned long q, unsigned long addr)
{
	__asm__ __volatile__("stxa\t%0, [%1] %2\t/* upa_writeq */"
			     : /* no outputs */
			     : "r" (q), "r" (addr), "i" (ASI_PHYS_BYPASS_EC_E));
}

#define upa_readb(__addr)		(_upa_readb((unsigned long)(__addr)))
#define upa_readw(__addr)		(_upa_readw((unsigned long)(__addr)))
#define upa_readl(__addr)		(_upa_readl((unsigned long)(__addr)))
#define upa_readq(__addr)		(_upa_readq((unsigned long)(__addr)))
#define upa_writeb(__b, __addr)		(_upa_writeb((__b), (unsigned long)(__addr)))
#define upa_writew(__w, __addr)		(_upa_writew((__w), (unsigned long)(__addr)))
#define upa_writel(__l, __addr)		(_upa_writel((__l), (unsigned long)(__addr)))
#define upa_writeq(__q, __addr)		(_upa_writeq((__q), (unsigned long)(__addr)))
#endif /* __KERNEL__ && !__ASSEMBLY__ */

#endif /* !(_SPARC64_UPA_H) */
