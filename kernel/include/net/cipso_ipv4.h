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
 * CIPSO - Commercial IP Security Option
 *
 * This is an implementation of the CIPSO 2.2 protocol as specified in
 * draft-ietf-cipso-ipsecurity-01.txt with additional tag types as found in
 * FIPS-188, copies of both documents can be found in the Documentation
 * directory.  While CIPSO never became a full IETF RFC standard many vendors
 * have chosen to adopt the protocol and over the years it has become a
 * de-facto standard for labeled networking.
 *
 * Author: Paul Moore <paul.moore@hp.com>
 *
 */

/*
 * (c) Copyright Hewlett-Packard Development Company, L.P., 2006
 *
 * This program is free software;  you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY;  without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program;  if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#ifndef _CIPSO_IPV4_H
#define _CIPSO_IPV4_H

#include <linux/types.h>
#include <linux/rcupdate.h>
#include <linux/list.h>
#include <linux/net.h>
#include <linux/skbuff.h>
#include <net/netlabel.h>
#include <asm/atomic.h>

/* known doi values */
#define CIPSO_V4_DOI_UNKNOWN          0x00000000

/* standard tag types */
#define CIPSO_V4_TAG_INVALID          0
#define CIPSO_V4_TAG_RBITMAP          1
#define CIPSO_V4_TAG_ENUM             2
#define CIPSO_V4_TAG_RANGE            5
#define CIPSO_V4_TAG_PBITMAP          6
#define CIPSO_V4_TAG_FREEFORM         7

/* non-standard tag types (tags > 127) */
#define CIPSO_V4_TAG_LOCAL            128

/* doi mapping types */
#define CIPSO_V4_MAP_UNKNOWN          0
#define CIPSO_V4_MAP_TRANS            1
#define CIPSO_V4_MAP_PASS             2
#define CIPSO_V4_MAP_LOCAL            3

/* limits */
#define CIPSO_V4_MAX_REM_LVLS         255
#define CIPSO_V4_INV_LVL              0x80000000
#define CIPSO_V4_MAX_LOC_LVLS         (CIPSO_V4_INV_LVL - 1)
#define CIPSO_V4_MAX_REM_CATS         65534
#define CIPSO_V4_INV_CAT              0x80000000
#define CIPSO_V4_MAX_LOC_CATS         (CIPSO_V4_INV_CAT - 1)

/*
 * CIPSO DOI definitions
 */

/* DOI definition struct */
#define CIPSO_V4_TAG_MAXCNT           5
struct cipso_v4_doi {
	u32 doi;
	u32 type;
	union {
		struct cipso_v4_std_map_tbl *std;
	} map;
	u8 tags[CIPSO_V4_TAG_MAXCNT];

	atomic_t refcount;
	struct list_head list;
	struct rcu_head rcu;
};

/* Standard CIPSO mapping table */
/* NOTE: the highest order bit (i.e. 0x80000000) is an 'invalid' flag, if the
 *       bit is set then consider that value as unspecified, meaning the
 *       mapping for that particular level/category is invalid */
struct cipso_v4_std_map_tbl {
	struct {
		u32 *cipso;
		u32 *local;
		u32 cipso_size;
		u32 local_size;
	} lvl;
	struct {
		u32 *cipso;
		u32 *local;
		u32 cipso_size;
		u32 local_size;
	} cat;
};

/*
 * Sysctl Variables
 */

#ifdef CONFIG_NETLABEL
extern int cipso_v4_cache_enabled;
extern int cipso_v4_cache_bucketsize;
extern int cipso_v4_rbm_optfmt;
extern int cipso_v4_rbm_strictvalid;
#endif

/*
 * Helper Functions
 */

#define CIPSO_V4_OPTEXIST(x) (IPCB(x)->opt.cipso != 0)
#define CIPSO_V4_OPTPTR(x) (skb_network_header(x) + IPCB(x)->opt.cipso)

/*
 * DOI List Functions
 */

#ifdef CONFIG_NETLABEL
int cipso_v4_doi_add(struct cipso_v4_doi *doi_def,
		     struct netlbl_audit *audit_info);
