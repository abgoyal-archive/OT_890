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
 * IPv6 library code, needed by static components when full IPv6 support is
 * not configured or static.
 */
#include <net/ipv6.h>

/*
 * find out if nexthdr is a well-known extension header or a protocol
 */

int ipv6_ext_hdr(u8 nexthdr)
{
	/*
	 * find out if nexthdr is an extension header or a protocol
	 */
	return ( (nexthdr == NEXTHDR_HOP)	||
		 (nexthdr == NEXTHDR_ROUTING)	||
		 (nexthdr == NEXTHDR_FRAGMENT)	||
		 (nexthdr == NEXTHDR_AUTH)	||
		 (nexthdr == NEXTHDR_NONE)	||
		 (nexthdr == NEXTHDR_DEST) );
}

/*
 * Skip any extension headers. This is used by the ICMP module.
 *
 * Note that strictly speaking this conflicts with RFC 2460 4.0:
 * ...The contents and semantics of each extension header determine whether
 * or not to proceed to the next header.  Therefore, extension headers must
 * be processed strictly in the order they appear in the packet; a
 * receiver must not, for example, scan through a packet looking for a
 * particular kind of extension header and process that header prior to
 * processing all preceding ones.
 *
 * We do exactly this. This is a protocol bug. We can't decide after a
 * seeing an unknown discard-with-error flavour TLV option if it's a
 * ICMP error message or not (errors should never be send in reply to
 * ICMP error messages).
 *
 * But I see no other way to do this. This might need to be reexamined
 * when Linux implements ESP (and maybe AUTH) headers.
 * --AK
 *
 * This function parses (probably truncated) exthdr set "hdr".
 * "nexthdrp" initially points to some place,
 * where type of the first header can be found.
 *
 * It skips all well-known exthdrs, and returns pointer to the start
 * of unparsable area i.e. the first header with unknown type.
 * If it is not NULL *nexthdr is updated by type/protocol of this header.
 *
 * NOTES: - if packet terminated with NEXTHDR_NONE it returns NULL.
 *        - it may return pointer pointing beyond end of packet,
 *	    if the last recognized header is truncated in the middle.
 *        - if packet is truncated, so that all parsed headers are skipped,
 *	    it returns NULL.
 *	  - First fragment header is skipped, not-first ones
 *	    are considered as unparsable.
 *	  - ESP is unparsable for now and considered like
 *	    normal payload protocol.
 *	  - Note also special handling of AUTH header. Thanks to IPsec wizards.
 *
 * --ANK (980726)
 */

int ipv6_skip_exthdr(const struct sk_buff *skb, int start, u8 *nexthdrp)
{
	u8 nexthdr = *nexthdrp;

	while (ipv6_ext_hdr(nexthdr)) {
		struct ipv6_opt_hdr _hdr, *hp;
		int hdrlen;

		if (nexthdr == NEXTHDR_NONE)
			return -1;
		hp = skb_header_pointer(skb, start, sizeof(_hdr), &_hdr);
		if (hp == NULL)
			return -1;
		if (nexthdr == NEXTHDR_FRAGMENT) {
			__be16 _frag_off, *fp;
			fp = skb_header_pointer(skb,
						start+offsetof(struct frag_hdr,
							       frag_off),
						sizeof(_frag_off),
						&_frag_off);
			if (fp == NULL)
				return -1;

			if (ntohs(*fp) & ~0x7)
				break;
			hdrlen = 8;
		} else if (nexthdr == NEXTHDR_AUTH)
			hdrlen = (hp->hdrlen+2)<<2;
		else
			hdrlen = ipv6_optlen(hp);

		nexthdr = hp->nexthdr;
		start += hdrlen;
	}

	*nexthdrp = nexthdr;
	return start;
}

EXPORT_SYMBOL(ipv6_ext_hdr);
EXPORT_SYMBOL(ipv6_skip_exthdr);
