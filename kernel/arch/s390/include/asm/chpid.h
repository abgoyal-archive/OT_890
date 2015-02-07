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
 *  drivers/s390/cio/chpid.h
 *
 *    Copyright IBM Corp. 2007
 *    Author(s): Peter Oberparleiter <peter.oberparleiter@de.ibm.com>
 */

#ifndef _ASM_S390_CHPID_H
#define _ASM_S390_CHPID_H _ASM_S390_CHPID_H

#include <linux/string.h>
#include <linux/types.h>

#define __MAX_CHPID 255

struct chp_id {
	u8 reserved1;
	u8 cssid;
	u8 reserved2;
	u8 id;
} __attribute__((packed));

#ifdef __KERNEL__
#include <asm/cio.h>

static inline void chp_id_init(struct chp_id *chpid)
{
	memset(chpid, 0, sizeof(struct chp_id));
}

static inline int chp_id_is_equal(struct chp_id *a, struct chp_id *b)
{
	return (a->id == b->id) && (a->cssid == b->cssid);
}

static inline void chp_id_next(struct chp_id *chpid)
{
	if (chpid->id < __MAX_CHPID)
		chpid->id++;
	else {
		chpid->id = 0;
		chpid->cssid++;
	}
}

static inline int chp_id_is_valid(struct chp_id *chpid)
{
	return (chpid->cssid <= __MAX_CSSID);
}


#define chp_id_for_each(c) \
	for (chp_id_init(c); chp_id_is_valid(c); chp_id_next(c))
#endif /* __KERNEL */

#endif /* _ASM_S390_CHPID_H */
