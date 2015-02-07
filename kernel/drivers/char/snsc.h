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
 * SN Platform system controller communication support
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2004-2006 Silicon Graphics, Inc. All rights reserved.
 */

/*
 * This file contains macros and data types for communication with the
 * system controllers in SGI SN systems.
 */

#ifndef _SN_SYSCTL_H_
#define _SN_SYSCTL_H_

#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/wait.h>
#include <linux/kobject.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <asm/sn/types.h>

#define CHUNKSIZE 127

/* This structure is used to track an open subchannel. */
struct subch_data_s {
	nasid_t sd_nasid;	/* node on which the subchannel was opened */
	int sd_subch;		/* subchannel number */
	spinlock_t sd_rlock;	/* monitor lock for rsv */
	spinlock_t sd_wlock;	/* monitor lock for wsv */
	wait_queue_head_t sd_rq;	/* wait queue for readers */
	wait_queue_head_t sd_wq;	/* wait queue for writers */
	struct semaphore sd_rbs;	/* semaphore for read buffer */
	struct semaphore sd_wbs;	/* semaphore for write buffer */

	char sd_rb[CHUNKSIZE];	/* read buffer */
	char sd_wb[CHUNKSIZE];	/* write buffer */
};

struct sysctl_data_s {
	struct cdev scd_cdev;	/* Character device info */
	nasid_t scd_nasid;	/* Node on which subchannels are opened. */
};


/* argument types */
#define IR_ARG_INT              0x00    /* 4-byte integer (big-endian)  */
#define IR_ARG_ASCII            0x01    /* null-terminated ASCII string */
#define IR_ARG_UNKNOWN          0x80    /* unknown data type.  The low
                                         * 7 bits will contain the data
                                         * length.                      */
#define IR_ARG_UNKNOWN_LENGTH_MASK	0x7f


/* system controller event codes */
#define EV_CLASS_MASK		0xf000ul
#define EV_SEVERITY_MASK	0x0f00ul
#define EV_COMPONENT_MASK	0x00fful

#define EV_CLASS_POWER		0x1000ul
#define EV_CLASS_FAN		0x2000ul
#define EV_CLASS_TEMP		0x3000ul
#define EV_CLASS_ENV		0x4000ul
#define EV_CLASS_TEST_FAULT	0x5000ul
#define EV_CLASS_TEST_WARNING	0x6000ul
#define EV_CLASS_PWRD_NOTIFY	0x8000ul

/* ENV class codes */
#define ENV_PWRDN_PEND		0x4101ul

#define EV_SEVERITY_POWER_STABLE	0x0000ul
#define EV_SEVERITY_POWER_LOW_WARNING	0x0100ul
#define EV_SEVERITY_POWER_HIGH_WARNING	0x0200ul
#define EV_SEVERITY_POWER_HIGH_FAULT	0x0300ul
#define EV_SEVERITY_POWER_LOW_FAULT	0x0400ul

#define EV_SEVERITY_FAN_STABLE		0x0000ul
#define EV_SEVERITY_FAN_WARNING		0x0100ul
#define EV_SEVERITY_FAN_FAULT		0x0200ul

#define EV_SEVERITY_TEMP_STABLE		0x0000ul
#define EV_SEVERITY_TEMP_ADVISORY	0x0100ul
#define EV_SEVERITY_TEMP_CRITICAL	0x0200ul
#define EV_SEVERITY_TEMP_FAULT		0x0300ul

void scdrv_event_init(struct sysctl_data_s *);

#endif /* _SN_SYSCTL_H_ */
