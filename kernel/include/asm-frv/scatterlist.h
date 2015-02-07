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

#ifndef _ASM_SCATTERLIST_H
#define _ASM_SCATTERLIST_H

#include <asm/types.h>

/*
 * Drivers must set either ->address or (preferred) page and ->offset
 * to indicate where data must be transferred to/from.
 *
 * Using page is recommended since it handles highmem data as well as
 * low mem. ->address is restricted to data which has a virtual mapping, and
 * it will go away in the future. Updating to page can be automated very
 * easily -- something like
 *
 * sg->address = some_ptr;
 *
 * can be rewritten as
 *
 * sg_set_buf(sg, some_ptr, length);
 *
 * and that's it. There's no excuse for not highmem enabling YOUR driver. /jens
 */
struct scatterlist {
#ifdef CONFIG_DEBUG_SG
	unsigned long	sg_magic;
#endif
	unsigned long	page_link;
	unsigned int	offset;		/* for highmem, page offset */

	dma_addr_t	dma_address;
	unsigned int	length;
};

/*
 * These macros should be used after a pci_map_sg call has been done
 * to get bus addresses of each of the SG entries and their lengths.
 * You should only work with the number of sg entries pci_map_sg
 * returns, or alternatively stop on the first sg_dma_len(sg) which
 * is 0.
 */
#define sg_dma_address(sg)	((sg)->dma_address)
#define sg_dma_len(sg)		((sg)->length)

#define ISA_DMA_THRESHOLD (0xffffffffUL)

#endif /* !_ASM_SCATTERLIST_H */
