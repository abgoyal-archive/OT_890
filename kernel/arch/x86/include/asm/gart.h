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

#ifndef _ASM_X86_GART_H
#define _ASM_X86_GART_H

#include <asm/e820.h>

extern void set_up_gart_resume(u32, u32);

extern int fallback_aper_order;
extern int fallback_aper_force;
extern int fix_aperture;

/* PTE bits. */
#define GPTE_VALID	1
#define GPTE_COHERENT	2

/* Aperture control register bits. */
#define GARTEN		(1<<0)
#define DISGARTCPU	(1<<4)
#define DISGARTIO	(1<<5)

/* GART cache control register bits. */
#define INVGART		(1<<0)
#define GARTPTEERR	(1<<1)

/* K8 On-cpu GART registers */
#define AMD64_GARTAPERTURECTL	0x90
#define AMD64_GARTAPERTUREBASE	0x94
#define AMD64_GARTTABLEBASE	0x98
#define AMD64_GARTCACHECTL	0x9c
#define AMD64_GARTEN		(1<<0)

#ifdef CONFIG_GART_IOMMU
extern int gart_iommu_aperture;
extern int gart_iommu_aperture_allowed;
extern int gart_iommu_aperture_disabled;

extern void early_gart_iommu_check(void);
extern void gart_iommu_init(void);
extern void gart_iommu_shutdown(void);
extern void __init gart_parse_options(char *);
extern void gart_iommu_hole_init(void);

#else
#define gart_iommu_aperture            0
#define gart_iommu_aperture_allowed    0
#define gart_iommu_aperture_disabled   1

static inline void early_gart_iommu_check(void)
{
}
static inline void gart_iommu_init(void)
{
}
static inline void gart_iommu_shutdown(void)
{
}
static inline void gart_parse_options(char *options)
{
}
static inline void gart_iommu_hole_init(void)
{
}
#endif

extern int agp_amd64_init(void);

static inline void enable_gart_translation(struct pci_dev *dev, u64 addr)
{
	u32 tmp, ctl;

        /* address of the mappings table */
        addr >>= 12;
        tmp = (u32) addr<<4;
        tmp &= ~0xf;
        pci_write_config_dword(dev, AMD64_GARTTABLEBASE, tmp);

        /* Enable GART translation for this hammer. */
        pci_read_config_dword(dev, AMD64_GARTAPERTURECTL, &ctl);
        ctl |= GARTEN;
        ctl &= ~(DISGARTCPU | DISGARTIO);
        pci_write_config_dword(dev, AMD64_GARTAPERTURECTL, ctl);
}

static inline int aperture_valid(u64 aper_base, u32 aper_size, u32 min_size)
{
	if (!aper_base)
		return 0;

	if (aper_base + aper_size > 0x100000000ULL) {
		printk(KERN_INFO "Aperture beyond 4GB. Ignoring.\n");
		return 0;
	}
	if (e820_any_mapped(aper_base, aper_base + aper_size, E820_RAM)) {
		printk(KERN_INFO "Aperture pointing to e820 RAM. Ignoring.\n");
		return 0;
	}
	if (aper_size < min_size) {
		printk(KERN_INFO "Aperture too small (%d MB) than (%d MB)\n",
				 aper_size>>20, min_size>>20);
		return 0;
	}

	return 1;
}

#endif /* _ASM_X86_GART_H */
