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
 * Quick & dirty crypto testing module.
 *
 * This will only exist until we have a better testing mechanism
 * (e.g. a char device).
 *
 * Copyright (c) 2002 James Morris <jmorris@intercode.com.au>
 * Copyright (c) 2002 Jean-Francois Dive <jef@linuxbe.org>
 * Copyright (c) 2007 Nokia Siemens Networks
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */
#ifndef _CRYPTO_TCRYPT_H
#define _CRYPTO_TCRYPT_H

struct cipher_speed_template {
	const char *key;
	unsigned int klen;
};

struct hash_speed {
	unsigned int blen;	/* buffer length */
	unsigned int plen;	/* per-update length */
};

/*
 * DES test vectors.
 */
#define DES3_SPEED_VECTORS	1

static struct cipher_speed_template des3_speed_template[] = {
	{
		.key	= "\x01\x23\x45\x67\x89\xab\xcd\xef"
			  "\x55\x55\x55\x55\x55\x55\x55\x55"
			  "\xfe\xdc\xba\x98\x76\x54\x32\x10",
		.klen	= 24,
	}
};

/*
 * Cipher speed tests
 */
static u8 speed_template_8[] = {8, 0};
static u8 speed_template_24[] = {24, 0};
static u8 speed_template_8_32[] = {8, 32, 0};
static u8 speed_template_16_32[] = {16, 32, 0};
static u8 speed_template_16_24_32[] = {16, 24, 32, 0};
static u8 speed_template_32_40_48[] = {32, 40, 48, 0};
static u8 speed_template_32_48_64[] = {32, 48, 64, 0};

/*
 * Digest speed tests
 */
static struct hash_speed generic_hash_speed_template[] = {
	{ .blen = 16,	.plen = 16, },
	{ .blen = 64,	.plen = 16, },
	{ .blen = 64,	.plen = 64, },
	{ .blen = 256,	.plen = 16, },
	{ .blen = 256,	.plen = 64, },
	{ .blen = 256,	.plen = 256, },
	{ .blen = 1024,	.plen = 16, },
	{ .blen = 1024,	.plen = 256, },
	{ .blen = 1024,	.plen = 1024, },
	{ .blen = 2048,	.plen = 16, },
	{ .blen = 2048,	.plen = 256, },
	{ .blen = 2048,	.plen = 1024, },
	{ .blen = 2048,	.plen = 2048, },
	{ .blen = 4096,	.plen = 16, },
	{ .blen = 4096,	.plen = 256, },
	{ .blen = 4096,	.plen = 1024, },
	{ .blen = 4096,	.plen = 4096, },
	{ .blen = 8192,	.plen = 16, },
	{ .blen = 8192,	.plen = 256, },
	{ .blen = 8192,	.plen = 1024, },
	{ .blen = 8192,	.plen = 4096, },
	{ .blen = 8192,	.plen = 8192, },

	/* End marker */
	{  .blen = 0,	.plen = 0, }
};

#endif	/* _CRYPTO_TCRYPT_H */
