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
 * lib80211.h -- common bits for IEEE802.11 wireless drivers
 *
 * Copyright (c) 2008, John W. Linville <linville@tuxdriver.com>
 *
 * Some bits copied from old ieee80211 component, w/ original copyright
 * notices below:
 *
 * Original code based on Host AP (software wireless LAN access point) driver
 * for Intersil Prism2/2.5/3.
 *
 * Copyright (c) 2001-2002, SSH Communications Security Corp and Jouni Malinen
 * <j@w1.fi>
 * Copyright (c) 2002-2003, Jouni Malinen <j@w1.fi>
 *
 * Adaption to a generic IEEE 802.11 stack by James Ketrenos
 * <jketreno@linux.intel.com>
 *
 * Copyright (c) 2004, Intel Corporation
 *
 */

#ifndef LIB80211_H
#define LIB80211_H

#include <linux/types.h>
#include <linux/list.h>
#include <linux/module.h>
#include <asm/atomic.h>
#include <linux/if.h>
#include <linux/skbuff.h>
#include <linux/ieee80211.h>
#include <linux/timer.h>
/* print_ssid() is intended to be used in debug (and possibly error)
 * messages. It should never be used for passing ssid to user space. */
const char *print_ssid(char *buf, const char *ssid, u8 ssid_len);
#define DECLARE_SSID_BUF(var) char var[IEEE80211_MAX_SSID_LEN * 4 + 1] __maybe_unused

#define NUM_WEP_KEYS	4

enum {
	IEEE80211_CRYPTO_TKIP_COUNTERMEASURES = (1 << 0),
};

struct lib80211_crypto_ops {
	const char *name;
	struct list_head list;

	/* init new crypto context (e.g., allocate private data space,
	 * select IV, etc.); returns NULL on failure or pointer to allocated
	 * private data on success */
	void *(*init) (int keyidx);

	/* deinitialize crypto context and free allocated private data */
	void (*deinit) (void *priv);

	int (*build_iv) (struct sk_buff * skb, int hdr_len,
			 u8 *key, int keylen, void *priv);

	/* encrypt/decrypt return < 0 on error or >= 0 on success. The return
	 * value from decrypt_mpdu is passed as the keyidx value for
	 * decrypt_msdu. skb must have enough head and tail room for the
	 * encryption; if not, error will be returned; these functions are
	 * called for all MPDUs (i.e., fragments).
	 */
	int (*encrypt_mpdu) (struct sk_buff * skb, int hdr_len, void *priv);
	int (*decrypt_mpdu) (struct sk_buff * skb, int hdr_len, void *priv);

	/* These functions are called for full MSDUs, i.e. full frames.
	 * These can be NULL if full MSDU operations are not needed. */
	int (*encrypt_msdu) (struct sk_buff * skb, int hdr_len, void *priv);
	int (*decrypt_msdu) (struct sk_buff * skb, int keyidx, int hdr_len,
			     void *priv);

	int (*set_key) (void *key, int len, u8 * seq, void *priv);
	int (*get_key) (void *key, int len, u8 * seq, void *priv);

	/* procfs handler for printing out key information and possible
	 * statistics */
	char *(*print_stats) (char *p, void *priv);

	/* Crypto specific flag get/set for configuration settings */
	unsigned long (*get_flags) (void *priv);
	unsigned long (*set_flags) (unsigned long flags, void *priv);

	/* maximum number of bytes added by encryption; encrypt buf is
	 * allocated with extra_prefix_len bytes, copy of in_buf, and
	 * extra_postfix_len; encrypt need not use all this space, but
	 * the result must start at the beginning of the buffer and correct
	 * length must be returned */
	int extra_mpdu_prefix_len, extra_mpdu_postfix_len;
	int extra_msdu_prefix_len, extra_msdu_postfix_len;

	struct module *owner;
};

struct lib80211_crypt_data {
	struct list_head list;	/* delayed deletion list */
	struct lib80211_crypto_ops *ops;
	void *priv;
	atomic_t refcnt;
};

struct lib80211_crypt_info {
	char *name;
	/* Most clients will already have a lock,
	   so just point to that. */
	spinlock_t *lock;

	struct lib80211_crypt_data *crypt[NUM_WEP_KEYS];
	int tx_keyidx;		/* default TX key index (crypt[tx_keyidx]) */
	struct list_head crypt_deinit_list;
	struct timer_list crypt_deinit_timer;
	int crypt_quiesced;
};

int lib80211_crypt_info_init(struct lib80211_crypt_info *info, char *name,
                                spinlock_t *lock);
void lib80211_crypt_info_free(struct lib80211_crypt_info *info);
int lib80211_register_crypto_ops(struct lib80211_crypto_ops *ops);
int lib80211_unregister_crypto_ops(struct lib80211_crypto_ops *ops);
struct lib80211_crypto_ops *lib80211_get_crypto_ops(const char *name);
void lib80211_crypt_deinit_entries(struct lib80211_crypt_info *, int);
void lib80211_crypt_deinit_handler(unsigned long);
void lib80211_crypt_delayed_deinit(struct lib80211_crypt_info *info,
				    struct lib80211_crypt_data **crypt);
void lib80211_crypt_quiescing(struct lib80211_crypt_info *info);

#endif /* LIB80211_H */
