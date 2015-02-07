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
 * nosun4c.c: This file is a bunch of dummies for SMP compiles, 
 *         so that it does not need sun4c and avoid ifdefs.
 *
 * Copyright (C) 1998 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 */

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <asm/pgtable.h>

static char shouldnothappen[] __initdata = "32bit SMP kernel only supports sun4m and sun4d\n";

/* Dummies */
struct sun4c_mmu_ring {
	unsigned long xxx1[3];
	unsigned char xxx2[2];
	int xxx3;
};
struct sun4c_mmu_ring sun4c_kernel_ring;
struct sun4c_mmu_ring sun4c_kfree_ring;
unsigned long sun4c_kernel_faults;
unsigned long *sun4c_memerr_reg;

static void __init should_not_happen(void)
{
	prom_printf(shouldnothappen);
	prom_halt();
}

unsigned long __init sun4c_paging_init(unsigned long start_mem, unsigned long end_mem)
{
	should_not_happen();
	return 0;
}

void __init ld_mmu_sun4c(void)
{
	should_not_happen();
}

void sun4c_mapioaddr(unsigned long physaddr, unsigned long virt_addr, int bus_type, int rdonly)
{
}

void sun4c_unmapioaddr(unsigned long virt_addr)
{
}

void sun4c_complete_all_stores(void)
{
}

pte_t *sun4c_pte_offset(pmd_t * dir, unsigned long address)
{
	return NULL;
}

pte_t *sun4c_pte_offset_kernel(pmd_t *dir, unsigned long address)
{
	return NULL;
}

void sun4c_update_mmu_cache(struct vm_area_struct *vma, unsigned long address, pte_t pte)
{
}

void __init sun4c_probe_vac(void)
{
	should_not_happen();
}

void __init sun4c_probe_memerr_reg(void)
{
	should_not_happen();
}
