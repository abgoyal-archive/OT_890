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
 * arch/s390/mm/page-states.c
 *
 * Copyright IBM Corp. 2008
 *
 * Guest page hinting for unused pages.
 *
 * Author(s): Martin Schwidefsky <schwidefsky@de.ibm.com>
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/init.h>

#define ESSA_SET_STABLE		1
#define ESSA_SET_UNUSED		2

static int cmma_flag;

static int __init cmma(char *str)
{
	char *parm;
	parm = strstrip(str);
	if (strcmp(parm, "yes") == 0 || strcmp(parm, "on") == 0) {
		cmma_flag = 1;
		return 1;
	}
	cmma_flag = 0;
	if (strcmp(parm, "no") == 0 || strcmp(parm, "off") == 0)
		return 1;
	return 0;
}

__setup("cmma=", cmma);

void __init cmma_init(void)
{
	register unsigned long tmp asm("0") = 0;
	register int rc asm("1") = -EOPNOTSUPP;

	if (!cmma_flag)
		return;
	asm volatile(
		"       .insn rrf,0xb9ab0000,%1,%1,0,0\n"
		"0:     la      %0,0\n"
		"1:\n"
		EX_TABLE(0b,1b)
		: "+&d" (rc), "+&d" (tmp));
	if (rc)
		cmma_flag = 0;
}

void arch_free_page(struct page *page, int order)
{
	int i, rc;

	if (!cmma_flag)
		return;
	for (i = 0; i < (1 << order); i++)
		asm volatile(".insn rrf,0xb9ab0000,%0,%1,%2,0"
			     : "=&d" (rc)
			     : "a" ((page_to_pfn(page) + i) << PAGE_SHIFT),
			       "i" (ESSA_SET_UNUSED));
}

void arch_alloc_page(struct page *page, int order)
{
	int i, rc;

	if (!cmma_flag)
		return;
	for (i = 0; i < (1 << order); i++)
		asm volatile(".insn rrf,0xb9ab0000,%0,%1,%2,0"
			     : "=&d" (rc)
			     : "a" ((page_to_pfn(page) + i) << PAGE_SHIFT),
			       "i" (ESSA_SET_STABLE));
}
