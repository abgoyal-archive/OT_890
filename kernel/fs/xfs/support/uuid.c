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
 * Copyright (c) 2000-2003,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <xfs.h>

static DEFINE_MUTEX(uuid_monitor);
static int	uuid_table_size;
static uuid_t	*uuid_table;

/* IRIX interpretation of an uuid_t */
typedef struct {
	__be32	uu_timelow;
	__be16	uu_timemid;
	__be16	uu_timehi;
	__be16	uu_clockseq;
	__be16	uu_node[3];
} xfs_uu_t;

/*
 * uuid_getnodeuniq - obtain the node unique fields of a UUID.
 *
 * This is not in any way a standard or condoned UUID function;
 * it just something that's needed for user-level file handles.
 */
void
uuid_getnodeuniq(uuid_t *uuid, int fsid [2])
{
	xfs_uu_t *uup = (xfs_uu_t *)uuid;

	fsid[0] = (be16_to_cpu(uup->uu_clockseq) << 16) |
		   be16_to_cpu(uup->uu_timemid);
	fsid[1] = be32_to_cpu(uup->uu_timelow);
}

void
uuid_create_nil(uuid_t *uuid)
{
	memset(uuid, 0, sizeof(*uuid));
}

int
uuid_is_nil(uuid_t *uuid)
{
	int	i;
	char	*cp = (char *)uuid;

	if (uuid == NULL)
		return 0;
	/* implied check of version number here... */
	for (i = 0; i < sizeof *uuid; i++)
		if (*cp++) return 0;	/* not nil */
	return 1;	/* is nil */
}

int
uuid_equal(uuid_t *uuid1, uuid_t *uuid2)
{
	return memcmp(uuid1, uuid2, sizeof(uuid_t)) ? 0 : 1;
}

/*
 * Given a 128-bit uuid, return a 64-bit value by adding the top and bottom
 * 64-bit words.  NOTE: This function can not be changed EVER.  Although
 * brain-dead, some applications depend on this 64-bit value remaining
 * persistent.  Specifically, DMI vendors store the value as a persistent
 * filehandle.
 */
__uint64_t
uuid_hash64(uuid_t *uuid)
{
	__uint64_t	*sp = (__uint64_t *)uuid;

	return sp[0] + sp[1];
}

int
uuid_table_insert(uuid_t *uuid)
{
	int	i, hole;

	mutex_lock(&uuid_monitor);
	for (i = 0, hole = -1; i < uuid_table_size; i++) {
		if (uuid_is_nil(&uuid_table[i])) {
			hole = i;
			continue;
		}
		if (uuid_equal(uuid, &uuid_table[i])) {
			mutex_unlock(&uuid_monitor);
			return 0;
		}
	}
	if (hole < 0) {
		uuid_table = kmem_realloc(uuid_table,
			(uuid_table_size + 1) * sizeof(*uuid_table),
			uuid_table_size  * sizeof(*uuid_table),
			KM_SLEEP);
		hole = uuid_table_size++;
	}
	uuid_table[hole] = *uuid;
	mutex_unlock(&uuid_monitor);
	return 1;
}

void
uuid_table_remove(uuid_t *uuid)
{
	int	i;

	mutex_lock(&uuid_monitor);
	for (i = 0; i < uuid_table_size; i++) {
		if (uuid_is_nil(&uuid_table[i]))
			continue;
		if (!uuid_equal(uuid, &uuid_table[i]))
			continue;
		uuid_create_nil(&uuid_table[i]);
		break;
	}
	ASSERT(i < uuid_table_size);
	mutex_unlock(&uuid_monitor);
}
