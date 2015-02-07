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
 * arch/sh/mm/ioremap_64.c
 *
 * Copyright (C) 2000, 2001  Paolo Alberelli
 * Copyright (C) 2003 - 2007  Paul Mundt
 *
 * Mostly derived from arch/sh/mm/ioremap.c which, in turn is mostly
 * derived from arch/i386/mm/ioremap.c .
 *
 *   (C) Copyright 1995 1996 Linus Torvalds
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/vmalloc.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/io.h>
#include <linux/bootmem.h>
#include <linux/proc_fs.h>
#include <asm/page.h>
#include <asm/pgalloc.h>
#include <asm/addrspace.h>
#include <asm/cacheflush.h>
#include <asm/tlbflush.h>
#include <asm/mmu.h>

static void shmedia_mapioaddr(unsigned long, unsigned long);
static unsigned long shmedia_ioremap(struct resource *, u32, int);

/*
 * Generic mapping function (not visible outside):
 */

/*
 * Remap an arbitrary physical address space into the kernel virtual
 * address space. Needed when the kernel wants to access high addresses
 * directly.
 *
 * NOTE! We need to allow non-page-aligned mappings too: we will obviously
 * have to convert them into an offset in a page-aligned mapping, but the
 * caller shouldn't need to know that small detail.
 */
void *__ioremap(unsigned long phys_addr, unsigned long size,
		unsigned long flags)
{
	void * addr;
	struct vm_struct * area;
	unsigned long offset, last_addr;
	pgprot_t pgprot;

	/* Don't allow wraparound or zero size */
	last_addr = phys_addr + size - 1;
	if (!size || last_addr < phys_addr)
		return NULL;

	pgprot = __pgprot(_PAGE_PRESENT  | _PAGE_READ   |
			  _PAGE_WRITE    | _PAGE_DIRTY  |
			  _PAGE_ACCESSED | _PAGE_SHARED | flags);

	/*
	 * Mappings have to be page-aligned
	 */
	offset = phys_addr & ~PAGE_MASK;
	phys_addr &= PAGE_MASK;
	size = PAGE_ALIGN(last_addr + 1) - phys_addr;

	/*
	 * Ok, go for it..
	 */
	area = get_vm_area(size, VM_IOREMAP);
	if (!area)
		return NULL;
	pr_debug("Get vm_area returns %p addr %p\n", area, area->addr);
	area->phys_addr = phys_addr;
	addr = area->addr;
	if (ioremap_page_range((unsigned long)addr, (unsigned long)addr + size,
			       phys_addr, pgprot)) {
		vunmap(addr);
		return NULL;
	}
	return (void *) (offset + (char *)addr);
}
EXPORT_SYMBOL(__ioremap);

void __iounmap(void *addr)
{
	struct vm_struct *area;

	vfree((void *) (PAGE_MASK & (unsigned long) addr));
	area = remove_vm_area((void *) (PAGE_MASK & (unsigned long) addr));
	if (!area) {
		printk(KERN_ERR "iounmap: bad address %p\n", addr);
		return;
	}

	kfree(area);
}
EXPORT_SYMBOL(__iounmap);

static struct resource shmedia_iomap = {
	.name	= "shmedia_iomap",
	.start	= IOBASE_VADDR + PAGE_SIZE,
	.end	= IOBASE_END - 1,
};

static void shmedia_mapioaddr(unsigned long pa, unsigned long va);
static void shmedia_unmapioaddr(unsigned long vaddr);
static unsigned long shmedia_ioremap(struct resource *res, u32 pa, int sz);

/*
 * We have the same problem as the SPARC, so lets have the same comment:
 * Our mini-allocator...
 * Boy this is gross! We need it because we must map I/O for
 * timers and interrupt controller before the kmalloc is available.
 */

#define XNMLN  15
#define XNRES  10

struct xresource {
	struct resource xres;   /* Must be first */
	int xflag;              /* 1 == used */
	char xname[XNMLN+1];
};

static struct xresource xresv[XNRES];

static struct xresource *xres_alloc(void)
{
        struct xresource *xrp;
        int n;

        xrp = xresv;
        for (n = 0; n < XNRES; n++) {
                if (xrp->xflag == 0) {
                        xrp->xflag = 1;
                        return xrp;
                }
                xrp++;
        }
        return NULL;
}

