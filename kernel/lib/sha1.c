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
 * SHA transform algorithm, originally taken from code written by
 * Peter Gutmann, and placed in the public domain.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cryptohash.h>

/* The SHA f()-functions.  */

#define f1(x,y,z)   (z ^ (x & (y ^ z)))		/* x ? y : z */
#define f2(x,y,z)   (x ^ y ^ z)			/* XOR */
#define f3(x,y,z)   ((x & y) + (z & (x ^ y)))	/* majority */

/* The SHA Mysterious Constants */

#define K1  0x5A827999L			/* Rounds  0-19: sqrt(2) * 2^30 */
#define K2  0x6ED9EBA1L			/* Rounds 20-39: sqrt(3) * 2^30 */
#define K3  0x8F1BBCDCL			/* Rounds 40-59: sqrt(5) * 2^30 */
#define K4  0xCA62C1D6L			/* Rounds 60-79: sqrt(10) * 2^30 */

/**
 * sha_transform - single block SHA1 transform
 *
 * @digest: 160 bit digest to update
 * @data:   512 bits of data to hash
 * @W:      80 words of workspace (see note)
 *
 * This function generates a SHA1 digest for a single 512-bit block.
 * Be warned, it does not handle padding and message digest, do not
 * confuse it with the full FIPS 180-1 digest algorithm for variable
 * length messages.
 *
 * Note: If the hash is security sensitive, the caller should be sure
 * to clear the workspace. This is left to the caller to avoid
 * unnecessary clears between chained hashing operations.
 */
void sha_transform(__u32 *digest, const char *in, __u32 *W)
{
	__u32 a, b, c, d, e, t, i;

	for (i = 0; i < 16; i++)
		W[i] = be32_to_cpu(((const __be32 *)in)[i]);

	for (i = 0; i < 64; i++)
		W[i+16] = rol32(W[i+13] ^ W[i+8] ^ W[i+2] ^ W[i], 1);

	a = digest[0];
	b = digest[1];
	c = digest[2];
	d = digest[3];
	e = digest[4];

	for (i = 0; i < 20; i++) {
		t = f1(b, c, d) + K1 + rol32(a, 5) + e + W[i];
		e = d; d = c; c = rol32(b, 30); b = a; a = t;
	}

	for (; i < 40; i ++) {
		t = f2(b, c, d) + K2 + rol32(a, 5) + e + W[i];
		e = d; d = c; c = rol32(b, 30); b = a; a = t;
	}

	for (; i < 60; i ++) {
		t = f3(b, c, d) + K3 + rol32(a, 5) + e + W[i];
		e = d; d = c; c = rol32(b, 30); b = a; a = t;
	}

	for (; i < 80; i ++) {
		t = f2(b, c, d) + K4 + rol32(a, 5) + e + W[i];
		e = d; d = c; c = rol32(b, 30); b = a; a = t;
	}

	digest[0] += a;
	digest[1] += b;
	digest[2] += c;
	digest[3] += d;
	digest[4] += e;
}
EXPORT_SYMBOL(sha_transform);

/**
 * sha_init - initialize the vectors for a SHA1 digest
 * @buf: vector to initialize
 */
void sha_init(__u32 *buf)
{
	buf[0] = 0x67452301;
	buf[1] = 0xefcdab89;
	buf[2] = 0x98badcfe;
	buf[3] = 0x10325476;
	buf[4] = 0xc3d2e1f0;
}

