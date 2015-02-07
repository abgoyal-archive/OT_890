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
 *	Zorro Device Name Tables
 *
 *	Copyright (C) 1999--2000 Geert Uytterhoeven
 *
 *	Based on the PCI version:
 *
 *	Copyright 1992--1999 Drew Eckhardt, Frederic Potter,
 *	David Mosberger-Tang, Martin Mares
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/zorro.h>


#ifdef CONFIG_ZORRO_NAMES

struct zorro_prod_info {
	__u16 prod;
	unsigned short seen;
	const char *name;
};

struct zorro_manuf_info {
	__u16 manuf;
	unsigned short nr;
	const char *name;
	struct zorro_prod_info *prods;
};

/*
 * This is ridiculous, but we want the strings in
 * the .init section so that they don't take up
 * real memory.. Parse the same file multiple times
 * to get all the info.
 */
#define MANUF( manuf, name )		static char __manufstr_##manuf[] __initdata = name;
#define ENDMANUF()
#define PRODUCT( manuf, prod, name ) 	static char __prodstr_##manuf##prod[] __initdata = name;
#include "devlist.h"


#define MANUF( manuf, name )		static struct zorro_prod_info __prods_##manuf[] __initdata = {
#define ENDMANUF()			};
#define PRODUCT( manuf, prod, name )	{ 0x##prod, 0, __prodstr_##manuf##prod },
#include "devlist.h"

static struct zorro_manuf_info __initdata zorro_manuf_list[] = {
#define MANUF( manuf, name )		{ 0x##manuf, sizeof(__prods_##manuf) / sizeof(struct zorro_prod_info), __manufstr_##manuf, __prods_##manuf },
#define ENDMANUF()
#define PRODUCT( manuf, prod, name )
#include "devlist.h"
};

#define MANUFS (sizeof(zorro_manuf_list)/sizeof(struct zorro_manuf_info))

void __init zorro_name_device(struct zorro_dev *dev)
{
	const struct zorro_manuf_info *manuf_p = zorro_manuf_list;
	int i = MANUFS;
	char *name = dev->name;

	do {
		if (manuf_p->manuf == ZORRO_MANUF(dev->id))
			goto match_manuf;
		manuf_p++;
	} while (--i);

	/* Couldn't find either the manufacturer nor the product */
	sprintf(name, "Zorro device %08x", dev->id);
	return;

	match_manuf: {
		struct zorro_prod_info *prod_p = manuf_p->prods;
		int i = manuf_p->nr;

		while (i > 0) {
			if (prod_p->prod ==
			    ((ZORRO_PROD(dev->id)<<8) | ZORRO_EPC(dev->id)))
				goto match_prod;
			prod_p++;
			i--;
		}

		/* Ok, found the manufacturer, but unknown product */
		sprintf(name, "Zorro device %08x (%s)", dev->id, manuf_p->name);
		return;

		/* Full match */
		match_prod: {
			char *n = name + sprintf(name, "%s %s", manuf_p->name, prod_p->name);
			int nr = prod_p->seen + 1;
			prod_p->seen = nr;
			if (nr > 1)
				sprintf(n, " (#%d)", nr);
		}
	}
}

#else

void __init zorro_name_device(struct zorro_dev *dev)
{
}

#endif
