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
 * Copyright (c) 2003-2005 Silicon Graphics, Inc. All rights reserved.
 */

#ifndef _ASM_IA64_SN_CE_PROVIDER_H
#define _ASM_IA64_SN_CE_PROVIDER_H

#include <asm/sn/pcibus_provider_defs.h>
#include <asm/sn/tioce.h>

/*
 * Common TIOCE structure shared between the prom and kernel
 *
 * DO NOT CHANGE THIS STRUCT WITHOUT MAKING CORRESPONDING CHANGES TO THE
 * PROM VERSION.
 */
struct tioce_common {
	struct pcibus_bussoft	ce_pcibus;	/* common pciio header */

	u32		ce_rev;
	u64		ce_kernel_private;
	u64		ce_prom_private;
};

struct tioce_kernel {
	struct tioce_common	*ce_common;
	spinlock_t		ce_lock;
	struct list_head	ce_dmamap_list;

	u64		ce_ate40_shadow[TIOCE_NUM_M40_ATES];
	u64		ce_ate3240_shadow[TIOCE_NUM_M3240_ATES];
	u32		ce_ate3240_pagesize;

	u8			ce_port1_secondary;

	/* per-port resources */
	struct {
		int 		dirmap_refcnt;
		u64	dirmap_shadow;
	} ce_port[TIOCE_NUM_PORTS];
};

struct tioce_dmamap {
	struct list_head	ce_dmamap_list;	/* headed by tioce_kernel */
	u32		refcnt;

	u64		nbytes;		/* # bytes mapped */

	u64		ct_start;	/* coretalk start address */
	u64		pci_start;	/* bus start address */

	u64		__iomem *ate_hw;/* hw ptr of first ate in map */
	u64		*ate_shadow;	/* shadow ptr of firat ate */
	u16		ate_count;	/* # ate's in the map */
};

extern int tioce_init_provider(void);

#endif  /* __ASM_IA64_SN_CE_PROVIDER_H */
