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

#ifndef _LINUX_NLS_H
#define _LINUX_NLS_H

#include <linux/init.h>

/* unicode character */
typedef __u16 wchar_t;

struct nls_table {
	const char *charset;
	const char *alias;
	int (*uni2char) (wchar_t uni, unsigned char *out, int boundlen);
	int (*char2uni) (const unsigned char *rawstring, int boundlen,
			 wchar_t *uni);
	const unsigned char *charset2lower;
	const unsigned char *charset2upper;
	struct module *owner;
	struct nls_table *next;
};

/* this value hold the maximum octet of charset */
#define NLS_MAX_CHARSET_SIZE 6 /* for UTF-8 */

/* nls.c */
extern int register_nls(struct nls_table *);
extern int unregister_nls(struct nls_table *);
extern struct nls_table *load_nls(char *);
extern void unload_nls(struct nls_table *);
extern struct nls_table *load_nls_default(void);

extern int utf8_mbtowc(wchar_t *, const __u8 *, int);
extern int utf8_mbstowcs(wchar_t *, const __u8 *, int);
extern int utf8_wctomb(__u8 *, wchar_t, int);
extern int utf8_wcstombs(__u8 *, const wchar_t *, int);

static inline unsigned char nls_tolower(struct nls_table *t, unsigned char c)
{
	unsigned char nc = t->charset2lower[c];

	return nc ? nc : c;
}

static inline unsigned char nls_toupper(struct nls_table *t, unsigned char c)
{
	unsigned char nc = t->charset2upper[c];

	return nc ? nc : c;
}

static inline int nls_strnicmp(struct nls_table *t, const unsigned char *s1,
		const unsigned char *s2, int len)
{
	while (len--) {
		if (nls_tolower(t, *s1++) != nls_tolower(t, *s2++))
			return 1;
	}

	return 0;
}

#define MODULE_ALIAS_NLS(name)	MODULE_ALIAS("nls_" __stringify(name))

#endif /* _LINUX_NLS_H */

