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
 * Original code based on Host AP (software wireless LAN access point) driver
 * for Intersil Prism2/2.5/3.
 *
 * Copyright (c) 2001-2002, SSH Communications Security Corp and Jouni Malinen
 * <jkmaline@cc.hut.fi>
 * Copyright (c) 2002-2003, Jouni Malinen <jkmaline@cc.hut.fi>
 *
 * Adaption to a generic IEEE 802.11 stack by James Ketrenos
 * <jketreno@linux.intel.com>
 *
 * Copyright (c) 2004, Intel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. See README and COPYING for
 * more details.
 */

/*
 * This file defines the interface to the ieee80211 crypto module.
 */
#ifndef IEEE80211_CRYPT_H
#define IEEE80211_CRYPT_H

#include <linux/skbuff.h>

struct ieee80211_crypto_ops {
	const char *name;

	/* init new crypto context (e.g., allocate private data space,
	 * select IV, etc.); returns NULL on failure or pointer to allocated
	 * private data on success */
	void * (*init)(int keyidx);

	/* deinitialize crypto context and free allocated private data */
	void (*deinit)(void *priv);

	/* encrypt/decrypt return < 0 on error or >= 0 on success. The return
	 * value from decrypt_mpdu is passed as the keyidx value for
	 * decrypt_msdu. skb must have enough head and tail room for the
	 * encryption; if not, error will be returned; these functions are
	 * called for all MPDUs (i.e., fragments).
	 */
	int (*encrypt_mpdu)(struct sk_buff *skb, int hdr_len, void *priv);
	int (*decrypt_mpdu)(struct sk_buff *skb, int hdr_len, void *priv);

	/* These functions are called for full MSDUs, i.e. full frames.
	 * These can be NULL if full MSDU operations are not needed. */
	int (*encrypt_msdu)(struct sk_buff *skb, int hdr_len, void *priv);
	int (*decrypt_msdu)(struct sk_buff *skb, int keyidx, int hdr_len,
			    void *priv);

	int (*set_key)(void *key, int len, u8 *seq, void *priv);
	int (*get_key)(void *key, int len, u8 *seq, void *priv);

	/* procfs handler for printing out key information and possible
	 * statistics */
	char * (*print_stats)(char *p, void *priv);

	/* maximum number of bytes added by encryption; encrypt buf is
	 * allocated with extra_prefix_len bytes, copy of in_buf, and
	 * extra_postfix_len; encrypt need not use all this space, but
	 * the result must start at the beginning of the buffer and correct
	 * length must be returned */
	int extra_prefix_len, extra_postfix_len;

	struct module *owner;
};

struct ieee80211_crypt_data {
	struct list_head list; /* delayed deletion list */
	struct ieee80211_crypto_ops *ops;
	void *priv;
	atomic_t refcnt;
};

int ieee80211_register_crypto_ops(struct ieee80211_crypto_ops *ops);
int ieee80211_unregister_crypto_ops(struct ieee80211_crypto_ops *ops);
struct ieee80211_crypto_ops * ieee80211_get_crypto_ops(const char *name);
void ieee80211_crypt_deinit_entries(struct ieee80211_device *, int);
void ieee80211_crypt_deinit_handler(unsigned long);
void ieee80211_crypt_delayed_deinit(struct ieee80211_device *ieee,
				    struct ieee80211_crypt_data **crypt);

#endif
