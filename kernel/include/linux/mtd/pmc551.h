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
 * PMC551 PCI Mezzanine Ram Device
 *
 * Author:
 *       Mark Ferrell
 *       Copyright 1999,2000 Nortel Networks
 *
 * License:
 *	 As part of this driver was derrived from the slram.c driver it falls
 *	 under the same license, which is GNU General Public License v2
 */

#ifndef __MTD_PMC551_H__
#define __MTD_PMC551_H__

#include <linux/mtd/mtd.h>

#define PMC551_VERSION \
       "Ramix PMC551 PCI Mezzanine Ram Driver. (C) 1999,2000 Nortel Networks.\n"

/*
 * Our personal and private information
 */
struct mypriv {
        struct pci_dev *dev;
        u_char *start;
        u32    base_map0;
        u32    curr_map0;
        u32    asize;
	struct mtd_info *nextpmc551;
};

/*
 * Function Prototypes
 */
static int pmc551_erase(struct mtd_info *, struct erase_info *);
static void pmc551_unpoint(struct mtd_info *, loff_t, size_t);
static int pmc551_point(struct mtd_info *mtd, loff_t from, size_t len,
		size_t *retlen, void **virt, resource_size_t *phys);
static int pmc551_read(struct mtd_info *, loff_t, size_t, size_t *, u_char *);
static int pmc551_write(struct mtd_info *, loff_t, size_t, size_t *, const u_char *);


/*
 * Define the PCI ID's if the kernel doesn't define them for us
 */
#ifndef PCI_VENDOR_ID_V3_SEMI
#define PCI_VENDOR_ID_V3_SEMI             0x11b0
#endif

#ifndef PCI_DEVICE_ID_V3_SEMI_V370PDC
#define PCI_DEVICE_ID_V3_SEMI_V370PDC     0x0200
#endif


#define PMC551_PCI_MEM_MAP0	0x50
#define PMC551_PCI_MEM_MAP1	0x54
#define PMC551_PCI_MEM_MAP_MAP_ADDR_MASK	0x3ff00000
#define PMC551_PCI_MEM_MAP_APERTURE_MASK	0x000000f0
#define PMC551_PCI_MEM_MAP_REG_EN		0x00000002
#define PMC551_PCI_MEM_MAP_ENABLE		0x00000001

#define PMC551_SDRAM_MA		0x60
#define PMC551_SDRAM_CMD	0x62
#define PMC551_DRAM_CFG		0x64
#define PMC551_SYS_CTRL_REG	0x78

#define PMC551_DRAM_BLK0	0x68
#define PMC551_DRAM_BLK1	0x6c
#define PMC551_DRAM_BLK2	0x70
#define PMC551_DRAM_BLK3	0x74
#define PMC551_DRAM_BLK_GET_SIZE(x) (524288<<((x>>4)&0x0f))
#define PMC551_DRAM_BLK_SET_COL_MUX(x,v) (((x) & ~0x00007000) | (((v) & 0x7) << 12))
#define PMC551_DRAM_BLK_SET_ROW_MUX(x,v) (((x) & ~0x00000f00) | (((v) & 0xf) << 8))


#endif /* __MTD_PMC551_H__ */

