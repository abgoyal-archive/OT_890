# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.

# MediaTek Inc. (C) 2010. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.


#
# Cryptographic API
#

obj-$(CONFIG_CRYPTO) += crypto.o
crypto-objs := api.o cipher.o digest.o compress.o

obj-$(CONFIG_CRYPTO_FIPS) += fips.o

crypto_algapi-$(CONFIG_PROC_FS) += proc.o
crypto_algapi-objs := algapi.o scatterwalk.o $(crypto_algapi-y)
obj-$(CONFIG_CRYPTO_ALGAPI2) += crypto_algapi.o

obj-$(CONFIG_CRYPTO_AEAD2) += aead.o

crypto_blkcipher-objs := ablkcipher.o
crypto_blkcipher-objs += blkcipher.o
obj-$(CONFIG_CRYPTO_BLKCIPHER2) += crypto_blkcipher.o
obj-$(CONFIG_CRYPTO_BLKCIPHER2) += chainiv.o
obj-$(CONFIG_CRYPTO_BLKCIPHER2) += eseqiv.o
obj-$(CONFIG_CRYPTO_SEQIV) += seqiv.o

crypto_hash-objs := hash.o
crypto_hash-objs += ahash.o
crypto_hash-objs += shash.o
obj-$(CONFIG_CRYPTO_HASH2) += crypto_hash.o

cryptomgr-objs := algboss.o testmgr.o

obj-$(CONFIG_CRYPTO_MANAGER2) += cryptomgr.o
obj-$(CONFIG_CRYPTO_HMAC) += hmac.o
obj-$(CONFIG_CRYPTO_XCBC) += xcbc.o
obj-$(CONFIG_CRYPTO_NULL) += crypto_null.o
obj-$(CONFIG_CRYPTO_MD4) += md4.o
obj-$(CONFIG_CRYPTO_MD5) += md5.o
obj-$(CONFIG_CRYPTO_RMD128) += rmd128.o
obj-$(CONFIG_CRYPTO_RMD160) += rmd160.o
obj-$(CONFIG_CRYPTO_RMD256) += rmd256.o
obj-$(CONFIG_CRYPTO_RMD320) += rmd320.o
obj-$(CONFIG_CRYPTO_SHA1) += sha1_generic.o
obj-$(CONFIG_CRYPTO_SHA256) += sha256_generic.o
obj-$(CONFIG_CRYPTO_SHA512) += sha512_generic.o
obj-$(CONFIG_CRYPTO_WP512) += wp512.o
obj-$(CONFIG_CRYPTO_TGR192) += tgr192.o
obj-$(CONFIG_CRYPTO_GF128MUL) += gf128mul.o
obj-$(CONFIG_CRYPTO_ECB) += ecb.o
obj-$(CONFIG_CRYPTO_CBC) += cbc.o
obj-$(CONFIG_CRYPTO_PCBC) += pcbc.o
obj-$(CONFIG_CRYPTO_CTS) += cts.o
obj-$(CONFIG_CRYPTO_LRW) += lrw.o
obj-$(CONFIG_CRYPTO_XTS) += xts.o
obj-$(CONFIG_CRYPTO_CTR) += ctr.o
obj-$(CONFIG_CRYPTO_GCM) += gcm.o
obj-$(CONFIG_CRYPTO_CCM) += ccm.o
obj-$(CONFIG_CRYPTO_CRYPTD) += cryptd.o
obj-$(CONFIG_CRYPTO_DES) += des_generic.o
obj-$(CONFIG_CRYPTO_FCRYPT) += fcrypt.o
obj-$(CONFIG_CRYPTO_BLOWFISH) += blowfish.o
obj-$(CONFIG_CRYPTO_TWOFISH) += twofish.o
obj-$(CONFIG_CRYPTO_TWOFISH_COMMON) += twofish_common.o
obj-$(CONFIG_CRYPTO_SERPENT) += serpent.o
obj-$(CONFIG_CRYPTO_AES) += aes_generic.o
obj-$(CONFIG_CRYPTO_CAMELLIA) += camellia.o
obj-$(CONFIG_CRYPTO_CAST5) += cast5.o
obj-$(CONFIG_CRYPTO_CAST6) += cast6.o
obj-$(CONFIG_CRYPTO_ARC4) += arc4.o
obj-$(CONFIG_CRYPTO_TEA) += tea.o
obj-$(CONFIG_CRYPTO_KHAZAD) += khazad.o
obj-$(CONFIG_CRYPTO_ANUBIS) += anubis.o
obj-$(CONFIG_CRYPTO_SEED) += seed.o
obj-$(CONFIG_CRYPTO_SALSA20) += salsa20_generic.o
obj-$(CONFIG_CRYPTO_DEFLATE) += deflate.o
obj-$(CONFIG_CRYPTO_MICHAEL_MIC) += michael_mic.o
obj-$(CONFIG_CRYPTO_CRC32C) += crc32c.o
obj-$(CONFIG_CRYPTO_AUTHENC) += authenc.o
obj-$(CONFIG_CRYPTO_LZO) += lzo.o
obj-$(CONFIG_CRYPTO_RNG2) += rng.o
obj-$(CONFIG_CRYPTO_RNG2) += krng.o
obj-$(CONFIG_CRYPTO_ANSI_CPRNG) += ansi_cprng.o
obj-$(CONFIG_CRYPTO_TEST) += tcrypt.o

#
# generic algorithms and the async_tx api
#
obj-$(CONFIG_XOR_BLOCKS) += xor.o
obj-$(CONFIG_ASYNC_CORE) += async_tx/
