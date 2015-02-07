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
 * compat.c - A series of functions to make it easier to convert drivers that use
 *            the old isapnp APIs. If possible use the new APIs instead.
 *
 * Copyright 2002 Adam Belay <ambx1@neo.rr.com>
 */

#include <linux/module.h>
#include <linux/isapnp.h>
#include <linux/string.h>

static void pnp_convert_id(char *buf, unsigned short vendor,
			   unsigned short device)
{
	sprintf(buf, "%c%c%c%x%x%x%x",
		'A' + ((vendor >> 2) & 0x3f) - 1,
		'A' + (((vendor & 3) << 3) | ((vendor >> 13) & 7)) - 1,
		'A' + ((vendor >> 8) & 0x1f) - 1,
		(device >> 4) & 0x0f, device & 0x0f,
		(device >> 12) & 0x0f, (device >> 8) & 0x0f);
}

struct pnp_card *pnp_find_card(unsigned short vendor, unsigned short device,
			       struct pnp_card *from)
{
	char id[8];
	char any[8];
	struct list_head *list;

	pnp_convert_id(id, vendor, device);
	pnp_convert_id(any, ISAPNP_ANY_ID, ISAPNP_ANY_ID);

	list = from ? from->global_list.next : pnp_cards.next;

	while (list != &pnp_cards) {
		struct pnp_card *card = global_to_pnp_card(list);

		if (compare_pnp_id(card->id, id) || (memcmp(id, any, 7) == 0))
			return card;
		list = list->next;
	}
	return NULL;
}

struct pnp_dev *pnp_find_dev(struct pnp_card *card, unsigned short vendor,
			     unsigned short function, struct pnp_dev *from)
{
	char id[8];
	char any[8];

	pnp_convert_id(id, vendor, function);
	pnp_convert_id(any, ISAPNP_ANY_ID, ISAPNP_ANY_ID);
	if (card == NULL) {	/* look for a logical device from all cards */
		struct list_head *list;

		list = pnp_global.next;
		if (from)
			list = from->global_list.next;

		while (list != &pnp_global) {
			struct pnp_dev *dev = global_to_pnp_dev(list);

			if (compare_pnp_id(dev->id, id) ||
			    (memcmp(id, any, 7) == 0))
				return dev;
			list = list->next;
		}
	} else {
		struct list_head *list;

		list = card->devices.next;
		if (from) {
			list = from->card_list.next;
			if (from->card != card)	/* something is wrong */
				return NULL;
		}
		while (list != &card->devices) {
			struct pnp_dev *dev = card_to_pnp_dev(list);

			if (compare_pnp_id(dev->id, id))
				return dev;
			list = list->next;
		}
	}
	return NULL;
}

EXPORT_SYMBOL(pnp_find_card);
EXPORT_SYMBOL(pnp_find_dev);
