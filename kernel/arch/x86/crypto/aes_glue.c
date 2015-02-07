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
 * Glue Code for the asm optimized version of the AES Cipher Algorithm
 *
 */

#include <crypto/aes.h>

asmlinkage void aes_enc_blk(struct crypto_tfm *tfm, u8 *out, const u8 *in);
asmlinkage void aes_dec_blk(struct crypto_tfm *tfm, u8 *out, const u8 *in);

static void aes_encrypt(struct crypto_tfm *tfm, u8 *dst, const u8 *src)
{
	aes_enc_blk(tfm, dst, src);
}

static void aes_decrypt(struct crypto_tfm *tfm, u8 *dst, const u8 *src)
{
	aes_dec_blk(tfm, dst, src);
}

static struct crypto_alg aes_alg = {
	.cra_name		= "aes",
	.cra_driver_name	= "aes-asm",
	.cra_priority		= 200,
	.cra_flags		= CRYPTO_ALG_TYPE_CIPHER,
	.cra_blocksize		= AES_BLOCK_SIZE,
	.cra_ctxsize		= sizeof(struct crypto_aes_ctx),
	.cra_module		= THIS_MODULE,
	.cra_list		= LIST_HEAD_INIT(aes_alg.cra_list),
	.cra_u	= {
		.cipher	= {
			.cia_min_keysize	= AES_MIN_KEY_SIZE,
			.cia_max_keysize	= AES_MAX_KEY_SIZE,
			.cia_setkey		= crypto_aes_set_key,
			.cia_encrypt		= aes_encrypt,
			.cia_decrypt		= aes_decrypt
		}
	}
};

static int __init aes_init(void)
{
	return crypto_register_alg(&aes_alg);
}

static void __exit aes_fini(void)
{
	crypto_unregister_alg(&aes_alg);
}

module_init(aes_init);
module_exit(aes_fini);

MODULE_DESCRIPTION("Rijndael (AES) Cipher Algorithm, asm optimized");
MODULE_LICENSE("GPL");
MODULE_ALIAS("aes");
MODULE_ALIAS("aes-asm");
