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

#ifndef _DYNAMIC_PRINTK_H
#define _DYNAMIC_PRINTK_H

#define DYNAMIC_DEBUG_HASH_BITS 6
#define DEBUG_HASH_TABLE_SIZE (1 << DYNAMIC_DEBUG_HASH_BITS)

#define TYPE_BOOLEAN 1

#define DYNAMIC_ENABLED_ALL 0
#define DYNAMIC_ENABLED_NONE 1
#define DYNAMIC_ENABLED_SOME 2

extern int dynamic_enabled;

/* dynamic_printk_enabled, and dynamic_printk_enabled2 are bitmasks in which
 * bit n is set to 1 if any modname hashes into the bucket n, 0 otherwise. They
 * use independent hash functions, to reduce the chance of false positives.
 */
extern long long dynamic_printk_enabled;
extern long long dynamic_printk_enabled2;

struct mod_debug {
	char *modname;
	char *logical_modname;
	char *flag_names;
	int type;
	int hash;
	int hash2;
} __attribute__((aligned(8)));

int register_dynamic_debug_module(char *mod_name, int type, char *share_name,
					char *flags, int hash, int hash2);

#if defined(CONFIG_DYNAMIC_PRINTK_DEBUG)
extern int unregister_dynamic_debug_module(char *mod_name);
extern int __dynamic_dbg_enabled_helper(char *modname, int type,
					int value, int hash);

#define __dynamic_dbg_enabled(module, type, value, level, hash)  ({	     \
	int __ret = 0;							     \
	if (unlikely((dynamic_printk_enabled & (1LL << DEBUG_HASH)) &&	     \
			(dynamic_printk_enabled2 & (1LL << DEBUG_HASH2))))   \
			__ret = __dynamic_dbg_enabled_helper(module, type,   \
								value, hash);\
	__ret; })

#define dynamic_pr_debug(fmt, ...) do {					    \
	static char mod_name[]						    \
	__attribute__((section("__verbose_strings")))			    \
	 = KBUILD_MODNAME;						    \
	static struct mod_debug descriptor				    \
	__used								    \
	__attribute__((section("__verbose"), aligned(8))) =		    \
	{ mod_name, mod_name, NULL, TYPE_BOOLEAN, DEBUG_HASH, DEBUG_HASH2 };\
	if (__dynamic_dbg_enabled(KBUILD_MODNAME, TYPE_BOOLEAN,		    \
						0, 0, DEBUG_HASH))	    \
		printk(KERN_DEBUG KBUILD_MODNAME ":" fmt,		    \
				##__VA_ARGS__);				    \
	} while (0)

#define dynamic_dev_dbg(dev, format, ...) do {				    \
	static char mod_name[]						    \
	__attribute__((section("__verbose_strings")))			    \
	 = KBUILD_MODNAME;						    \
	static struct mod_debug descriptor				    \
	__used								    \
	__attribute__((section("__verbose"), aligned(8))) =		    \
	{ mod_name, mod_name, NULL, TYPE_BOOLEAN, DEBUG_HASH, DEBUG_HASH2 };\
	if (__dynamic_dbg_enabled(KBUILD_MODNAME, TYPE_BOOLEAN,		    \
						0, 0, DEBUG_HASH))	    \
			dev_printk(KERN_DEBUG, dev,			    \
					KBUILD_MODNAME ": " format,	    \
					##__VA_ARGS__);			    \
	} while (0)

#else

static inline int unregister_dynamic_debug_module(const char *mod_name)
{
	return 0;
}
static inline int __dynamic_dbg_enabled_helper(char *modname, int type,
						int value, int hash)
{
	return 0;
}

#define __dynamic_dbg_enabled(module, type, value, level, hash)  ({ 0; })
#define dynamic_pr_debug(fmt, ...)  do { } while (0)
#define dynamic_dev_dbg(dev, format, ...)  do { } while (0)
#endif

#endif
