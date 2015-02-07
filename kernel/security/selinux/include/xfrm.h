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
 * SELinux support for the XFRM LSM hooks
 *
 * Author : Trent Jaeger, <jaegert@us.ibm.com>
 * Updated : Venkat Yekkirala, <vyekkirala@TrustedCS.com>
 */
#ifndef _SELINUX_XFRM_H_
#define _SELINUX_XFRM_H_

int selinux_xfrm_policy_alloc(struct xfrm_sec_ctx **ctxp,
			      struct xfrm_user_sec_ctx *sec_ctx);
int selinux_xfrm_policy_clone(struct xfrm_sec_ctx *old_ctx,
			      struct xfrm_sec_ctx **new_ctxp);
void selinux_xfrm_policy_free(struct xfrm_sec_ctx *ctx);
int selinux_xfrm_policy_delete(struct xfrm_sec_ctx *ctx);
int selinux_xfrm_state_alloc(struct xfrm_state *x,
	struct xfrm_user_sec_ctx *sec_ctx, u32 secid);
void selinux_xfrm_state_free(struct xfrm_state *x);
int selinux_xfrm_state_delete(struct xfrm_state *x);
int selinux_xfrm_policy_lookup(struct xfrm_sec_ctx *ctx, u32 fl_secid, u8 dir);
int selinux_xfrm_state_pol_flow_match(struct xfrm_state *x,
			struct xfrm_policy *xp, struct flowi *fl);

/*
 * Extract the security blob from the sock (it's actually on the socket)
 */
static inline struct inode_security_struct *get_sock_isec(struct sock *sk)
{
	if (!sk->sk_socket)
		return NULL;

	return SOCK_INODE(sk->sk_socket)->i_security;
}

#ifdef CONFIG_SECURITY_NETWORK_XFRM
extern atomic_t selinux_xfrm_refcount;

static inline int selinux_xfrm_enabled(void)
{
	return (atomic_read(&selinux_xfrm_refcount) > 0);
}

int selinux_xfrm_sock_rcv_skb(u32 sid, struct sk_buff *skb,
			struct avc_audit_data *ad);
int selinux_xfrm_postroute_last(u32 isec_sid, struct sk_buff *skb,
			struct avc_audit_data *ad, u8 proto);
int selinux_xfrm_decode_session(struct sk_buff *skb, u32 *sid, int ckall);

static inline void selinux_xfrm_notify_policyload(void)
{
	atomic_inc(&flow_cache_genid);
}
#else
static inline int selinux_xfrm_enabled(void)
{
	return 0;
}

static inline int selinux_xfrm_sock_rcv_skb(u32 isec_sid, struct sk_buff *skb,
			struct avc_audit_data *ad)
{
	return 0;
}

static inline int selinux_xfrm_postroute_last(u32 isec_sid, struct sk_buff *skb,
			struct avc_audit_data *ad, u8 proto)
{
	return 0;
}

static inline int selinux_xfrm_decode_session(struct sk_buff *skb, u32 *sid, int ckall)
{
	*sid = SECSID_NULL;
	return 0;
}

static inline void selinux_xfrm_notify_policyload(void)
{
}
#endif

static inline void selinux_skb_xfrm_sid(struct sk_buff *skb, u32 *sid)
{
	int err = selinux_xfrm_decode_session(skb, sid, 0);
	BUG_ON(err);
}

#endif /* _SELINUX_XFRM_H_ */
