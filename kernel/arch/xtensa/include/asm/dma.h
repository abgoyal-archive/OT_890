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
 * include/asm-xtensa/dma.h
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2003 - 2005 Tensilica Inc.
 */

#ifndef _XTENSA_DMA_H
#define _XTENSA_DMA_H

#include <asm/io.h>		/* need byte IO */

/*
 * This is only to be defined if we have PC-like DMA.
 * By default this is not true on an Xtensa processor,
 * however on boards with a PCI bus, such functionality
 * might be emulated externally.
 *
 * NOTE:  there still exists driver code that assumes
 * this is defined, eg. drivers/sound/soundcard.c (as of 2.4).
 */
#define MAX_DMA_CHANNELS	8

/*
 * The maximum virtual address to which DMA transfers
 * can be performed on this platform.
 *
 * NOTE: This is board (platform) specific, not processor-specific!
 *
 * NOTE: This assumes DMA transfers can only be performed on
 *	the section of physical memory contiguously mapped in virtual
 *	space for the kernel.  For the Xtensa architecture, this
 *	means the maximum possible size of this DMA area is
 *	the size of the statically mapped kernel segment
 *	(XCHAL_KSEG_{CACHED,BYPASS}_SIZE), ie. 128 MB.
 *
 * NOTE: When the entire KSEG area is DMA capable, we substract
 *	one from the max address so that the virt_to_phys() macro
 *	works correctly on the address (otherwise the address
 *	enters another area, and virt_to_phys() may not return
 *	the value desired).
 */

#define MAX_DMA_ADDRESS		(PAGE_OFFSET + XCHAL_KIO_SIZE - 1)


/* Reserve and release a DMA channel */
extern int request_dma(unsigned int dmanr, const char * device_id);
extern void free_dma(unsigned int dmanr);

#ifdef CONFIG_PCI
extern int isa_dma_bridge_buggy;
#else
#define isa_dma_bridge_buggy 	(0)
#endif


#endif
