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
 * Function for checking keys for the DES and Tripple DES Encryption
 * algorithms.
 *
 * Originally released as descore by Dana L. How <how@isl.stanford.edu>.
 * Modified by Raimar Falke <rf13@inf.tu-dresden.de> for the Linux-Kernel.
 * Derived from Cryptoapi and Nettle implementations, adapted for in-place
 * scatterlist interface.  Changed LGPL to GPL per section 3 of the LGPL.
 *
 * s390 Version:
 *   Copyright IBM Corp. 2003
 *   Author(s): Thomas Spatzier
 *		Jan Glauber (jan.glauber@de.ibm.com)
 *
 * Derived from "crypto/des.c"
 *   Copyright (c) 1992 Dana L. How.
 *   Copyright (c) Raimar Falke <rf13@inf.tu-dresden.de>
 *   Copyright (c) Gisle Sflensminde <gisle@ii.uib.no>
 *   Copyright (C) 2001 Niels Mvller.
 *   Copyright (c) 2002 James Morris <jmorris@intercode.com.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/crypto.h>
#include "crypto_des.h"

#define ROR(d,c,o)	((d) = (d) >> (c) | (d) << (o))

static const u8 parity[] = {
	8,1,0,8,0,8,8,0,0,8,8,0,8,0,2,8,0,8,8,0,8,0,0,8,8,0,0,8,0,8,8,3,
	0,8,8,0,8,0,0,8,8,0,0,8,0,8,8,0,8,0,0,8,0,8,8,0,0,8,8,0,8,0,0,8,
	0,8,8,0,8,0,0,8,8,0,0,8,0,8,8,0,8,0,0,8,0,8,8,0,0,8,8,0,8,0,0,8,
	8,0,0,8,0,8,8,0,0,8,8,0,8,0,0,8,0,8,8,0,8,0,0,8,8,0,0,8,0,8,8,0,
	0,8,8,0,8,0,0,8,8,0,0,8,0,8,8,0,8,0,0,8,0,8,8,0,0,8,8,0,8,0,0,8,
	8,0,0,8,0,8,8,0,0,8,8,0,8,0,0,8,0,8,8,0,8,0,0,8,8,0,0,8,0,8,8,0,
	8,0,0,8,0,8,8,0,0,8,8,0,8,0,0,8,0,8,8,0,8,0,0,8,8,0,0,8,0,8,8,0,
	4,8,8,0,8,0,0,8,8,0,0,8,0,8,8,0,8,5,0,8,0,8,8,0,0,8,8,0,8,0,6,8,
};

/*
 * RFC2451: Weak key checks SHOULD be performed.
 */
int
crypto_des_check_key(const u8 *key, unsigned int keylen, u32 *flags)
{
	u32 n, w;

	n  = parity[key[0]]; n <<= 4;
	n |= parity[key[1]]; n <<= 4;
	n |= parity[key[2]]; n <<= 4;
	n |= parity[key[3]]; n <<= 4;
	n |= parity[key[4]]; n <<= 4;
	n |= parity[key[5]]; n <<= 4;
	n |= parity[key[6]]; n <<= 4;
	n |= parity[key[7]];
	w = 0x88888888L;

	if ((*flags & CRYPTO_TFM_REQ_WEAK_KEY)
	    && !((n - (w >> 3)) & w)) {  /* 1 in 10^10 keys passes this test */
		if (n < 0x41415151) {
			if (n < 0x31312121) {
				if (n < 0x14141515) {
					/* 01 01 01 01 01 01 01 01 */
					if (n == 0x11111111) goto weak;
					/* 01 1F 01 1F 01 0E 01 0E */
					if (n == 0x13131212) goto weak;
				} else {
					/* 01 E0 01 E0 01 F1 01 F1 */
					if (n == 0x14141515) goto weak;
					/* 01 FE 01 FE 01 FE 01 FE */
					if (n == 0x16161616) goto weak;
				}
			} else {
				if (n < 0x34342525) {
					/* 1F 01 1F 01 0E 01 0E 01 */
					if (n == 0x31312121) goto weak;
					/* 1F 1F 1F 1F 0E 0E 0E 0E (?) */
					if (n == 0x33332222) goto weak;
				} else {
					/* 1F E0 1F E0 0E F1 0E F1 */
					if (n == 0x34342525) goto weak;
					/* 1F FE 1F FE 0E FE 0E FE */
					if (n == 0x36362626) goto weak;
				}
			}
		} else {
			if (n < 0x61616161) {
				if (n < 0x44445555) {
					/* E0 01 E0 01 F1 01 F1 01 */
					if (n == 0x41415151) goto weak;
					/* E0 1F E0 1F F1 0E F1 0E */
					if (n == 0x43435252) goto weak;
				} else {
					/* E0 E0 E0 E0 F1 F1 F1 F1 (?) */
					if (n == 0x44445555) goto weak;
					/* E0 FE E0 FE F1 FE F1 FE */
					if (n == 0x46465656) goto weak;
				}
			} else {
				if (n < 0x64646565) {
					/* FE 01 FE 01 FE 01 FE 01 */
					if (n == 0x61616161) goto weak;
					/* FE 1F FE 1F FE 0E FE 0E */
					if (n == 0x63636262) goto weak;
				} else {
					/* FE E0 FE E0 FE F1 FE F1 */
					if (n == 0x64646565) goto weak;
					/* FE FE FE FE FE FE FE FE */
					if (n == 0x66666666) goto weak;
				}
			}
		}
	}
	return 0;
weak:
	*flags |= CRYPTO_TFM_RES_WEAK_KEY;
	return -EINVAL;
}

EXPORT_SYMBOL(crypto_des_check_key);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Key Check function for DES &  DES3 Cipher Algorithms");
