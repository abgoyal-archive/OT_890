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
 * PC-Speaker driver for Linux
 *
 * Copyright (C) 1993-1997  Michael Beck
 * Copyright (C) 1997-2001  David Woodhouse
 * Copyright (C) 2001-2008  Stas Sergeev
 */

#ifndef __PCSP_H__
#define __PCSP_H__

#include <linux/hrtimer.h>
#if defined(CONFIG_MIPS) || defined(CONFIG_X86)
/* Use the global PIT lock ! */
#include <asm/i8253.h>
#else
#include <asm/8253pit.h>
static DEFINE_SPINLOCK(i8253_lock);
#endif

#define PCSP_SOUND_VERSION 0x400	/* read 4.00 */
#define PCSP_DEBUG 0

/* default timer freq for PC-Speaker: 18643 Hz */
#define DIV_18KHZ 64
#define MAX_DIV DIV_18KHZ
#define CALC_DIV(d) (MAX_DIV >> (d))
#define CUR_DIV() CALC_DIV(chip->treble)
#define PCSP_MAX_TREBLE 1

/* unfortunately, with hrtimers 37KHz does not work very well :( */
#define PCSP_DEFAULT_TREBLE 0
#define MIN_DIV (MAX_DIV >> PCSP_MAX_TREBLE)

/* wild guess */
#define PCSP_MIN_LPJ 1000000
#define PCSP_DEFAULT_SDIV (DIV_18KHZ >> 1)
#define PCSP_DEFAULT_SRATE (PIT_TICK_RATE / PCSP_DEFAULT_SDIV)
#define PCSP_INDEX_INC() (1 << (PCSP_MAX_TREBLE - chip->treble))
#define PCSP_CALC_RATE(i) (PIT_TICK_RATE / CALC_DIV(i))
#define PCSP_RATE() PCSP_CALC_RATE(chip->treble)
#define PCSP_MIN_RATE__1 MAX_DIV/PIT_TICK_RATE
#define PCSP_MAX_RATE__1 MIN_DIV/PIT_TICK_RATE
#define PCSP_MAX_PERIOD_NS (1000000000ULL * PCSP_MIN_RATE__1)
#define PCSP_MIN_PERIOD_NS (1000000000ULL * PCSP_MAX_RATE__1)
#define PCSP_CALC_NS(div) ({ \
	u64 __val = 1000000000ULL * (div); \
	do_div(__val, PIT_TICK_RATE); \
	__val; \
})
#define PCSP_PERIOD_NS() PCSP_CALC_NS(CUR_DIV())

#define PCSP_MAX_PERIOD_SIZE	(64*1024)
#define PCSP_MAX_PERIODS	512
#define PCSP_BUFFER_SIZE	(128*1024)

struct snd_pcsp {
	struct snd_card *card;
	struct snd_pcm *pcm;
	struct input_dev *input_dev;
	struct hrtimer timer;
	unsigned short port, irq, dma;
	spinlock_t substream_lock;
	struct snd_pcm_substream *playback_substream;
	unsigned int fmt_size;
	unsigned int is_signed;
	size_t playback_ptr;
	size_t period_ptr;
	atomic_t timer_active;
	int thalf;
	u64 ns_rem;
	unsigned char val61;
	int enable;
	int max_treble;
	int treble;
	int pcspkr;
};

extern struct snd_pcsp pcsp_chip;

extern enum hrtimer_restart pcsp_do_timer(struct hrtimer *handle);
extern void pcsp_sync_stop(struct snd_pcsp *chip);

extern int snd_pcsp_new_pcm(struct snd_pcsp *chip);
extern int snd_pcsp_new_mixer(struct snd_pcsp *chip);

#endif
