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
 * RNG implementation using standard kernel RNG.
 *
 * Copyright (c) 2008 Herbert Xu <herbert@gondor.apana.org.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * any later version.
 *
 */

#include <crypto/internal/rng.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/random.h>

static int krng_get_random(struct crypto_rng *tfm, u8 *rdata, unsigned int dlen)
{
	get_random_bytes(rdata, dlen);
	return 0;
}

static int krng_reset(struct crypto_rng *tfm, u8 *seed, unsigned int slen)
{
	return 0;
}

static struct crypto_alg krng_alg = {
	.cra_name		= "stdrng",
	.cra_driver_name	= "krng",
	.cra_priority		= 200,
	.cra_flags		= CRYPTO_ALG_TYPE_RNG,
	.cra_ctxsize		= 0,
	.cra_type		= &crypto_rng_type,
	.cra_module		= THIS_MODULE,
	.cra_list		= LIST_HEAD_INIT(krng_alg.cra_list),
	.cra_u			= {
		.rng = {
			.rng_make_random	= krng_get_random,
			.rng_reset		= krng_reset,
			.seedsize		= 0,
		}
	}
};


/* Module initalization */
static int __init krng_mod_init(void)
{
	return crypto_register_alg(&krng_alg);
}

static void __exit krng_mod_fini(void)
{
	crypto_unregister_alg(&krng_alg);
	return;
}

module_init(krng_mod_init);
module_exit(krng_mod_fini);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel Random Number Generator");
MODULE_ALIAS("stdrng");
