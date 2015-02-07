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
 * Copyright (c) 1997, 1999 by Ralf Baechle
 * Copyright (c) 1999 Silicon Graphics, Inc.
 */
#ifndef _ASM_BCACHE_H
#define _ASM_BCACHE_H


/* Some R4000 / R4400 / R4600 / R5000 machines may have a non-dma-coherent,
   chipset implemented caches.  On machines with other CPUs the CPU does the
   cache thing itself. */
struct bcache_ops {
	void (*bc_enable)(void);
	void (*bc_disable)(void);
	void (*bc_wback_inv)(unsigned long page, unsigned long size);
	void (*bc_inv)(unsigned long page, unsigned long size);
};

extern void indy_sc_init(void);

#ifdef CONFIG_BOARD_SCACHE

extern struct bcache_ops *bcops;

static inline void bc_enable(void)
{
	bcops->bc_enable();
}

static inline void bc_disable(void)
{
	bcops->bc_disable();
}

static inline void bc_wback_inv(unsigned long page, unsigned long size)
{
	bcops->bc_wback_inv(page, size);
}

static inline void bc_inv(unsigned long page, unsigned long size)
{
	bcops->bc_inv(page, size);
}

#else /* !defined(CONFIG_BOARD_SCACHE) */

/* Not R4000 / R4400 / R4600 / R5000.  */

#define bc_enable() do { } while (0)
#define bc_disable() do { } while (0)
#define bc_wback_inv(page, size) do { } while (0)
#define bc_inv(page, size) do { } while (0)

#endif /* !defined(CONFIG_BOARD_SCACHE) */

#endif /* _ASM_BCACHE_H */
