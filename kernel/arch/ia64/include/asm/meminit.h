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

#ifndef meminit_h
#define meminit_h

/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */


/*
 * Entries defined so far:
 * 	- boot param structure itself
 * 	- memory map
 * 	- initrd (optional)
 * 	- command line string
 * 	- kernel code & data
 * 	- crash dumping code reserved region
 * 	- Kernel memory map built from EFI memory map
 * 	- ELF core header
 *	- xen start info if CONFIG_XEN
 *
 * More could be added if necessary
 */
#define IA64_MAX_RSVD_REGIONS 9

struct rsvd_region {
	unsigned long start;	/* virtual address of beginning of element */
	unsigned long end;	/* virtual address of end of element + 1 */
};

extern struct rsvd_region rsvd_region[IA64_MAX_RSVD_REGIONS + 1];
extern int num_rsvd_regions;

extern void find_memory (void);
extern void reserve_memory (void);
extern void find_initrd (void);
extern int filter_rsvd_memory (unsigned long start, unsigned long end, void *arg);
extern int filter_memory (unsigned long start, unsigned long end, void *arg);
extern unsigned long efi_memmap_init(unsigned long *s, unsigned long *e);
extern int find_max_min_low_pfn (unsigned long , unsigned long, void *);

extern unsigned long vmcore_find_descriptor_size(unsigned long address);
extern int reserve_elfcorehdr(unsigned long *start, unsigned long *end);

/*
 * For rounding an address to the next IA64_GRANULE_SIZE or order
 */
#define GRANULEROUNDDOWN(n)	((n) & ~(IA64_GRANULE_SIZE-1))
#define GRANULEROUNDUP(n)	(((n)+IA64_GRANULE_SIZE-1) & ~(IA64_GRANULE_SIZE-1))

#ifdef CONFIG_NUMA
  extern void call_pernode_memory (unsigned long start, unsigned long len, void *func);
#else
# define call_pernode_memory(start, len, func)	(*func)(start, len, 0)
#endif

#define IGNORE_PFN0	1	/* XXX fix me: ignore pfn 0 until TLB miss handler is updated... */

extern int register_active_ranges(u64 start, u64 len, int nid);

#ifdef CONFIG_VIRTUAL_MEM_MAP
# define LARGE_GAP	0x40000000 /* Use virtual mem map if hole is > than this */
  extern unsigned long vmalloc_end;
  extern struct page *vmem_map;
  extern int find_largest_hole (u64 start, u64 end, void *arg);
  extern int create_mem_map_page_table (u64 start, u64 end, void *arg);
  extern int vmemmap_find_next_valid_pfn(int, int);
#else
static inline int vmemmap_find_next_valid_pfn(int node, int i)
{
	return i + 1;
}
#endif
#endif /* meminit_h */
