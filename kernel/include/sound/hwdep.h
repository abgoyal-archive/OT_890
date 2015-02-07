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

#ifndef __SOUND_HWDEP_H
#define __SOUND_HWDEP_H

/*
 *  Hardware dependent layer 
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
 *
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#include <sound/asound.h>
#include <linux/poll.h>

struct snd_hwdep;

struct snd_hwdep_ops {
	long long (*llseek) (struct snd_hwdep *hw, struct file * file, long long offset, int orig);
	long (*read) (struct snd_hwdep *hw, char __user *buf, long count, loff_t *offset);
	long (*write) (struct snd_hwdep *hw, const char __user *buf, long count, loff_t *offset);
	int (*open) (struct snd_hwdep * hw, struct file * file);
	int (*release) (struct snd_hwdep *hw, struct file * file);
	unsigned int (*poll) (struct snd_hwdep *hw, struct file * file, poll_table * wait);
	int (*ioctl) (struct snd_hwdep *hw, struct file * file, unsigned int cmd, unsigned long arg);
	int (*ioctl_compat) (struct snd_hwdep *hw, struct file * file, unsigned int cmd, unsigned long arg);
	int (*mmap) (struct snd_hwdep *hw, struct file * file, struct vm_area_struct * vma);
	int (*dsp_status) (struct snd_hwdep *hw, struct snd_hwdep_dsp_status *status);
	int (*dsp_load) (struct snd_hwdep *hw, struct snd_hwdep_dsp_image *image);
};

struct snd_hwdep {
	struct snd_card *card;
	struct list_head list;
	int device;
	char id[32];
	char name[80];
	int iface;

#ifdef CONFIG_SND_OSSEMUL
	char oss_dev[32];
	int oss_type;
	int ossreg;
#endif

	struct snd_hwdep_ops ops;
	wait_queue_head_t open_wait;
	void *private_data;
	void (*private_free) (struct snd_hwdep *hwdep);

	struct mutex open_mutex;
	int used;
	unsigned int dsp_loaded;
	unsigned int exclusive: 1;
};

extern int snd_hwdep_new(struct snd_card *card, char *id, int device,
			 struct snd_hwdep **rhwdep);

#endif /* __SOUND_HWDEP_H */
