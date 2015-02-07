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
 * Copyright (C) 2004, 2005 MIPS Technologies, Inc.  All rights reserved.
 *
 */

#ifndef __ASM_RTLX_H_
#define __ASM_RTLX_H_

#include <irq.h>

#define LX_NODE_BASE 10

#define MIPS_CPU_RTLX_IRQ 0

#define RTLX_VERSION 2
#define RTLX_xID 0x12345600
#define RTLX_ID (RTLX_xID | RTLX_VERSION)
#define RTLX_CHANNELS 8

#define RTLX_CHANNEL_STDIO	0
#define RTLX_CHANNEL_DBG	1
#define RTLX_CHANNEL_SYSIO	2

extern int rtlx_open(int index, int can_sleep);
extern int rtlx_release(int index);
extern ssize_t rtlx_read(int index, void __user *buff, size_t count);
extern ssize_t rtlx_write(int index, const void __user *buffer, size_t count);
extern unsigned int rtlx_read_poll(int index, int can_sleep);
extern unsigned int rtlx_write_poll(int index);

enum rtlx_state {
	RTLX_STATE_UNUSED = 0,
	RTLX_STATE_INITIALISED,
	RTLX_STATE_REMOTE_READY,
	RTLX_STATE_OPENED
};

#define RTLX_BUFFER_SIZE 2048

/* each channel supports read and write.
   linux (vpe0) reads lx_buffer  and writes rt_buffer
   SP (vpe1) reads rt_buffer and writes lx_buffer
*/
struct rtlx_channel {
	enum rtlx_state rt_state;
	enum rtlx_state lx_state;

	int buffer_size;

	/* read and write indexes per buffer */
	int rt_write, rt_read;
	char *rt_buffer;

	int lx_write, lx_read;
	char *lx_buffer;
};

struct rtlx_info {
	unsigned long id;
	enum rtlx_state state;

	struct rtlx_channel channel[RTLX_CHANNELS];
};

#endif /* __ASM_RTLX_H_ */
