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
 *  drivers/s390/cio/chp.h
 *
 *    Copyright IBM Corp. 2007
 *    Author(s): Peter Oberparleiter <peter.oberparleiter@de.ibm.com>
 */

#ifndef S390_CHP_H
#define S390_CHP_H S390_CHP_H

#include <linux/types.h>
#include <linux/device.h>
#include <asm/chpid.h>
#include "chsc.h"
#include "css.h"

#define CHP_STATUS_STANDBY		0
#define CHP_STATUS_CONFIGURED		1
#define CHP_STATUS_RESERVED		2
#define CHP_STATUS_NOT_RECOGNIZED	3

#define CHP_ONLINE 0
#define CHP_OFFLINE 1
#define CHP_VARY_ON 2
#define CHP_VARY_OFF 3

struct chp_link {
	struct chp_id chpid;
	u32 fla_mask;
	u16 fla;
};

static inline int chp_test_bit(u8 *bitmap, int num)
{
	int byte = num >> 3;
	int mask = 128 >> (num & 7);

	return (bitmap[byte] & mask) ? 1 : 0;
}


struct channel_path {
	struct chp_id chpid;
	int state;
	struct channel_path_desc desc;
	/* Channel-measurement related stuff: */
	int cmg;
	int shared;
	void *cmg_chars;
	struct device dev;
};

int chp_get_status(struct chp_id chpid);
u8 chp_get_sch_opm(struct subchannel *sch);
int chp_is_registered(struct chp_id chpid);
void *chp_get_chp_desc(struct chp_id chpid);
void chp_remove_cmg_attr(struct channel_path *chp);
int chp_add_cmg_attr(struct channel_path *chp);
int chp_new(struct chp_id chpid);
void chp_cfg_schedule(struct chp_id chpid, int configure);
void chp_cfg_cancel_deconfigure(struct chp_id chpid);
int chp_info_get_status(struct chp_id chpid);
int chp_ssd_get_mask(struct chsc_ssd_info *, struct chp_link *);
#endif /* S390_CHP_H */
