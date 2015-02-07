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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 2005 Silicon Graphics, Inc. All rights reserved.
 */

#ifndef _ASM_IA64_SN_TIO_TIOCX_H
#define _ASM_IA64_SN_TIO_TIOCX_H

#ifdef __KERNEL__

struct cx_id_s {
	unsigned int part_num;
	unsigned int mfg_num;
	int nasid;
};

struct cx_dev {
	struct cx_id_s cx_id;
	int bt;				/* board/blade type */
	void *soft;			/* driver specific */
	struct hubdev_info *hubdev;
	struct device dev;
	struct cx_drv *driver;
};

struct cx_device_id {
	unsigned int part_num;
	unsigned int mfg_num;
};

struct cx_drv {
	char *name;
	const struct cx_device_id *id_table;
	struct device_driver driver;
	int (*probe) (struct cx_dev * dev, const struct cx_device_id * id);
	int (*remove) (struct cx_dev * dev);
};

/* create DMA address by stripping AS bits */
#define TIOCX_DMA_ADDR(a) (u64)((u64)(a) & 0xffffcfffffffffUL)

#define TIOCX_TO_TIOCX_DMA_ADDR(a) (u64)(((u64)(a) & 0xfffffffff) |  \
                                  ((((u64)(a)) & 0xffffc000000000UL) <<2))

#define TIO_CE_ASIC_PARTNUM 0xce00
#define TIOCX_CORELET 3

/* These are taken from tio_mmr_as.h */
#define TIO_ICE_FRZ_CFG               TIO_MMR_ADDR_MOD(0x00000000b0008100UL)
#define TIO_ICE_PMI_TX_CFG            TIO_MMR_ADDR_MOD(0x00000000b000b100UL)
#define TIO_ICE_PMI_TX_DYN_CREDIT_STAT_CB3 TIO_MMR_ADDR_MOD(0x00000000b000be18UL)
#define TIO_ICE_PMI_TX_DYN_CREDIT_STAT_CB3_CREDIT_CNT_MASK 0x000000000000000fUL

#define to_cx_dev(n) container_of(n, struct cx_dev, dev)
#define to_cx_driver(drv) container_of(drv, struct cx_drv, driver)

extern struct sn_irq_info *tiocx_irq_alloc(nasid_t, int, int, nasid_t, int);
extern void tiocx_irq_free(struct sn_irq_info *);
extern int cx_device_unregister(struct cx_dev *);
extern int cx_device_register(nasid_t, int, int, struct hubdev_info *, int);
extern int cx_driver_unregister(struct cx_drv *);
extern int cx_driver_register(struct cx_drv *);
extern u64 tiocx_dma_addr(u64 addr);
extern u64 tiocx_swin_base(int nasid);
extern void tiocx_mmr_store(int nasid, u64 offset, u64 value);
extern u64 tiocx_mmr_load(int nasid, u64 offset);

#endif				//  __KERNEL__
#endif				// _ASM_IA64_SN_TIO_TIOCX__
