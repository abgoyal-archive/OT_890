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
 * Copyright 2003-2004, Instant802 Networks, Inc.
 * Copyright 2005-2006, Devicescape Software, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/crypto.h>
#include <linux/err.h>

#include <net/mac80211.h>
#include "key.h"
#include "aes_ccm.h"

static void aes_ccm_prepare(struct crypto_cipher *tfm, u8 *scratch, u8 *a)
{
	int i;
	u8 *b_0, *aad, *b, *s_0;

	b_0 = scratch + 3 * AES_BLOCK_LEN;
	aad = scratch + 4 * AES_BLOCK_LEN;
	b = scratch;
	s_0 = scratch + AES_BLOCK_LEN;

	crypto_cipher_encrypt_one(tfm, b, b_0);

	/* Extra Authenticate-only data (always two AES blocks) */
	for (i = 0; i < AES_BLOCK_LEN; i++)
		aad[i] ^= b[i];
	crypto_cipher_encrypt_one(tfm, b, aad);

	aad += AES_BLOCK_LEN;

	for (i = 0; i < AES_BLOCK_LEN; i++)
		aad[i] ^= b[i];
	crypto_cipher_encrypt_one(tfm, a, aad);

	/* Mask out bits from auth-only-b_0 */
	b_0[0] &= 0x07;

	/* S_0 is used to encrypt T (= MIC) */
	b_0[14] = 0;
	b_0[15] = 0;
	crypto_cipher_encrypt_one(tfm, s_0, b_0);
}


void ieee80211_aes_ccm_encrypt(struct crypto_cipher *tfm, u8 *scratch,
			       u8 *data, size_t data_len,
			       u8 *cdata, u8 *mic)
{
	int i, j, last_len, num_blocks;
	u8 *pos, *cpos, *b, *s_0, *e, *b_0, *aad;

	b = scratch;
	s_0 = scratch + AES_BLOCK_LEN;
	e = scratch + 2 * AES_BLOCK_LEN;
	b_0 = scratch + 3 * AES_BLOCK_LEN;
	aad = scratch + 4 * AES_BLOCK_LEN;

	num_blocks = DIV_ROUND_UP(data_len, AES_BLOCK_LEN);
	last_len = data_len % AES_BLOCK_LEN;
	aes_ccm_prepare(tfm, scratch, b);

	/* Process payload blocks */
	pos = data;
	cpos = cdata;
	for (j = 1; j <= num_blocks; j++) {
		int blen = (j == num_blocks && last_len) ?
			last_len : AES_BLOCK_LEN;

		/* Authentication followed by encryption */
		for (i = 0; i < blen; i++)
			b[i] ^= pos[i];
		crypto_cipher_encrypt_one(tfm, b, b);

		b_0[14] = (j >> 8) & 0xff;
		b_0[15] = j & 0xff;
		crypto_cipher_encrypt_one(tfm, e, b_0);
		for (i = 0; i < blen; i++)
			*cpos++ = *pos++ ^ e[i];
	}

	for (i = 0; i < CCMP_MIC_LEN; i++)
		mic[i] = b[i] ^ s_0[i];
}


int ieee80211_aes_ccm_decrypt(struct crypto_cipher *tfm, u8 *scratch,
			      u8 *cdata, size_t data_len, u8 *mic, u8 *data)
{
	int i, j, last_len, num_blocks;
	u8 *pos, *cpos, *b, *s_0, *a, *b_0, *aad;

	b = scratch;
	s_0 = scratch + AES_BLOCK_LEN;
	a = scratch + 2 * AES_BLOCK_LEN;
	b_0 = scratch + 3 * AES_BLOCK_LEN;
	aad = scratch + 4 * AES_BLOCK_LEN;

	num_blocks = DIV_ROUND_UP(data_len, AES_BLOCK_LEN);
	last_len = data_len % AES_BLOCK_LEN;
	aes_ccm_prepare(tfm, scratch, a);

	/* Process payload blocks */
	cpos = cdata;
	pos = data;
	for (j = 1; j <= num_blocks; j++) {
		int blen = (j == num_blocks && last_len) ?
			last_len : AES_BLOCK_LEN;

		/* Decryption followed by authentication */
		b_0[14] = (j >> 8) & 0xff;
		b_0[15] = j & 0xff;
		crypto_cipher_encrypt_one(tfm, b, b_0);
		for (i = 0; i < blen; i++) {
			*pos = *cpos++ ^ b[i];
			a[i] ^= *pos++;
		}
		crypto_cipher_encrypt_one(tfm, a, a);
	}

	for (i = 0; i < CCMP_MIC_LEN; i++) {
		if ((mic[i] ^ s_0[i]) != a[i])
			return -1;
	}

	return 0;
}


struct crypto_cipher *ieee80211_aes_key_setup_encrypt(const u8 key[])
{
	struct crypto_cipher *tfm;

	tfm = crypto_alloc_cipher("aes", 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(tfm))
		return NULL;

	crypto_cipher_setkey(tfm, key, ALG_CCMP_KEY_LEN);

	return tfm;
}


void ieee80211_aes_key_free(struct crypto_cipher *tfm)
{
	if (tfm)
		crypto_free_cipher(tfm);
}
