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

#ifndef _ASM_X86_PCI_32_H
#define _ASM_X86_PCI_32_H


#ifdef __KERNEL__


/* Dynamic DMA mapping stuff.
 * i386 has everything mapped statically.
 */

struct pci_dev;

/* The PCI address space does equal the physical memory
 * address space.  The networking and block device layers use
 * this boolean for bounce buffer decisions.
 */
#define PCI_DMA_BUS_IS_PHYS	(1)

/* pci_unmap_{page,single} is a nop so... */
#define DECLARE_PCI_UNMAP_ADDR(ADDR_NAME)	dma_addr_t ADDR_NAME[0];
#define DECLARE_PCI_UNMAP_LEN(LEN_NAME)	unsigned LEN_NAME[0];
#define pci_unmap_addr(PTR, ADDR_NAME)	sizeof((PTR)->ADDR_NAME)
#define pci_unmap_addr_set(PTR, ADDR_NAME, VAL) \
	do { break; } while (pci_unmap_addr(PTR, ADDR_NAME))
#define pci_unmap_len(PTR, LEN_NAME)		sizeof((PTR)->LEN_NAME)
#define pci_unmap_len_set(PTR, LEN_NAME, VAL) \
	do { break; } while (pci_unmap_len(PTR, LEN_NAME))


#endif /* __KERNEL__ */


#endif /* _ASM_X86_PCI_32_H */
