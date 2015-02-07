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

#ifndef _DMA_ATTR_H
#define _DMA_ATTR_H

#include <linux/bitmap.h>
#include <linux/bitops.h>
#include <linux/bug.h>

/**
 * an enum dma_attr represents an attribute associated with a DMA
 * mapping. The semantics of each attribute should be defined in
 * Documentation/DMA-attributes.txt.
 */
enum dma_attr {
	DMA_ATTR_WRITE_BARRIER,
	DMA_ATTR_WEAK_ORDERING,
	DMA_ATTR_MAX,
};

#define __DMA_ATTRS_LONGS BITS_TO_LONGS(DMA_ATTR_MAX)

/**
 * struct dma_attrs - an opaque container for DMA attributes
 * @flags - bitmask representing a collection of enum dma_attr
 */
struct dma_attrs {
	unsigned long flags[__DMA_ATTRS_LONGS];
};

#define DEFINE_DMA_ATTRS(x) 					\
	struct dma_attrs x = {					\
		.flags = { [0 ... __DMA_ATTRS_LONGS-1] = 0 },	\
	}

static inline void init_dma_attrs(struct dma_attrs *attrs)
{
	bitmap_zero(attrs->flags, __DMA_ATTRS_LONGS);
}

#ifdef CONFIG_HAVE_DMA_ATTRS
/**
 * dma_set_attr - set a specific attribute
 * @attr: attribute to set
 * @attrs: struct dma_attrs (may be NULL)
 */
static inline void dma_set_attr(enum dma_attr attr, struct dma_attrs *attrs)
{
	if (attrs == NULL)
		return;
	BUG_ON(attr >= DMA_ATTR_MAX);
	__set_bit(attr, attrs->flags);
}

/**
 * dma_get_attr - check for a specific attribute
 * @attr: attribute to set
 * @attrs: struct dma_attrs (may be NULL)
 */
static inline int dma_get_attr(enum dma_attr attr, struct dma_attrs *attrs)
{
	if (attrs == NULL)
		return 0;
	BUG_ON(attr >= DMA_ATTR_MAX);
	return test_bit(attr, attrs->flags);
}
#else /* !CONFIG_HAVE_DMA_ATTRS */
static inline void dma_set_attr(enum dma_attr attr, struct dma_attrs *attrs)
{
}

static inline int dma_get_attr(enum dma_attr attr, struct dma_attrs *attrs)
{
	return 0;
}
#endif /* CONFIG_HAVE_DMA_ATTRS */
#endif /* _DMA_ATTR_H */
