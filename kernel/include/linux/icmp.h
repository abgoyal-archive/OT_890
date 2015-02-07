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
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Definitions for the ICMP protocol.
 *
 * Version:	@(#)icmp.h	1.0.3	04/28/93
 *
 * Author:	Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */
#ifndef _LINUX_ICMP_H
#define	_LINUX_ICMP_H

#include <linux/types.h>

#define ICMP_ECHOREPLY		0	/* Echo Reply			*/
#define ICMP_DEST_UNREACH	3	/* Destination Unreachable	*/
#define ICMP_SOURCE_QUENCH	4	/* Source Quench		*/
#define ICMP_REDIRECT		5	/* Redirect (change route)	*/
#define ICMP_ECHO		8	/* Echo Request			*/
#define ICMP_TIME_EXCEEDED	11	/* Time Exceeded		*/
#define ICMP_PARAMETERPROB	12	/* Parameter Problem		*/
#define ICMP_TIMESTAMP		13	/* Timestamp Request		*/
#define ICMP_TIMESTAMPREPLY	14	/* Timestamp Reply		*/
#define ICMP_INFO_REQUEST	15	/* Information Request		*/
#define ICMP_INFO_REPLY		16	/* Information Reply		*/
#define ICMP_ADDRESS		17	/* Address Mask Request		*/
#define ICMP_ADDRESSREPLY	18	/* Address Mask Reply		*/
#define NR_ICMP_TYPES		18


/* Codes for UNREACH. */
#define ICMP_NET_UNREACH	0	/* Network Unreachable		*/
#define ICMP_HOST_UNREACH	1	/* Host Unreachable		*/
#define ICMP_PROT_UNREACH	2	/* Protocol Unreachable		*/
#define ICMP_PORT_UNREACH	3	/* Port Unreachable		*/
#define ICMP_FRAG_NEEDED	4	/* Fragmentation Needed/DF set	*/
#define ICMP_SR_FAILED		5	/* Source Route failed		*/
#define ICMP_NET_UNKNOWN	6
#define ICMP_HOST_UNKNOWN	7
#define ICMP_HOST_ISOLATED	8
#define ICMP_NET_ANO		9
#define ICMP_HOST_ANO		10
#define ICMP_NET_UNR_TOS	11
#define ICMP_HOST_UNR_TOS	12
#define ICMP_PKT_FILTERED	13	/* Packet filtered */
#define ICMP_PREC_VIOLATION	14	/* Precedence violation */
#define ICMP_PREC_CUTOFF	15	/* Precedence cut off */
#define NR_ICMP_UNREACH		15	/* instead of hardcoding immediate value */

/* Codes for REDIRECT. */
#define ICMP_REDIR_NET		0	/* Redirect Net			*/
#define ICMP_REDIR_HOST		1	/* Redirect Host		*/
#define ICMP_REDIR_NETTOS	2	/* Redirect Net for TOS		*/
#define ICMP_REDIR_HOSTTOS	3	/* Redirect Host for TOS	*/

/* Codes for TIME_EXCEEDED. */
#define ICMP_EXC_TTL		0	/* TTL count exceeded		*/
#define ICMP_EXC_FRAGTIME	1	/* Fragment Reass time exceeded	*/


struct icmphdr {
  __u8		type;
  __u8		code;
  __sum16	checksum;
  union {
	struct {
		__be16	id;
		__be16	sequence;
	} echo;
	__be32	gateway;
	struct {
		__be16	__unused;
		__be16	mtu;
	} frag;
  } un;
};

#ifdef __KERNEL__
#include <linux/skbuff.h>

static inline struct icmphdr *icmp_hdr(const struct sk_buff *skb)
{
	return (struct icmphdr *)skb_transport_header(skb);
}
#endif

/*
 *	constants for (set|get)sockopt
 */

#define ICMP_FILTER			1

struct icmp_filter {
	__u32		data;
};


#endif	/* _LINUX_ICMP_H */