void cipso_v4_doi_free(struct cipso_v4_doi *doi_def);
int cipso_v4_doi_remove(u32 doi, struct netlbl_audit *audit_info);
struct cipso_v4_doi *cipso_v4_doi_getdef(u32 doi);
void cipso_v4_doi_putdef(struct cipso_v4_doi *doi_def);
int cipso_v4_doi_walk(u32 *skip_cnt,
		     int (*callback) (struct cipso_v4_doi *doi_def, void *arg),
	             void *cb_arg);
#else
static inline int cipso_v4_doi_add(struct cipso_v4_doi *doi_def,
				   struct netlbl_audit *audit_info)
{
	return -ENOSYS;
}

static inline void cipso_v4_doi_free(struct cipso_v4_doi *doi_def)
{
	return;
}

static inline int cipso_v4_doi_remove(u32 doi,
				      struct netlbl_audit *audit_info)
{
	return 0;
}

static inline struct cipso_v4_doi *cipso_v4_doi_getdef(u32 doi)
{
	return NULL;
}

static inline int cipso_v4_doi_walk(u32 *skip_cnt,
		     int (*callback) (struct cipso_v4_doi *doi_def, void *arg),
		     void *cb_arg)
{
	return 0;
}

static inline int cipso_v4_doi_domhsh_add(struct cipso_v4_doi *doi_def,
					  const char *domain)
{
	return -ENOSYS;
}

static inline int cipso_v4_doi_domhsh_remove(struct cipso_v4_doi *doi_def,
					     const char *domain)
{
	return 0;
}
#endif /* CONFIG_NETLABEL */

/*
 * Label Mapping Cache Functions
 */

#ifdef CONFIG_NETLABEL
void cipso_v4_cache_invalidate(void);
int cipso_v4_cache_add(const struct sk_buff *skb,
		       const struct netlbl_lsm_secattr *secattr);
#else
static inline void cipso_v4_cache_invalidate(void)
{
	return;
}

static inline int cipso_v4_cache_add(const struct sk_buff *skb,
				     const struct netlbl_lsm_secattr *secattr)
{
	return 0;
}
#endif /* CONFIG_NETLABEL */

/*
 * Protocol Handling Functions
 */

#ifdef CONFIG_NETLABEL
void cipso_v4_error(struct sk_buff *skb, int error, u32 gateway);
int cipso_v4_sock_setattr(struct sock *sk,
			  const struct cipso_v4_doi *doi_def,
			  const struct netlbl_lsm_secattr *secattr);
void cipso_v4_sock_delattr(struct sock *sk);
int cipso_v4_sock_getattr(struct sock *sk, struct netlbl_lsm_secattr *secattr);
int cipso_v4_skbuff_setattr(struct sk_buff *skb,
			    const struct cipso_v4_doi *doi_def,
			    const struct netlbl_lsm_secattr *secattr);
int cipso_v4_skbuff_delattr(struct sk_buff *skb);
int cipso_v4_skbuff_getattr(const struct sk_buff *skb,
			    struct netlbl_lsm_secattr *secattr);
int cipso_v4_validate(const struct sk_buff *skb, unsigned char **option);
#else
static inline void cipso_v4_error(struct sk_buff *skb,
				  int error,
				  u32 gateway)
{
	return;
}

static inline int cipso_v4_sock_setattr(struct sock *sk,
				      const struct cipso_v4_doi *doi_def,
				      const struct netlbl_lsm_secattr *secattr)
{
	return -ENOSYS;
}

static inline void cipso_v4_sock_delattr(struct sock *sk)
{
}

static inline int cipso_v4_sock_getattr(struct sock *sk,
					struct netlbl_lsm_secattr *secattr)
{
	return -ENOSYS;
}

static inline int cipso_v4_skbuff_setattr(struct sk_buff *skb,
				      const struct cipso_v4_doi *doi_def,
				      const struct netlbl_lsm_secattr *secattr)
{
	return -ENOSYS;
}

static inline int cipso_v4_skbuff_delattr(struct sk_buff *skb)
{
	return -ENOSYS;
}

static inline int cipso_v4_skbuff_getattr(const struct sk_buff *skb,
					  struct netlbl_lsm_secattr *secattr)
{
	return -ENOSYS;
}

static inline int cipso_v4_validate(const struct sk_buff *skb,
				    unsigned char **option)
{
	return -ENOSYS;
}
#endif /* CONFIG_NETLABEL */

#endif /* _CIPSO_IPV4_H */
