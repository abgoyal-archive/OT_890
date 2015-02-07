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

/* -*- linux-c -*-
 *
 * (C) 2003 zecke@handhelds.org
 *
 * GPL version 2
 *
 * based on arch/arm/kernel/apm.c
 * factor out the information needed by architectures to provide
 * apm status
 */
#ifndef __LINUX_APM_EMULATION_H
#define __LINUX_APM_EMULATION_H

#include <linux/apm_bios.h>

/*
 * This structure gets filled in by the machine specific 'get_power_status'
 * implementation.  Any fields which are not set default to a safe value.
 */
struct apm_power_info {
	unsigned char	ac_line_status;
#define APM_AC_OFFLINE			0
#define APM_AC_ONLINE			1
#define APM_AC_BACKUP			2
#define APM_AC_UNKNOWN			0xff

	unsigned char	battery_status;
#define APM_BATTERY_STATUS_HIGH		0
#define APM_BATTERY_STATUS_LOW		1
#define APM_BATTERY_STATUS_CRITICAL	2
#define APM_BATTERY_STATUS_CHARGING	3
#define APM_BATTERY_STATUS_NOT_PRESENT	4
#define APM_BATTERY_STATUS_UNKNOWN	0xff

	unsigned char	battery_flag;
#define APM_BATTERY_FLAG_HIGH		(1 << 0)
#define APM_BATTERY_FLAG_LOW		(1 << 1)
#define APM_BATTERY_FLAG_CRITICAL	(1 << 2)
#define APM_BATTERY_FLAG_CHARGING	(1 << 3)
#define APM_BATTERY_FLAG_NOT_PRESENT	(1 << 7)
#define APM_BATTERY_FLAG_UNKNOWN	0xff

	int		battery_life;
	int		time;
	int		units;
#define APM_UNITS_MINS			0
#define APM_UNITS_SECS			1
#define APM_UNITS_UNKNOWN		-1

};

/*
 * This allows machines to provide their own "apm get power status" function.
 */
extern void (*apm_get_power_status)(struct apm_power_info *);

/*
 * Queue an event (APM_SYS_SUSPEND or APM_CRITICAL_SUSPEND)
 */
void apm_queue_event(apm_event_t event);

#endif /* __LINUX_APM_EMULATION_H */