static void xres_free(struct xresource *xrp)
{
	xrp->xflag = 0;
}

static struct resource *shmedia_find_resource(struct resource *root,
					      unsigned long vaddr)
{
	struct resource *res;

	for (res = root->child; res; res = res->sibling)
		if (res->start <= vaddr && res->end >= vaddr)
			return res;

	return NULL;
}

static unsigned long shmedia_alloc_io(unsigned long phys, unsigned long size,
				      const char *name)
{
        static int printed_full = 0;
        struct xresource *xres;
        struct resource *res;
        char *tack;
        int tlen;

        if (name == NULL) name = "???";

        if ((xres = xres_alloc()) != 0) {
                tack = xres->xname;
                res = &xres->xres;
        } else {
                if (!printed_full) {
                        printk("%s: done with statics, switching to kmalloc\n",
			       __func__);
                        printed_full = 1;
                }
                tlen = strlen(name);
                tack = kmalloc(sizeof (struct resource) + tlen + 1, GFP_KERNEL);
                if (!tack)
			return -ENOMEM;
                memset(tack, 0, sizeof(struct resource));
                res = (struct resource *) tack;
                tack += sizeof (struct resource);
        }

        strncpy(tack, name, XNMLN);
        tack[XNMLN] = 0;
        res->name = tack;

        return shmedia_ioremap(res, phys, size);
}

static unsigned long shmedia_ioremap(struct resource *res, u32 pa, int sz)
{
        unsigned long offset = ((unsigned long) pa) & (~PAGE_MASK);
	unsigned long round_sz = (offset + sz + PAGE_SIZE-1) & PAGE_MASK;
        unsigned long va;
        unsigned int psz;

        if (allocate_resource(&shmedia_iomap, res, round_sz,
			      shmedia_iomap.start, shmedia_iomap.end,
			      PAGE_SIZE, NULL, NULL) != 0) {
                panic("alloc_io_res(%s): cannot occupy\n",
                    (res->name != NULL)? res->name: "???");
        }

        va = res->start;
        pa &= PAGE_MASK;

	psz = (res->end - res->start + (PAGE_SIZE - 1)) / PAGE_SIZE;

	/* log at boot time ... */
	printk("mapioaddr: %6s  [%2d page%s]  va 0x%08lx   pa 0x%08x\n",
	       ((res->name != NULL) ? res->name : "???"),
	       psz, psz == 1 ? " " : "s", va, pa);

        for (psz = res->end - res->start + 1; psz != 0; psz -= PAGE_SIZE) {
                shmedia_mapioaddr(pa, va);
                va += PAGE_SIZE;
                pa += PAGE_SIZE;
        }

        res->start += offset;
        res->end = res->start + sz - 1;         /* not strictly necessary.. */

        return res->start;
}

static void shmedia_free_io(struct resource *res)
{
	unsigned long len = res->end - res->start + 1;

	BUG_ON((len & (PAGE_SIZE - 1)) != 0);

	while (len) {
		len -= PAGE_SIZE;
		shmedia_unmapioaddr(res->start + len);
	}

	release_resource(res);
}

static __init_refok void *sh64_get_page(void)
{
	extern int after_bootmem;
	void *page;

	if (after_bootmem) {
		page = (void *)get_zeroed_page(GFP_ATOMIC);
	} else {
		page = alloc_bootmem_pages(PAGE_SIZE);
	}

	if (!page || ((unsigned long)page & ~PAGE_MASK))
		panic("sh64_get_page: Out of memory already?\n");

	return page;
}

