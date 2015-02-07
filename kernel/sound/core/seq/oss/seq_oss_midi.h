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
 * midi device information
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

#ifndef __SEQ_OSS_MIDI_H
#define __SEQ_OSS_MIDI_H

#include "seq_oss_device.h"
#include <sound/seq_oss_legacy.h>

int snd_seq_oss_midi_lookup_ports(int client);
int snd_seq_oss_midi_check_new_port(struct snd_seq_port_info *pinfo);
int snd_seq_oss_midi_check_exit_port(int client, int port);
void snd_seq_oss_midi_clear_all(void);

void snd_seq_oss_midi_setup(struct seq_oss_devinfo *dp);
void snd_seq_oss_midi_cleanup(struct seq_oss_devinfo *dp);

int snd_seq_oss_midi_open(struct seq_oss_devinfo *dp, int dev, int file_mode);
void snd_seq_oss_midi_open_all(struct seq_oss_devinfo *dp, int file_mode);
int snd_seq_oss_midi_close(struct seq_oss_devinfo *dp, int dev);
void snd_seq_oss_midi_reset(struct seq_oss_devinfo *dp, int dev);
int snd_seq_oss_midi_putc(struct seq_oss_devinfo *dp, int dev, unsigned char c,
			  struct snd_seq_event *ev);
int snd_seq_oss_midi_input(struct snd_seq_event *ev, int direct, void *private);
int snd_seq_oss_midi_filemode(struct seq_oss_devinfo *dp, int dev);
int snd_seq_oss_midi_make_info(struct seq_oss_devinfo *dp, int dev, struct midi_info *inf);
void snd_seq_oss_midi_get_addr(struct seq_oss_devinfo *dp, int dev, struct snd_seq_addr *addr);

#endif
