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

#ifndef _ASM_X86_VISWS_LITHIUM_H
#define _ASM_X86_VISWS_LITHIUM_H

#include <asm/fixmap.h>

/*
 * Lithium is the SGI Visual Workstation I/O ASIC
 */

#define	LI_PCI_A_PHYS		0xfc000000	/* Enet is dev 3 */
#define	LI_PCI_B_PHYS		0xfd000000	/* PIIX4 is here */

/* see set_fixmap() and asm/fixmap.h */
#define LI_PCIA_VADDR   (fix_to_virt(FIX_LI_PCIA))
#define LI_PCIB_VADDR   (fix_to_virt(FIX_LI_PCIB))

/* Not a standard PCI? (not in linux/pci.h) */
#define	LI_PCI_BUSNUM	0x44			/* lo8: primary, hi8: sub */
#define LI_PCI_INTEN    0x46

/* LI_PCI_INTENT bits */
#define	LI_INTA_0	0x0001
#define	LI_INTA_1	0x0002
#define	LI_INTA_2	0x0004
#define	LI_INTA_3	0x0008
#define	LI_INTA_4	0x0010
#define	LI_INTB		0x0020
#define	LI_INTC		0x0040
#define	LI_INTD		0x0080

/* More special purpose macros... */
static inline void li_pcia_write16(unsigned long reg, unsigned short v)
{
	*((volatile unsigned short *)(LI_PCIA_VADDR+reg))=v;
}

static inline unsigned short li_pcia_read16(unsigned long reg)
{
	 return *((volatile unsigned short *)(LI_PCIA_VADDR+reg));
}

static inline void li_pcib_write16(unsigned long reg, unsigned short v)
{
	*((volatile unsigned short *)(LI_PCIB_VADDR+reg))=v;
}

static inline unsigned short li_pcib_read16(unsigned long reg)
{
	return *((volatile unsigned short *)(LI_PCIB_VADDR+reg));
}

#endif /* _ASM_X86_VISWS_LITHIUM_H */

