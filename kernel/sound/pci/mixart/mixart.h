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
 * Driver for Digigram miXart soundcards
 *
 * main header file
 *
 * Copyright (c) 2003 by Digigram <alsa@digigram.com>
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
 */

#ifndef __SOUND_MIXART_H
#define __SOUND_MIXART_H

#include <linux/interrupt.h>
#include <linux/mutex.h>
#include <sound/pcm.h>

#define MIXART_DRIVER_VERSION	0x000100	/* 0.1.0 */


/*
 */

struct mixart_uid {
	u32 object_id;
	u32 desc;
};

struct mem_area {
	unsigned long phys;
	void __iomem *virt;
	struct resource *res;
};


struct mixart_route {
	unsigned char connected;
	unsigned char phase_inv;
	int volume;
};


/* firmware status codes  */
#define MIXART_MOTHERBOARD_XLX_INDEX  0
#define MIXART_MOTHERBOARD_ELF_INDEX  1
#define MIXART_AESEBUBOARD_XLX_INDEX  2
#define MIXART_HARDW_FILES_MAX_INDEX  3  /* xilinx, elf, AESEBU xilinx */

#define MIXART_MAX_CARDS	4
#define MSG_FIFO_SIZE           16

#define MIXART_MAX_PHYS_CONNECTORS  (MIXART_MAX_CARDS * 2 * 2) /* 4 * stereo * (analog+digital) */

struct mixart_mgr {
	unsigned int num_cards;
	struct snd_mixart *chip[MIXART_MAX_CARDS];

	struct pci_dev *pci;

	int irq;

	/* memory-maps */
	struct mem_area mem[2];

	/* share the name */
	char shortname[32];         /* short name of this soundcard */
	char longname[80];          /* name of this soundcard */

	/* message tasklet */
	struct tasklet_struct msg_taskq;

	/* one and only blocking message or notification may be pending  */
	u32 pending_event;
	wait_queue_head_t msg_sleep;

	/* messages stored for tasklet */
	u32 msg_fifo[MSG_FIFO_SIZE];
	int msg_fifo_readptr;
	int msg_fifo_writeptr;
	atomic_t msg_processed;       /* number of messages to be processed in takslet */

	spinlock_t lock;              /* interrupt spinlock */
	spinlock_t msg_lock;          /* mailbox spinlock */
	struct mutex msg_mutex;   /* mutex for blocking_requests */

	struct mutex setup_mutex; /* mutex used in hw_params, open and close */

	/* hardware interface */
	unsigned int dsp_loaded;      /* bit flags of loaded dsp indices */
	unsigned int board_type;      /* read from embedded once elf file is loaded, 250 = miXart8, 251 = with AES, 252 = with Cobranet */

	struct snd_dma_buffer flowinfo;
	struct snd_dma_buffer bufferinfo;

	struct mixart_uid         uid_console_manager;
	int sample_rate;
	int ref_count_rate;

	struct mutex mixer_mutex; /* mutex for mixer */

};


#define MIXART_STREAM_STATUS_FREE	0
#define MIXART_STREAM_STATUS_OPEN	1
#define MIXART_STREAM_STATUS_RUNNING	2
#define MIXART_STREAM_STATUS_DRAINING	3
#define MIXART_STREAM_STATUS_PAUSE	4

#define MIXART_PLAYBACK_STREAMS		4
#define MIXART_CAPTURE_STREAMS		1

#define MIXART_PCM_ANALOG		0
#define MIXART_PCM_DIGITAL		1
#define MIXART_PCM_TOTAL		2

#define MIXART_MAX_STREAM_PER_CARD  (MIXART_PCM_TOTAL * (MIXART_PLAYBACK_STREAMS + MIXART_CAPTURE_STREAMS) )


#define MIXART_NOTIFY_CARD_MASK		0xF000
#define MIXART_NOTIFY_CARD_OFFSET	12
#define MIXART_NOTIFY_PCM_MASK		0x0F00
#define MIXART_NOTIFY_PCM_OFFSET	8
#define MIXART_NOTIFY_CAPT_MASK		0x0080
#define MIXART_NOTIFY_SUBS_MASK		0x007F


