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

/**
 * @file me_slist.c
 *
 * @brief Implements the subdevice list class.
 * @note Copyright (C) 2007 Meilhaus Electronic GmbH (support@meilhaus.de)
 * @author Guenter Gebhardt
 */

/*
 * Copyright (C) 2007 Meilhaus Electronic GmbH (support@meilhaus.de)
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "meerror.h"
#include "medefines.h"

#include "meslist.h"
#include "medebug.h"

int me_slist_query_number_subdevices(struct me_slist *slist, int *number)
{
	PDEBUG_LOCKS("called.\n");
	*number = slist->n;
	return ME_ERRNO_SUCCESS;
}

unsigned int me_slist_get_number_subdevices(struct me_slist *slist)
{
	PDEBUG_LOCKS("called.\n");
	return slist->n;
}

me_subdevice_t *me_slist_get_subdevice(struct me_slist * slist,
				       unsigned int index)
{

	struct list_head *pos;
	me_subdevice_t *subdevice = NULL;
	unsigned int i = 0;

	PDEBUG_LOCKS("called.\n");

	if (index >= slist->n) {
		PERROR("Index out of range.\n");
		return NULL;
	}

	list_for_each(pos, &slist->head) {
		if (i == index) {
			subdevice = list_entry(pos, me_subdevice_t, list);
			break;
		}

		++i;
	}

	return subdevice;
}

int me_slist_get_subdevice_by_type(struct me_slist *slist,
				   unsigned int start_subdevice,
				   int type, int subtype, int *subdevice)
{
	me_subdevice_t *pos;
	int s_type, s_subtype;
	unsigned int index = 0;

	PDEBUG_LOCKS("called.\n");

	if (start_subdevice >= slist->n) {
		PERROR("Start index out of range.\n");
		return ME_ERRNO_NOMORE_SUBDEVICE_TYPE;
	}

	list_for_each_entry(pos, &slist->head, list) {
		if (index < start_subdevice) {	// Go forward to start subdevice.
			++index;
			continue;
		}

		pos->me_subdevice_query_subdevice_type(pos,
						       &s_type, &s_subtype);

		if (subtype == ME_SUBTYPE_ANY) {
			if (s_type == type)
				break;
		} else {
			if ((s_type == type) && (s_subtype == subtype))
				break;
		}

		++index;
	}

	if (index >= slist->n) {
		return ME_ERRNO_NOMORE_SUBDEVICE_TYPE;
	}

	*subdevice = index;

	return ME_ERRNO_SUCCESS;
}

void me_slist_add_subdevice_tail(struct me_slist *slist,
				 me_subdevice_t * subdevice)
{
	PDEBUG_LOCKS("called.\n");

	list_add_tail(&subdevice->list, &slist->head);
	++slist->n;
}

me_subdevice_t *me_slist_del_subdevice_tail(struct me_slist *slist)
{

	struct list_head *last;
	me_subdevice_t *subdevice;

	PDEBUG_LOCKS("called.\n");

	if (list_empty(&slist->head))
		return NULL;

	last = slist->head.prev;

	subdevice = list_entry(last, me_subdevice_t, list);

	list_del(last);

	--slist->n;

	return subdevice;
}

int me_slist_init(me_slist_t * slist)
{
	PDEBUG_LOCKS("called.\n");

	INIT_LIST_HEAD(&slist->head);
	slist->n = 0;
	return 0;
}

void me_slist_deinit(me_slist_t * slist)
{

	struct list_head *s;
	me_subdevice_t *subdevice;

	PDEBUG_LOCKS("called.\n");

	while (!list_empty(&slist->head)) {
		s = slist->head.next;
		list_del(s);
		subdevice = list_entry(s, me_subdevice_t, list);
		subdevice->me_subdevice_destructor(subdevice);
	}

	slist->n = 0;
}
