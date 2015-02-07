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

#ifndef _NF_NAT_HELPER_H
#define _NF_NAT_HELPER_H
/* NAT protocol helper routines. */

#include <net/netfilter/nf_conntrack.h>

struct sk_buff;

/* These return true or false. */
extern int nf_nat_mangle_tcp_packet(struct sk_buff *skb,
				    struct nf_conn *ct,
				    enum ip_conntrack_info ctinfo,
				    unsigned int match_offset,
				    unsigned int match_len,
				    const char *rep_buffer,
				    unsigned int rep_len);
extern int nf_nat_mangle_udp_packet(struct sk_buff *skb,
				    struct nf_conn *ct,
				    enum ip_conntrack_info ctinfo,
				    unsigned int match_offset,
				    unsigned int match_len,
				    const char *rep_buffer,
				    unsigned int rep_len);
extern int nf_nat_seq_adjust(struct sk_buff *skb,
			     struct nf_conn *ct,
			     enum ip_conntrack_info ctinfo);
extern int (*nf_nat_seq_adjust_hook)(struct sk_buff *skb,
				     struct nf_conn *ct,
				     enum ip_conntrack_info ctinfo);

/* Setup NAT on this expected conntrack so it follows master, but goes
 * to port ct->master->saved_proto. */
extern void nf_nat_follow_master(struct nf_conn *ct,
				 struct nf_conntrack_expect *this);
#endif