struct mixart_stream {
	struct snd_pcm_substream *substream;
	struct mixart_pipe *pipe;
	int pcm_number;

	int status;      /* nothing, running, draining */

	u64  abs_period_elapsed;  /* last absolute stream position where period_elapsed was called (multiple of runtime->period_size) */
	u32  buf_periods;         /* periods counter in the buffer (< runtime->periods) */
	u32  buf_period_frag;     /* defines with buf_period_pos the exact position in the buffer (< runtime->period_size) */

	int channels;
};


enum mixart_pipe_status {
	PIPE_UNDEFINED,
	PIPE_STOPPED,
	PIPE_RUNNING,
	PIPE_CLOCK_SET
};

struct mixart_pipe {
	struct mixart_uid group_uid;			/* id of the pipe, as returned by embedded */
	int          stream_count;
	struct mixart_uid uid_left_connector;	/* UID's for the audio connectors */
	struct mixart_uid uid_right_connector;
	enum mixart_pipe_status status;
	int references;             /* number of subs openned */
	int monitoring;             /* pipe used for monitoring issue */
};


struct snd_mixart {
	struct snd_card *card;
	struct mixart_mgr *mgr;
	int chip_idx;               /* zero based */
	struct snd_hwdep *hwdep;	    /* DSP loader, only for the first card */

	struct snd_pcm *pcm;             /* PCM analog i/o */
	struct snd_pcm *pcm_dig;         /* PCM digital i/o */

	/* allocate stereo pipe for instance */
	struct mixart_pipe pipe_in_ana;
	struct mixart_pipe pipe_out_ana;

	/* if AES/EBU daughter board is available, additional pipes possible on pcm_dig */
	struct mixart_pipe pipe_in_dig;
	struct mixart_pipe pipe_out_dig;

	struct mixart_stream playback_stream[MIXART_PCM_TOTAL][MIXART_PLAYBACK_STREAMS]; /* 0 = pcm, 1 = pcm_dig */
	struct mixart_stream capture_stream[MIXART_PCM_TOTAL];                           /* 0 = pcm, 1 = pcm_dig */

	/* UID's for the physical io's */
	struct mixart_uid uid_out_analog_physio;
	struct mixart_uid uid_in_analog_physio;

	int analog_playback_active[2];		/* Mixer : Master Playback active (!mute) */
	int analog_playback_volume[2];		/* Mixer : Master Playback Volume */
	int analog_capture_volume[2];		/* Mixer : Master Capture Volume */
	int digital_playback_active[2*MIXART_PLAYBACK_STREAMS][2];	/* Mixer : Digital Playback Active [(analog+AES output)*streams][stereo]*/
	int digital_playback_volume[2*MIXART_PLAYBACK_STREAMS][2];	/* Mixer : Digital Playback Volume [(analog+AES output)*streams][stereo]*/
	int digital_capture_volume[2][2];	/* Mixer : Digital Capture Volume [analog+AES output][stereo] */
	int monitoring_active[2];		/* Mixer : Monitoring Active */
	int monitoring_volume[2];		/* Mixer : Monitoring Volume */
};

struct mixart_bufferinfo
{
	u32 buffer_address;
	u32 reserved[5];
	u32 available_length;
	u32 buffer_id;
};

struct mixart_flowinfo
{
	u32 bufferinfo_array_phy_address;
	u32 reserved[11];
	u32 bufferinfo_count;
	u32 capture;
};

/* exported */
int snd_mixart_create_pcm(struct snd_mixart * chip);
struct mixart_pipe *snd_mixart_add_ref_pipe(struct snd_mixart *chip, int pcm_number, int capture, int monitoring);
int snd_mixart_kill_ref_pipe(struct mixart_mgr *mgr, struct mixart_pipe *pipe, int monitoring);

#endif /* __SOUND_MIXART_H */
