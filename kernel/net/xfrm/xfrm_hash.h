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

#ifndef _XFRM_HASH_H
#define _XFRM_HASH_H

#include <linux/xfrm.h>
#include <linux/socket.h>

static inline unsigned int __xfrm4_addr_hash(xfrm_address_t *addr)
{
	return ntohl(addr->a4);
}

static inline unsigned int __xfrm6_addr_hash(xfrm_address_t *addr)
{
	return ntohl(addr->a6[2] ^ addr->a6[3]);
}

static inline unsigned int __xfrm4_daddr_saddr_hash(xfrm_address_t *daddr, xfrm_address_t *saddr)
{
	return ntohl(daddr->a4 ^ saddr->a4);
}

static inline unsigned int __xfrm6_daddr_saddr_hash(xfrm_address_t *daddr, xfrm_address_t *saddr)
{
	return ntohl(daddr->a6[2] ^ daddr->a6[3] ^
		     saddr->a6[2] ^ saddr->a6[3]);
}

static inline unsigned int __xfrm_dst_hash(xfrm_address_t *daddr, xfrm_address_t *saddr,
					   u32 reqid, unsigned short family,
					   unsigned int hmask)
{
	unsigned int h = family ^ reqid;
	switch (family) {
	case AF_INET:
		h ^= __xfrm4_daddr_saddr_hash(daddr, saddr);
		break;
	case AF_INET6:
		h ^= __xfrm6_daddr_saddr_hash(daddr, saddr);
		break;
	}
	return (h ^ (h >> 16)) & hmask;
}

static inline unsigned __xfrm_src_hash(xfrm_address_t *daddr,
				       xfrm_address_t *saddr,
				       unsigned short family,
				       unsigned int hmask)
{
	unsigned int h = family;
	switch (family) {
	case AF_INET:
		h ^= __xfrm4_daddr_saddr_hash(daddr, saddr);
		break;
	case AF_INET6:
		h ^= __xfrm6_daddr_saddr_hash(daddr, saddr);
		break;
	};
	return (h ^ (h >> 16)) & hmask;
}

static inline unsigned int
__xfrm_spi_hash(xfrm_address_t *daddr, __be32 spi, u8 proto, unsigned short family,
		unsigned int hmask)
{
	unsigned int h = (__force u32)spi ^ proto;
	switch (family) {
	case AF_INET:
		h ^= __xfrm4_addr_hash(daddr);
		break;
	case AF_INET6:
		h ^= __xfrm6_addr_hash(daddr);
		break;
	}
	return (h ^ (h >> 10) ^ (h >> 20)) & hmask;
}

static inline unsigned int __idx_hash(u32 index, unsigned int hmask)
{
	return (index ^ (index >> 8)) & hmask;
}

static inline unsigned int __sel_hash(struct xfrm_selector *sel, unsigned short family, unsigned int hmask)
{
	xfrm_address_t *daddr = &sel->daddr;
	xfrm_address_t *saddr = &sel->saddr;
	unsigned int h = 0;

	switch (family) {
	case AF_INET:
		if (sel->prefixlen_d != 32 ||
		    sel->prefixlen_s != 32)
			return hmask + 1;

		h = __xfrm4_daddr_saddr_hash(daddr, saddr);
		break;

	case AF_INET6:
		if (sel->prefixlen_d != 128 ||
		    sel->prefixlen_s != 128)
			return hmask + 1;

		h = __xfrm6_daddr_saddr_hash(daddr, saddr);
		break;
	};
	h ^= (h >> 16);
	return h & hmask;
}

static inline unsigned int __addr_hash(xfrm_address_t *daddr, xfrm_address_t *saddr, unsigned short family, unsigned int hmask)
{
	unsigned int h = 0;

	switch (family) {
	case AF_INET:
		h = __xfrm4_daddr_saddr_hash(daddr, saddr);
		break;

	case AF_INET6:
		h = __xfrm6_daddr_saddr_hash(daddr, saddr);
		break;
	};
	h ^= (h >> 16);
	return h & hmask;
}

extern struct hlist_head *xfrm_hash_alloc(unsigned int sz);
extern void xfrm_hash_free(struct hlist_head *n, unsigned int sz);

#endif /* _XFRM_HASH_H */
