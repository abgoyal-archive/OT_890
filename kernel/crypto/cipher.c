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
 * Cryptographic API.
 *
 * Cipher operations.
 *
 * Copyright (c) 2002 James Morris <jmorris@intercode.com.au>
 * Copyright (c) 2005 Herbert Xu <herbert@gondor.apana.org.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option) 
 * any later version.
 *
 */

#include <linux/kernel.h>
#include <linux/crypto.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/string.h>
#include "internal.h"

static int setkey_unaligned(struct crypto_tfm *tfm, const u8 *key,
			    unsigned int keylen)
{
	struct cipher_alg *cia = &tfm->__crt_alg->cra_cipher;
	unsigned long alignmask = crypto_tfm_alg_alignmask(tfm);
	int ret;
	u8 *buffer, *alignbuffer;
	unsigned long absize;

	absize = keylen + alignmask;
	buffer = kmalloc(absize, GFP_ATOMIC);
	if (!buffer)
		return -ENOMEM;

	alignbuffer = (u8 *)ALIGN((unsigned long)buffer, alignmask + 1);
	memcpy(alignbuffer, key, keylen);
	ret = cia->cia_setkey(tfm, alignbuffer, keylen);
	memset(alignbuffer, 0, keylen);
	kfree(buffer);
	return ret;

}

static int setkey(struct crypto_tfm *tfm, const u8 *key, unsigned int keylen)
{
	struct cipher_alg *cia = &tfm->__crt_alg->cra_cipher;
	unsigned long alignmask = crypto_tfm_alg_alignmask(tfm);

	tfm->crt_flags &= ~CRYPTO_TFM_RES_MASK;
	if (keylen < cia->cia_min_keysize || keylen > cia->cia_max_keysize) {
		tfm->crt_flags |= CRYPTO_TFM_RES_BAD_KEY_LEN;
		return -EINVAL;
	}

	if ((unsigned long)key & alignmask)
		return setkey_unaligned(tfm, key, keylen);

	return cia->cia_setkey(tfm, key, keylen);
}

static void cipher_crypt_unaligned(void (*fn)(struct crypto_tfm *, u8 *,
					      const u8 *),
				   struct crypto_tfm *tfm,
				   u8 *dst, const u8 *src)
{
	unsigned long alignmask = crypto_tfm_alg_alignmask(tfm);
	unsigned int size = crypto_tfm_alg_blocksize(tfm);
	u8 buffer[size + alignmask];
	u8 *tmp = (u8 *)ALIGN((unsigned long)buffer, alignmask + 1);

	memcpy(tmp, src, size);
	fn(tfm, tmp, tmp);
	memcpy(dst, tmp, size);
}

static void cipher_encrypt_unaligned(struct crypto_tfm *tfm,
				     u8 *dst, const u8 *src)
{
	unsigned long alignmask = crypto_tfm_alg_alignmask(tfm);
	struct cipher_alg *cipher = &tfm->__crt_alg->cra_cipher;

	if (unlikely(((unsigned long)dst | (unsigned long)src) & alignmask)) {
		cipher_crypt_unaligned(cipher->cia_encrypt, tfm, dst, src);
		return;
	}

	cipher->cia_encrypt(tfm, dst, src);
}

static void cipher_decrypt_unaligned(struct crypto_tfm *tfm,
				     u8 *dst, const u8 *src)
{
	unsigned long alignmask = crypto_tfm_alg_alignmask(tfm);
	struct cipher_alg *cipher = &tfm->__crt_alg->cra_cipher;

	if (unlikely(((unsigned long)dst | (unsigned long)src) & alignmask)) {
		cipher_crypt_unaligned(cipher->cia_decrypt, tfm, dst, src);
		return;
	}

	cipher->cia_decrypt(tfm, dst, src);
}

int crypto_init_cipher_ops(struct crypto_tfm *tfm)
{
	struct cipher_tfm *ops = &tfm->crt_cipher;
	struct cipher_alg *cipher = &tfm->__crt_alg->cra_cipher;

	ops->cit_setkey = setkey;
	ops->cit_encrypt_one = crypto_tfm_alg_alignmask(tfm) ?
		cipher_encrypt_unaligned : cipher->cia_encrypt;
	ops->cit_decrypt_one = crypto_tfm_alg_alignmask(tfm) ?
		cipher_decrypt_unaligned : cipher->cia_decrypt;

	return 0;
}

void crypto_exit_cipher_ops(struct crypto_tfm *tfm)
{
}