static void shmedia_mapioaddr(unsigned long pa, unsigned long va)
{
	pgd_t *pgdp;
	pud_t *pudp;
	pmd_t *pmdp;
	pte_t *ptep, pte;
	pgprot_t prot;
	unsigned long flags = 1; /* 1 = CB0-1 device */

	pr_debug("shmedia_mapiopage pa %08lx va %08lx\n",  pa, va);

	pgdp = pgd_offset_k(va);
	if (pgd_none(*pgdp) || !pgd_present(*pgdp)) {
		pudp = (pud_t *)sh64_get_page();
		set_pgd(pgdp, __pgd((unsigned long)pudp | _KERNPG_TABLE));
	}

	pudp = pud_offset(pgdp, va);
	if (pud_none(*pudp) || !pud_present(*pudp)) {
		pmdp = (pmd_t *)sh64_get_page();
		set_pud(pudp, __pud((unsigned long)pmdp | _KERNPG_TABLE));
	}

	pmdp = pmd_offset(pudp, va);
	if (pmd_none(*pmdp) || !pmd_present(*pmdp) ) {
		ptep = (pte_t *)sh64_get_page();
		set_pmd(pmdp, __pmd((unsigned long)ptep + _PAGE_TABLE));
	}

	prot = __pgprot(_PAGE_PRESENT | _PAGE_READ     | _PAGE_WRITE  |
			_PAGE_DIRTY   | _PAGE_ACCESSED | _PAGE_SHARED | flags);

	pte = pfn_pte(pa >> PAGE_SHIFT, prot);
	ptep = pte_offset_kernel(pmdp, va);

	if (!pte_none(*ptep) &&
	    pte_val(*ptep) != pte_val(pte))
		pte_ERROR(*ptep);

	set_pte(ptep, pte);

	flush_tlb_kernel_range(va, PAGE_SIZE);
}

static void shmedia_unmapioaddr(unsigned long vaddr)
{
	pgd_t *pgdp;
	pud_t *pudp;
	pmd_t *pmdp;
	pte_t *ptep;

	pgdp = pgd_offset_k(vaddr);
	if (pgd_none(*pgdp) || pgd_bad(*pgdp))
		return;

	pudp = pud_offset(pgdp, vaddr);
	if (pud_none(*pudp) || pud_bad(*pudp))
		return;

	pmdp = pmd_offset(pudp, vaddr);
	if (pmd_none(*pmdp) || pmd_bad(*pmdp))
		return;

	ptep = pte_offset_kernel(pmdp, vaddr);

	if (pte_none(*ptep) || !pte_present(*ptep))
		return;

	clear_page((void *)ptep);
	pte_clear(&init_mm, vaddr, ptep);
}

unsigned long onchip_remap(unsigned long phys, unsigned long size, const char *name)
{
	if (size < PAGE_SIZE)
		size = PAGE_SIZE;

	return shmedia_alloc_io(phys, size, name);
}
EXPORT_SYMBOL(onchip_remap);

void onchip_unmap(unsigned long vaddr)
{
	struct resource *res;
	unsigned int psz;

	res = shmedia_find_resource(&shmedia_iomap, vaddr);
	if (!res) {
		printk(KERN_ERR "%s: Failed to free 0x%08lx\n",
		       __func__, vaddr);
		return;
	}

        psz = (res->end - res->start + (PAGE_SIZE - 1)) / PAGE_SIZE;

        printk(KERN_DEBUG "unmapioaddr: %6s  [%2d page%s] freed\n",
	       res->name, psz, psz == 1 ? " " : "s");

	shmedia_free_io(res);

	if ((char *)res >= (char *)xresv &&
	    (char *)res <  (char *)&xresv[XNRES]) {
		xres_free((struct xresource *)res);
	} else {
		kfree(res);
	}
}
EXPORT_SYMBOL(onchip_unmap);

#ifdef CONFIG_PROC_FS
static int
ioremap_proc_info(char *buf, char **start, off_t fpos, int length, int *eof,
		  void *data)
{
	char *p = buf, *e = buf + length;
	struct resource *r;
	const char *nm;

	for (r = ((struct resource *)data)->child; r != NULL; r = r->sibling) {
		if (p + 32 >= e)        /* Better than nothing */
			break;
		if ((nm = r->name) == 0) nm = "???";
		p += sprintf(p, "%08lx-%08lx: %s\n",
			     (unsigned long)r->start,
			     (unsigned long)r->end, nm);
	}

	return p-buf;
}
#endif /* CONFIG_PROC_FS */

static int __init register_proc_onchip(void)
{
#ifdef CONFIG_PROC_FS
	create_proc_read_entry("io_map",0,0, ioremap_proc_info, &shmedia_iomap);
#endif
	return 0;
}

__initcall(register_proc_onchip);