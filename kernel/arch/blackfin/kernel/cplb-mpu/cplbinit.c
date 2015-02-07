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
 * Blackfin CPLB initialization
 *
 *               Copyright 2004-2007 Analog Devices Inc.
 *
 * Bugs:         Enter bugs at http://blackfin.uclinux.org/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <linux/module.h>

#include <asm/blackfin.h>
#include <asm/cplb.h>
#include <asm/cplbinit.h>
#include <asm/mem_map.h>

#if ANOMALY_05000263
# error the MPU will not function safely while Anomaly 05000263 applies
#endif

struct cplb_entry icplb_tbl[NR_CPUS][MAX_CPLBS];
struct cplb_entry dcplb_tbl[NR_CPUS][MAX_CPLBS];

int first_switched_icplb, first_switched_dcplb;
int first_mask_dcplb;

void __init generate_cplb_tables_cpu(unsigned int cpu)
{
	int i_d, i_i;
	unsigned long addr;
	unsigned long d_data, i_data;
	unsigned long d_cache = 0, i_cache = 0;

	printk(KERN_INFO "MPU: setting up cplb tables with memory protection\n");

#ifdef CONFIG_BFIN_ICACHE
	i_cache = CPLB_L1_CHBL | ANOMALY_05000158_WORKAROUND;
#endif

#ifdef CONFIG_BFIN_DCACHE
	d_cache = CPLB_L1_CHBL;
#ifdef CONFIG_BFIN_WT
	d_cache |= CPLB_L1_AOW | CPLB_WT;
#endif
#endif

	i_d = i_i = 0;

	/* Set up the zero page.  */
	dcplb_tbl[cpu][i_d].addr = 0;
	dcplb_tbl[cpu][i_d++].data = SDRAM_OOPS | PAGE_SIZE_1KB;

	icplb_tbl[cpu][i_i].addr = 0;
	icplb_tbl[cpu][i_i++].data = i_cache | CPLB_USER_RD | PAGE_SIZE_1KB;

	/* Cover kernel memory with 4M pages.  */
	addr = 0;
	d_data = d_cache | CPLB_SUPV_WR | CPLB_VALID | PAGE_SIZE_4MB | CPLB_DIRTY;
	i_data = i_cache | CPLB_VALID | CPLB_PORTPRIO | PAGE_SIZE_4MB;

	for (; addr < memory_start; addr += 4 * 1024 * 1024) {
		dcplb_tbl[cpu][i_d].addr = addr;
		dcplb_tbl[cpu][i_d++].data = d_data;
		icplb_tbl[cpu][i_i].addr = addr;
		icplb_tbl[cpu][i_i++].data = i_data | (addr == 0 ? CPLB_USER_RD : 0);
	}

	/* Cover L1 memory.  One 4M area for code and data each is enough.  */
#if L1_DATA_A_LENGTH > 0 || L1_DATA_B_LENGTH > 0
	dcplb_tbl[cpu][i_d].addr = get_l1_data_a_start_cpu(cpu);
	dcplb_tbl[cpu][i_d++].data = L1_DMEMORY | PAGE_SIZE_4MB;
#endif
#if L1_CODE_LENGTH > 0
	icplb_tbl[cpu][i_i].addr = get_l1_code_start_cpu(cpu);
	icplb_tbl[cpu][i_i++].data = L1_IMEMORY | PAGE_SIZE_4MB;
#endif

	/* Cover L2 memory */
#if L2_LENGTH > 0
	dcplb_tbl[cpu][i_d].addr = L2_START;
	dcplb_tbl[cpu][i_d++].data = L2_DMEMORY | PAGE_SIZE_1MB;
	icplb_tbl[cpu][i_i].addr = L2_START;
	icplb_tbl[cpu][i_i++].data = L2_IMEMORY | PAGE_SIZE_1MB;
#endif

	first_mask_dcplb = i_d;
	first_switched_dcplb = i_d + (1 << page_mask_order);
	first_switched_icplb = i_i;

	while (i_d < MAX_CPLBS)
		dcplb_tbl[cpu][i_d++].data = 0;
	while (i_i < MAX_CPLBS)
		icplb_tbl[cpu][i_i++].data = 0;
}

void generate_cplb_tables_all(void)
{
}
