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

#ifndef _FIB_LOOKUP_H
#define _FIB_LOOKUP_H

#include <linux/types.h>
#include <linux/list.h>
#include <net/ip_fib.h>

struct fib_alias {
	struct list_head	fa_list;
	struct fib_info		*fa_info;
	u8			fa_tos;
	u8			fa_type;
	u8			fa_scope;
	u8			fa_state;
#ifdef CONFIG_IP_FIB_TRIE
	struct rcu_head		rcu;
#endif
};

#define FA_S_ACCESSED	0x01

/* Exported by fib_semantics.c */
extern int fib_semantic_match(struct list_head *head,
			      const struct flowi *flp,
			      struct fib_result *res, __be32 zone, __be32 mask,
				int prefixlen);
extern void fib_release_info(struct fib_info *);
extern struct fib_info *fib_create_info(struct fib_config *cfg);
extern int fib_nh_match(struct fib_config *cfg, struct fib_info *fi);
extern int fib_dump_info(struct sk_buff *skb, u32 pid, u32 seq, int event,
			 u32 tb_id, u8 type, u8 scope, __be32 dst,
			 int dst_len, u8 tos, struct fib_info *fi,
			 unsigned int);
extern void rtmsg_fib(int event, __be32 key, struct fib_alias *fa,
		      int dst_len, u32 tb_id, struct nl_info *info,
		      unsigned int nlm_flags);
extern struct fib_alias *fib_find_alias(struct list_head *fah,
					u8 tos, u32 prio);
extern int fib_detect_death(struct fib_info *fi, int order,
			    struct fib_info **last_resort,
			    int *last_idx, int dflt);

static inline void fib_result_assign(struct fib_result *res,
				     struct fib_info *fi)
{
	if (res->fi != NULL)
		fib_info_put(res->fi);
	res->fi = fi;
	if (fi != NULL)
		atomic_inc(&fi->fib_clntref);
}

#endif /* _FIB_LOOKUP_H */
