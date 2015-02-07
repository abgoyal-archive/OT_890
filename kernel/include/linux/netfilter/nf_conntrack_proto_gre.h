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

#ifndef _CONNTRACK_PROTO_GRE_H
#define _CONNTRACK_PROTO_GRE_H
#include <asm/byteorder.h>

/* GRE PROTOCOL HEADER */

/* GRE Version field */
#define GRE_VERSION_1701	0x0
#define GRE_VERSION_PPTP	0x1

/* GRE Protocol field */
#define GRE_PROTOCOL_PPTP	0x880B

/* GRE Flags */
#define GRE_FLAG_C		0x80
#define GRE_FLAG_R		0x40
#define GRE_FLAG_K		0x20
#define GRE_FLAG_S		0x10
#define GRE_FLAG_A		0x80

#define GRE_IS_C(f)	((f)&GRE_FLAG_C)
#define GRE_IS_R(f)	((f)&GRE_FLAG_R)
#define GRE_IS_K(f)	((f)&GRE_FLAG_K)
#define GRE_IS_S(f)	((f)&GRE_FLAG_S)
#define GRE_IS_A(f)	((f)&GRE_FLAG_A)

/* GRE is a mess: Four different standards */
struct gre_hdr {
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u16	rec:3,
		srr:1,
		seq:1,
		key:1,
		routing:1,
		csum:1,
		version:3,
		reserved:4,
		ack:1;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u16	csum:1,
		routing:1,
		key:1,
		seq:1,
		srr:1,
		rec:3,
		ack:1,
		reserved:4,
		version:3;
#else
#error "Adjust your <asm/byteorder.h> defines"
#endif
	__be16	protocol;
};

/* modified GRE header for PPTP */
struct gre_hdr_pptp {
	__u8   flags;		/* bitfield */
	__u8   version;		/* should be GRE_VERSION_PPTP */
	__be16 protocol;	/* should be GRE_PROTOCOL_PPTP */
	__be16 payload_len;	/* size of ppp payload, not inc. gre header */
	__be16 call_id;		/* peer's call_id for this session */
	__be32 seq;		/* sequence number.  Present if S==1 */
	__be32 ack;		/* seq number of highest packet recieved by */
				/*  sender in this session */
};

struct nf_ct_gre {
	unsigned int stream_timeout;
	unsigned int timeout;
};

#ifdef __KERNEL__
#include <net/netfilter/nf_conntrack_tuple.h>

struct nf_conn;

/* structure for original <-> reply keymap */
struct nf_ct_gre_keymap {
	struct list_head list;
	struct nf_conntrack_tuple tuple;
};

/* add new tuple->key_reply pair to keymap */
int nf_ct_gre_keymap_add(struct nf_conn *ct, enum ip_conntrack_dir dir,
			 struct nf_conntrack_tuple *t);

/* delete keymap entries */
void nf_ct_gre_keymap_destroy(struct nf_conn *ct);

extern void nf_ct_gre_keymap_flush(struct net *net);
extern void nf_nat_need_gre(void);

#endif /* __KERNEL__ */
#endif /* _CONNTRACK_PROTO_GRE_H */
