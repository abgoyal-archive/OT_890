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

/* SCTP kernel implementation
 * Copyright (c) 2003 International Business Machines, Corp.
 *
 * This file is part of the SCTP kernel implementation
 *
 * These functions manipulate sctp SSN tracker.
 *
 * This SCTP implementation is free software;
 * you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This SCTP implementation is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 *                 ************************
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU CC; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Please send any bug reports or fixes you make to the
 * email address(es):
 *    lksctp developers <lksctp-developers@lists.sourceforge.net>
 *
 * Or submit a bug report through the following website:
 *    http://www.sf.net/projects/lksctp
 *
 * Written or modified by:
 *    Jon Grimm             <jgrimm@us.ibm.com>
 *
 * Any bugs reported given to us we will try to fix... any fixes shared will
 * be incorporated into the next SCTP release.
 */

#include <linux/types.h>
#include <net/sctp/sctp.h>
#include <net/sctp/sm.h>

#define MAX_KMALLOC_SIZE	131072

static struct sctp_ssnmap *sctp_ssnmap_init(struct sctp_ssnmap *map, __u16 in,
					    __u16 out);

/* Storage size needed for map includes 2 headers and then the
 * specific needs of in or out streams.
 */
static inline size_t sctp_ssnmap_size(__u16 in, __u16 out)
{
	return sizeof(struct sctp_ssnmap) + (in + out) * sizeof(__u16);
}


/* Create a new sctp_ssnmap.
 * Allocate room to store at least 'len' contiguous TSNs.
 */
struct sctp_ssnmap *sctp_ssnmap_new(__u16 in, __u16 out,
				    gfp_t gfp)
{
	struct sctp_ssnmap *retval;
	int size;

	size = sctp_ssnmap_size(in, out);
	if (size <= MAX_KMALLOC_SIZE)
		retval = kmalloc(size, gfp);
	else
		retval = (struct sctp_ssnmap *)
			  __get_free_pages(gfp, get_order(size));
	if (!retval)
		goto fail;

	if (!sctp_ssnmap_init(retval, in, out))
		goto fail_map;

	retval->malloced = 1;
	SCTP_DBG_OBJCNT_INC(ssnmap);

	return retval;

fail_map:
	if (size <= MAX_KMALLOC_SIZE)
		kfree(retval);
	else
		free_pages((unsigned long)retval, get_order(size));
fail:
	return NULL;
}


/* Initialize a block of memory as a ssnmap.  */
static struct sctp_ssnmap *sctp_ssnmap_init(struct sctp_ssnmap *map, __u16 in,
					    __u16 out)
{
	memset(map, 0x00, sctp_ssnmap_size(in, out));

	/* Start 'in' stream just after the map header. */
	map->in.ssn = (__u16 *)&map[1];
	map->in.len = in;

	/* Start 'out' stream just after 'in'. */
	map->out.ssn = &map->in.ssn[in];
	map->out.len = out;

	return map;
}

/* Clear out the ssnmap streams.  */
void sctp_ssnmap_clear(struct sctp_ssnmap *map)
{
	size_t size;

	size = (map->in.len + map->out.len) * sizeof(__u16);
	memset(map->in.ssn, 0x00, size);
}

/* Dispose of a ssnmap.  */
void sctp_ssnmap_free(struct sctp_ssnmap *map)
{
	if (map && map->malloced) {
		int size;

		size = sctp_ssnmap_size(map->in.len, map->out.len);
		if (size <= MAX_KMALLOC_SIZE)
			kfree(map);
		else
			free_pages((unsigned long)map, get_order(size));
		SCTP_DBG_OBJCNT_DEC(ssnmap);
	}
}
