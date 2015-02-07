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

#ifndef MD5_H
#define MD5_H
#ifndef HEADER_MD5_H
/* Try to avoid clashes with OpenSSL */
#define HEADER_MD5_H
#endif

struct MD5Context {
	__u32 buf[4];
	__u32 bits[2];
	unsigned char in[64];
};
#endif				/* !MD5_H */

#ifndef _HMAC_MD5_H
struct HMACMD5Context {
	struct MD5Context ctx;
	unsigned char k_ipad[65];
	unsigned char k_opad[65];
};
#endif				/* _HMAC_MD5_H */

void cifs_MD5_init(struct MD5Context *context);
void cifs_MD5_update(struct MD5Context *context, unsigned char const *buf,
			unsigned len);
void cifs_MD5_final(unsigned char digest[16], struct MD5Context *context);

/* The following definitions come from lib/hmacmd5.c  */

/* void hmac_md5_init_rfc2104(unsigned char *key, int key_len,
			struct HMACMD5Context *ctx);*/
void hmac_md5_init_limK_to_64(const unsigned char *key, int key_len,
			struct HMACMD5Context *ctx);
void hmac_md5_update(const unsigned char *text, int text_len,
			struct HMACMD5Context *ctx);
void hmac_md5_final(unsigned char *digest, struct HMACMD5Context *ctx);
/* void hmac_md5(unsigned char key[16], unsigned char *data, int data_len,
			unsigned char *digest);*/
