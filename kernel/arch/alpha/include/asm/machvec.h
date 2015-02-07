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

#ifndef __ALPHA_MACHVEC_H
#define __ALPHA_MACHVEC_H 1

#include <linux/types.h>

/*
 *	This file gets pulled in by asm/io.h from user space. We don't
 *	want most of this escaping.
 */
 
#ifdef __KERNEL__

/* The following structure vectors all of the I/O and IRQ manipulation
   from the generic kernel to the hardware specific backend.  */

struct task_struct;
struct mm_struct;
struct vm_area_struct;
struct linux_hose_info;
struct pci_dev;
struct pci_ops;
struct pci_controller;
struct _alpha_agp_info;
struct rtc_time;

struct alpha_machine_vector
{
	/* This "belongs" down below with the rest of the runtime
	   variables, but it is convenient for entry.S if these 
	   two slots are at the beginning of the struct.  */
	unsigned long hae_cache;
	unsigned long *hae_register;

	int nr_irqs;
	int rtc_port;
	unsigned int max_asn;
	unsigned long max_isa_dma_address;
	unsigned long irq_probe_mask;
	unsigned long iack_sc;
	unsigned long min_io_address;
	unsigned long min_mem_address;
	unsigned long pci_dac_offset;

	void (*mv_pci_tbi)(struct pci_controller *hose,
			   dma_addr_t start, dma_addr_t end);

	unsigned int (*mv_ioread8)(void __iomem *);
	unsigned int (*mv_ioread16)(void __iomem *);
	unsigned int (*mv_ioread32)(void __iomem *);

	void (*mv_iowrite8)(u8, void __iomem *);
	void (*mv_iowrite16)(u16, void __iomem *);
	void (*mv_iowrite32)(u32, void __iomem *);

	u8 (*mv_readb)(const volatile void __iomem *);
	u16 (*mv_readw)(const volatile void __iomem *);
	u32 (*mv_readl)(const volatile void __iomem *);
	u64 (*mv_readq)(const volatile void __iomem *);

	void (*mv_writeb)(u8, volatile void __iomem *);
	void (*mv_writew)(u16, volatile void __iomem *);
	void (*mv_writel)(u32, volatile void __iomem *);
	void (*mv_writeq)(u64, volatile void __iomem *);

	void __iomem *(*mv_ioportmap)(unsigned long);
	void __iomem *(*mv_ioremap)(unsigned long, unsigned long);
	void (*mv_iounmap)(volatile void __iomem *);
	int (*mv_is_ioaddr)(unsigned long);
	int (*mv_is_mmio)(const volatile void __iomem *);

	void (*mv_switch_mm)(struct mm_struct *, struct mm_struct *,
			     struct task_struct *);
	void (*mv_activate_mm)(struct mm_struct *, struct mm_struct *);

	void (*mv_flush_tlb_current)(struct mm_struct *);
	void (*mv_flush_tlb_current_page)(struct mm_struct * mm,
					  struct vm_area_struct *vma,
					  unsigned long addr);

	void (*update_irq_hw)(unsigned long, unsigned long, int);
	void (*ack_irq)(unsigned long);
	void (*device_interrupt)(unsigned long vector);
	void (*machine_check)(u64 vector, u64 la);

	void (*smp_callin)(void);
	void (*init_arch)(void);
	void (*init_irq)(void);
	void (*init_rtc)(void);
	void (*init_pci)(void);
	void (*kill_arch)(int);

	u8 (*pci_swizzle)(struct pci_dev *, u8 *);
	int (*pci_map_irq)(struct pci_dev *, u8, u8);
	struct pci_ops *pci_ops;

	struct _alpha_agp_info *(*agp_info)(void);

	unsigned int (*rtc_get_time)(struct rtc_time *);
	int (*rtc_set_time)(struct rtc_time *);

	const char *vector_name;

	/* NUMA information */
	int (*pa_to_nid)(unsigned long);
	int (*cpuid_to_nid)(int);
	unsigned long (*node_mem_start)(int);
	unsigned long (*node_mem_size)(int);

	/* System specific parameters.  */
	union {
	    struct {
		unsigned long gru_int_req_bits;
	    } cia;

	    struct {
		unsigned long gamma_bias;
	    } t2;

	    struct {
		unsigned int route_tab;
	    } sio;
	} sys;
};

extern struct alpha_machine_vector alpha_mv;

#ifdef CONFIG_ALPHA_GENERIC
extern int alpha_using_srm;
#else
#ifdef CONFIG_ALPHA_SRM
#define alpha_using_srm 1
#else
#define alpha_using_srm 0
#endif
#endif /* GENERIC */

#endif
#endif /* __ALPHA_MACHVEC_H */
