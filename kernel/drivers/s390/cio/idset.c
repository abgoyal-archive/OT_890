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
 *  drivers/s390/cio/idset.c
 *
 *    Copyright IBM Corp. 2007
 *    Author(s): Peter Oberparleiter <peter.oberparleiter@de.ibm.com>
 */

#include <linux/vmalloc.h>
#include <linux/bitops.h>
#include "idset.h"
#include "css.h"

struct idset {
	int num_ssid;
	int num_id;
	unsigned long bitmap[0];
};

static inline unsigned long bitmap_size(int num_ssid, int num_id)
{
	return __BITOPS_WORDS(num_ssid * num_id) * sizeof(unsigned long);
}

static struct idset *idset_new(int num_ssid, int num_id)
{
	struct idset *set;

	set = vmalloc(sizeof(struct idset) + bitmap_size(num_ssid, num_id));
	if (set) {
		set->num_ssid = num_ssid;
		set->num_id = num_id;
		memset(set->bitmap, 0, bitmap_size(num_ssid, num_id));
	}
	return set;
}

void idset_free(struct idset *set)
{
	vfree(set);
}

void idset_clear(struct idset *set)
{
	memset(set->bitmap, 0, bitmap_size(set->num_ssid, set->num_id));
}

void idset_fill(struct idset *set)
{
	memset(set->bitmap, 0xff, bitmap_size(set->num_ssid, set->num_id));
}

static inline void idset_add(struct idset *set, int ssid, int id)
{
	set_bit(ssid * set->num_id + id, set->bitmap);
}

static inline void idset_del(struct idset *set, int ssid, int id)
{
	clear_bit(ssid * set->num_id + id, set->bitmap);
}

static inline int idset_contains(struct idset *set, int ssid, int id)
{
	return test_bit(ssid * set->num_id + id, set->bitmap);
}

static inline int idset_get_first(struct idset *set, int *ssid, int *id)
{
	int bitnum;

	bitnum = find_first_bit(set->bitmap, set->num_ssid * set->num_id);
	if (bitnum >= set->num_ssid * set->num_id)
		return 0;
	*ssid = bitnum / set->num_id;
	*id = bitnum % set->num_id;
	return 1;
}

struct idset *idset_sch_new(void)
{
	return idset_new(__MAX_SSID + 1, __MAX_SUBCHANNEL + 1);
}

void idset_sch_add(struct idset *set, struct subchannel_id schid)
{
	idset_add(set, schid.ssid, schid.sch_no);
}

void idset_sch_del(struct idset *set, struct subchannel_id schid)
{
	idset_del(set, schid.ssid, schid.sch_no);
}

int idset_sch_contains(struct idset *set, struct subchannel_id schid)
{
	return idset_contains(set, schid.ssid, schid.sch_no);
}

int idset_sch_get_first(struct idset *set, struct subchannel_id *schid)
{
	int ssid = 0;
	int id = 0;
	int rc;

	rc = idset_get_first(set, &ssid, &id);
	if (rc) {
		init_subchannel_id(schid);
		schid->ssid = ssid;
		schid->sch_no = id;
	}
	return rc;
}
