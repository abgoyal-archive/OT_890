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

/* include/linux/logger.h
 *
 * Copyright (C) 2007-2008 Google, Inc.
 * Author: Robert Love <rlove@android.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _LINUX_LOGGER_H
#define _LINUX_LOGGER_H

#include <linux/types.h>
#include <linux/ioctl.h>

struct logger_entry {
	__u16		len;	/* length of the payload */
	__u16		__pad;	/* no matter what, we get 2 bytes of padding */
	__s32		pid;	/* generating process's pid */
	__s32		tid;	/* generating process's tid */
	__s32		sec;	/* seconds since Epoch */
	__s32		nsec;	/* nanoseconds */
	char		msg[0];	/* the entry's payload */
};

#define LOGGER_LOG_RADIO	"log_radio"	/* radio-related messages */
#define LOGGER_LOG_EVENTS	"log_events"	/* system/hardware events */
#define LOGGER_LOG_MAIN		"log_main"	/* everything else */

#define LOGGER_ENTRY_MAX_LEN		(4*1024)
#define LOGGER_ENTRY_MAX_PAYLOAD	\
	(LOGGER_ENTRY_MAX_LEN - sizeof(struct logger_entry))

#define __LOGGERIO	0xAE

#define LOGGER_GET_LOG_BUF_SIZE		_IO(__LOGGERIO, 1) /* size of log */
#define LOGGER_GET_LOG_LEN		_IO(__LOGGERIO, 2) /* used log len */
#define LOGGER_GET_NEXT_ENTRY_LEN	_IO(__LOGGERIO, 3) /* next entry len */
#define LOGGER_FLUSH_LOG		_IO(__LOGGERIO, 4) /* flush log */

#endif /* _LINUX_LOGGER_H */
