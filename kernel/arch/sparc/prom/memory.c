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

/* memory.c: Prom routine for acquiring various bits of information
 *           about RAM on the machine, both virtual and physical.
 *
 * Copyright (C) 1995, 2008 David S. Miller (davem@davemloft.net)
 * Copyright (C) 1997 Michael A. Griffith (grif@acm.org)
 */

#include <linux/kernel.h>
#include <linux/sort.h>
#include <linux/init.h>

#include <asm/openprom.h>
#include <asm/oplib.h>
#include <asm/page.h>

static int __init prom_meminit_v0(void)
{
	struct linux_mlist_v0 *p;
	int index;

	index = 0;
	for (p = *(romvec->pv_v0mem.v0_available); p; p = p->theres_more) {
		sp_banks[index].base_addr = (unsigned long) p->start_adr;
		sp_banks[index].num_bytes = p->num_bytes;
		index++;
	}

	return index;
}

static int __init prom_meminit_v2(void)
{
	struct linux_prom_registers reg[64];
	int node, size, num_ents, i;

	node = prom_searchsiblings(prom_getchild(prom_root_node), "memory");
	size = prom_getproperty(node, "available", (char *) reg, sizeof(reg));
	num_ents = size / sizeof(struct linux_prom_registers);

	for (i = 0; i < num_ents; i++) {
		sp_banks[i].base_addr = reg[i].phys_addr;
		sp_banks[i].num_bytes = reg[i].reg_size;
	}

	return num_ents;
}

static int sp_banks_cmp(const void *a, const void *b)
{
	const struct sparc_phys_banks *x = a, *y = b;

	if (x->base_addr > y->base_addr)
		return 1;
	if (x->base_addr < y->base_addr)
		return -1;
	return 0;
}

/* Initialize the memory lists based upon the prom version. */
void __init prom_meminit(void)
{
	int i, num_ents = 0;

	switch (prom_vers) {
	case PROM_V0:
		num_ents = prom_meminit_v0();
		break;

	case PROM_V2:
	case PROM_V3:
		num_ents = prom_meminit_v2();
		break;

	default:
		break;
	}
	sort(sp_banks, num_ents, sizeof(struct sparc_phys_banks),
	     sp_banks_cmp, NULL);

	/* Sentinel.  */
	sp_banks[num_ents].base_addr = 0xdeadbeef;
	sp_banks[num_ents].num_bytes = 0;

	for (i = 0; i < num_ents; i++)
		sp_banks[i].num_bytes &= PAGE_MASK;
}
