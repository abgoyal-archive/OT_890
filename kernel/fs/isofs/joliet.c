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
 *  linux/fs/isofs/joliet.c
 *
 *  (C) 1996 Gordon Chaffee
 *
 *  Joliet: Microsoft's Unicode extensions to iso9660
 */

#include <linux/types.h>
#include <linux/nls.h>
#include "isofs.h"

/*
 * Convert Unicode 16 to UTF-8 or ASCII.
 */
static int
uni16_to_x8(unsigned char *ascii, __be16 *uni, int len, struct nls_table *nls)
{
	__be16 *ip, ch;
	unsigned char *op;

	ip = uni;
	op = ascii;

	while ((ch = get_unaligned(ip)) && len) {
		int llen;
		llen = nls->uni2char(be16_to_cpu(ch), op, NLS_MAX_CHARSET_SIZE);
		if (llen > 0)
			op += llen;
		else
			*op++ = '?';
		ip++;

		len--;
	}
	*op = 0;
	return (op - ascii);
}

/* Convert big endian wide character string to utf8 */
static int
wcsntombs_be(__u8 *s, const __u8 *pwcs, int inlen, int maxlen)
{
	const __u8 *ip;
	__u8 *op;
	int size;
	__u16 c;

	op = s;
	ip = pwcs;
	while ((*ip || ip[1]) && (maxlen > 0) && (inlen > 0)) {
		c = (*ip << 8) | ip[1];
		if (c > 0x7f) {
			size = utf8_wctomb(op, c, maxlen);
			if (size == -1) {
				/* Ignore character and move on */
				maxlen--;
			} else {
				op += size;
				maxlen -= size;
			}
		} else {
			*op++ = (__u8) c;
		}
		ip += 2;
		inlen--;
	}
	return (op - s);
}

int
get_joliet_filename(struct iso_directory_record * de, unsigned char *outname, struct inode * inode)
{
	unsigned char utf8;
	struct nls_table *nls;
	unsigned char len = 0;

	utf8 = ISOFS_SB(inode->i_sb)->s_utf8;
	nls = ISOFS_SB(inode->i_sb)->s_nls_iocharset;

	if (utf8) {
		len = wcsntombs_be(outname, de->name,
				de->name_len[0] >> 1, PAGE_SIZE);
	} else {
		len = uni16_to_x8(outname, (__be16 *) de->name,
				de->name_len[0] >> 1, nls);
	}
	if ((len > 2) && (outname[len-2] == ';') && (outname[len-1] == '1'))
		len -= 2;

	/*
	 * Windows doesn't like periods at the end of a name,
	 * so neither do we
	 */
	while (len >= 2 && (outname[len-1] == '.'))
		len--;

	return len;
}
