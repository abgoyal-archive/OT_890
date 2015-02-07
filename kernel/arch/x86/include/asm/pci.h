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

#ifndef _ASM_X86_PCI_H
#define _ASM_X86_PCI_H

#include <linux/mm.h> /* for struct page */
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <asm/scatterlist.h>
#include <asm/io.h>

#ifdef __KERNEL__

struct pci_sysdata {
	int		domain;		/* PCI domain */
	int		node;		/* NUMA node */
#ifdef CONFIG_X86_64
	void		*iommu;		/* IOMMU private data */
#endif
};

extern int pci_routeirq;
extern int noioapicquirk;
extern int noioapicreroute;

/* scan a bus after allocating a pci_sysdata for it */
extern struct pci_bus *pci_scan_bus_on_node(int busno, struct pci_ops *ops,
					    int node);
extern struct pci_bus *pci_scan_bus_with_sysdata(int busno);

static inline int pci_domain_nr(struct pci_bus *bus)
{
	struct pci_sysdata *sd = bus->sysdata;
	return sd->domain;
}

static inline int pci_proc_domain(struct pci_bus *bus)
{
	return pci_domain_nr(bus);
}


/* Can be used to override the logic in pci_scan_bus for skipping
   already-configured bus numbers - to be used for buggy BIOSes
   or architectures with incomplete PCI setup by the loader */

#ifdef CONFIG_PCI
extern unsigned int pcibios_assign_all_busses(void);
#else
#define pcibios_assign_all_busses()	0
#endif
#define pcibios_scan_all_fns(a, b)	0

extern unsigned long pci_mem_start;
#define PCIBIOS_MIN_IO		0x1000
#define PCIBIOS_MIN_MEM		(pci_mem_start)

#define PCIBIOS_MIN_CARDBUS_IO	0x4000

void pcibios_config_init(void);
struct pci_bus *pcibios_scan_root(int bus);

void pcibios_set_master(struct pci_dev *dev);
void pcibios_penalize_isa_irq(int irq, int active);
struct irq_routing_table *pcibios_get_irq_routing_table(void);
int pcibios_set_irq_routing(struct pci_dev *dev, int pin, int irq);


#define HAVE_PCI_MMAP
extern int pci_mmap_page_range(struct pci_dev *dev, struct vm_area_struct *vma,
			       enum pci_mmap_state mmap_state,
			       int write_combine);


#ifdef CONFIG_PCI
extern void early_quirks(void);
static inline void pci_dma_burst_advice(struct pci_dev *pdev,
					enum pci_dma_burst_strategy *strat,
					unsigned long *strategy_parameter)
{
	*strat = PCI_DMA_BURST_INFINITY;
	*strategy_parameter = ~0UL;
}
#else
static inline void early_quirks(void) { }
#endif

extern void pci_iommu_alloc(void);

#endif  /* __KERNEL__ */

#ifdef CONFIG_X86_32
# include "pci_32.h"
#else
# include "pci_64.h"
#endif

/* implement the pci_ DMA API in terms of the generic device dma_ one */
#include <asm-generic/pci-dma-compat.h>

/* generic pci stuff */
#include <asm-generic/pci.h>

#ifdef CONFIG_NUMA
/* Returns the node based on pci bus */
static inline int __pcibus_to_node(const struct pci_bus *bus)
{
	const struct pci_sysdata *sd = bus->sysdata;

	return sd->node;
}

static inline cpumask_t __pcibus_to_cpumask(struct pci_bus *bus)
{
	return node_to_cpumask(__pcibus_to_node(bus));
}

static inline const struct cpumask *
cpumask_of_pcibus(const struct pci_bus *bus)
{
	return cpumask_of_node(__pcibus_to_node(bus));
}
#endif

#endif /* _ASM_X86_PCI_H */
