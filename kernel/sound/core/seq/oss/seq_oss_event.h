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
 * OSS compatible sequencer driver
 *
 * seq_oss_event.h - OSS event queue record
 *
 * Copyright (C) 1998,99 Takashi Iwai <tiwai@suse.de>
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

#ifndef __SEQ_OSS_EVENT_H
#define __SEQ_OSS_EVENT_H

#include "seq_oss_device.h"

#define SHORT_EVENT_SIZE	4
#define LONG_EVENT_SIZE		8

/* short event (4bytes) */
struct evrec_short {
	unsigned char code;
	unsigned char parm1;
	unsigned char dev;
	unsigned char parm2;
};
	
/* short note events (4bytes) */
struct evrec_note {
	unsigned char code;
	unsigned char chn;
	unsigned char note;
	unsigned char vel;
};
	
/* long timer events (8bytes) */
struct evrec_timer {
	unsigned char code;
	unsigned char cmd;
	unsigned char dummy1, dummy2;
	unsigned int time;
};

/* long extended events (8bytes) */
struct evrec_extended {
	unsigned char code;
	unsigned char cmd;
	unsigned char dev;
	unsigned char chn;
	unsigned char p1, p2, p3, p4;
};

/* long channel events (8bytes) */
struct evrec_long {
	unsigned char code;
	unsigned char dev;
	unsigned char cmd;
	unsigned char chn;
	unsigned char p1, p2;
	unsigned short val;
};
	
/* channel voice events (8bytes) */
struct evrec_voice {
	unsigned char code;
	unsigned char dev;
	unsigned char cmd;
	unsigned char chn;
	unsigned char note, parm;
	unsigned short dummy;
};

/* sysex events (8bytes) */
struct evrec_sysex {
	unsigned char code;
	unsigned char dev;
	unsigned char buf[6];
};

/* event record */
union evrec {
	struct evrec_short s;
	struct evrec_note n;
	struct evrec_long l;
	struct evrec_voice v;
	struct evrec_timer t;
	struct evrec_extended e;
	struct evrec_sysex x;
	unsigned int echo;
	unsigned char c[LONG_EVENT_SIZE];
};

#define ev_is_long(ev) ((ev)->s.code >= 128)
#define ev_length(ev) ((ev)->s.code >= 128 ? LONG_EVENT_SIZE : SHORT_EVENT_SIZE)

int snd_seq_oss_process_event(struct seq_oss_devinfo *dp, union evrec *q, struct snd_seq_event *ev);
int snd_seq_oss_process_timer_event(struct seq_oss_timer *rec, union evrec *q);
int snd_seq_oss_event_input(struct snd_seq_event *ev, int direct, void *private_data, int atomic, int hop);


#endif /* __SEQ_OSS_EVENT_H */
