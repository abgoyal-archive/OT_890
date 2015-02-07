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
 * s390 generic implementation of the SHA Secure Hash Algorithms.
 *
 * Copyright IBM Corp. 2007
 * Author(s): Jan Glauber (jang@de.ibm.com)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

#include <linux/crypto.h>
#include "sha.h"
#include "crypt_s390.h"

void s390_sha_update(struct crypto_tfm *tfm, const u8 *data, unsigned int len)
{
	struct s390_sha_ctx *ctx = crypto_tfm_ctx(tfm);
	unsigned int bsize = crypto_tfm_alg_blocksize(tfm);
	unsigned int index;
	int ret;

	/* how much is already in the buffer? */
	index = ctx->count & (bsize - 1);
	ctx->count += len;

	if ((index + len) < bsize)
		goto store;

	/* process one stored block */
	if (index) {
		memcpy(ctx->buf + index, data, bsize - index);
		ret = crypt_s390_kimd(ctx->func, ctx->state, ctx->buf, bsize);
		BUG_ON(ret != bsize);
		data += bsize - index;
		len -= bsize - index;
	}

	/* process as many blocks as possible */
	if (len >= bsize) {
		ret = crypt_s390_kimd(ctx->func, ctx->state, data,
				      len & ~(bsize - 1));
		BUG_ON(ret != (len & ~(bsize - 1)));
		data += ret;
		len -= ret;
	}
store:
	if (len)
		memcpy(ctx->buf + index , data, len);
}
EXPORT_SYMBOL_GPL(s390_sha_update);

void s390_sha_final(struct crypto_tfm *tfm, u8 *out)
{
	struct s390_sha_ctx *ctx = crypto_tfm_ctx(tfm);
	unsigned int bsize = crypto_tfm_alg_blocksize(tfm);
	u64 bits;
	unsigned int index, end, plen;
	int ret;

	/* SHA-512 uses 128 bit padding length */
	plen = (bsize > SHA256_BLOCK_SIZE) ? 16 : 8;

	/* must perform manual padding */
	index = ctx->count & (bsize - 1);
	end = (index < bsize - plen) ? bsize : (2 * bsize);

	/* start pad with 1 */
	ctx->buf[index] = 0x80;
	index++;

	/* pad with zeros */
	memset(ctx->buf + index, 0x00, end - index - 8);

	/*
	 * Append message length. Well, SHA-512 wants a 128 bit lenght value,
	 * nevertheless we use u64, should be enough for now...
	 */
	bits = ctx->count * 8;
	memcpy(ctx->buf + end - 8, &bits, sizeof(bits));

	ret = crypt_s390_kimd(ctx->func, ctx->state, ctx->buf, end);
	BUG_ON(ret != end);

	/* copy digest to out */
	memcpy(out, ctx->state, crypto_hash_digestsize(crypto_hash_cast(tfm)));
	/* wipe context */
	memset(ctx, 0, sizeof *ctx);
}
EXPORT_SYMBOL_GPL(s390_sha_final);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("s390 SHA cipher common functions");
