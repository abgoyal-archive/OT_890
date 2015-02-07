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

#ifndef _PSYCHO_COMMON_H
#define _PSYCHO_COMMON_H

/* U2P Programmer's Manual, page 13-55, configuration space
 * address format:
 * 
 *  32             24 23 16 15    11 10       8 7   2  1 0
 * ---------------------------------------------------------
 * |0 0 0 0 0 0 0 0 1| bus | device | function | reg | 0 0 |
 * ---------------------------------------------------------
 */
#define PSYCHO_CONFIG_BASE(PBM)	\
	((PBM)->config_space | (1UL << 24))
#define PSYCHO_CONFIG_ENCODE(BUS, DEVFN, REG)	\
	(((unsigned long)(BUS)   << 16) |	\
	 ((unsigned long)(DEVFN) << 8)  |	\
	 ((unsigned long)(REG)))

static inline void *psycho_pci_config_mkaddr(struct pci_pbm_info *pbm,
					     unsigned char bus,
					     unsigned int devfn,
					     int where)
{
	return (void *)
		(PSYCHO_CONFIG_BASE(pbm) |
		 PSYCHO_CONFIG_ENCODE(bus, devfn, where));
}

enum psycho_error_type {
	UE_ERR, CE_ERR, PCI_ERR
};

extern void psycho_check_iommu_error(struct pci_pbm_info *pbm,
				     unsigned long afsr,
				     unsigned long afar,
				     enum psycho_error_type type);

extern irqreturn_t psycho_pcierr_intr(int irq, void *dev_id);

extern int psycho_iommu_init(struct pci_pbm_info *pbm, int tsbsize,
			     u32 dvma_offset, u32 dma_mask,
			     unsigned long write_complete_offset);

extern void psycho_pbm_init_common(struct pci_pbm_info *pbm,
				   struct of_device *op,
				   const char *chip_name, int chip_type);

#endif /* _PSYCHO_COMMON_H */
