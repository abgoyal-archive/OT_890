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

#ifndef __SOUND_SEQ_OSS_H
#define __SOUND_SEQ_OSS_H

/*
 * OSS compatible sequencer driver
 *
 * Copyright (C) 1998,99 Takashi Iwai
 *
 * This program is free software; you can redistribute it and/or modify
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include "asequencer.h"
#include "seq_kernel.h"

/*
 * argument structure for synthesizer operations
 */
struct snd_seq_oss_arg {
	/* given by OSS sequencer */
	int app_index;	/* application unique index */
	int file_mode;	/* file mode - see below */
	int seq_mode;	/* sequencer mode - see below */

	/* following must be initialized in open callback */
	struct snd_seq_addr addr;	/* opened port address */
	void *private_data;	/* private data for lowlevel drivers */

	/* note-on event passing mode: initially given by OSS seq,
	 * but configurable by drivers - see below
	 */
	int event_passing;
};


/*
 * synthesizer operation callbacks
 */
struct snd_seq_oss_callback {
	struct module *owner;
	int (*open)(struct snd_seq_oss_arg *p, void *closure);
	int (*close)(struct snd_seq_oss_arg *p);
	int (*ioctl)(struct snd_seq_oss_arg *p, unsigned int cmd, unsigned long arg);
	int (*load_patch)(struct snd_seq_oss_arg *p, int format, const char __user *buf, int offs, int count);
	int (*reset)(struct snd_seq_oss_arg *p);
	int (*raw_event)(struct snd_seq_oss_arg *p, unsigned char *data);
};

/* flag: file_mode */
#define SNDRV_SEQ_OSS_FILE_ACMODE		3
#define SNDRV_SEQ_OSS_FILE_READ		1
#define SNDRV_SEQ_OSS_FILE_WRITE		2
#define SNDRV_SEQ_OSS_FILE_NONBLOCK	4

/* flag: seq_mode */
#define SNDRV_SEQ_OSS_MODE_SYNTH		0
#define SNDRV_SEQ_OSS_MODE_MUSIC		1

/* flag: event_passing */
#define SNDRV_SEQ_OSS_PROCESS_EVENTS	0	/* key == 255 is processed as velocity change */
#define SNDRV_SEQ_OSS_PASS_EVENTS		1	/* pass all events to callback */
#define SNDRV_SEQ_OSS_PROCESS_KEYPRESS	2	/* key >= 128 will be processed as key-pressure */

/* default control rate: fixed */
#define SNDRV_SEQ_OSS_CTRLRATE		100

/* default max queue length: configurable by module option */
#define SNDRV_SEQ_OSS_MAX_QLEN		1024


/*
 * data pointer to snd_seq_register_device
 */
struct snd_seq_oss_reg {
	int type;
	int subtype;
	int nvoices;
	struct snd_seq_oss_callback oper;
	void *private_data;
};

/* device id */
#define SNDRV_SEQ_DEV_ID_OSS		"seq-oss"

#endif /* __SOUND_SEQ_OSS_H */
