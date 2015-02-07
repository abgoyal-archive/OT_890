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
 *   ALSA sequencer FIFO
 *   Copyright (c) 1998 by Frank van de Pol <fvdpol@coil.demon.nl>
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
#ifndef __SND_SEQ_FIFO_H
#define __SND_SEQ_FIFO_H

#include "seq_memory.h"
#include "seq_lock.h"


/* === FIFO === */

struct snd_seq_fifo {
	struct snd_seq_pool *pool;		/* FIFO pool */
	struct snd_seq_event_cell *head;    	/* pointer to head of fifo */
	struct snd_seq_event_cell *tail;    	/* pointer to tail of fifo */
	int cells;
	spinlock_t lock;
	snd_use_lock_t use_lock;
	wait_queue_head_t input_sleep;
	atomic_t overflow;

};

/* create new fifo (constructor) */
struct snd_seq_fifo *snd_seq_fifo_new(int poolsize);

/* delete fifo (destructor) */
void snd_seq_fifo_delete(struct snd_seq_fifo **f);


/* enqueue event to fifo */
int snd_seq_fifo_event_in(struct snd_seq_fifo *f, struct snd_seq_event *event);

/* lock fifo from release */
#define snd_seq_fifo_lock(fifo)		snd_use_lock_use(&(fifo)->use_lock)
#define snd_seq_fifo_unlock(fifo)	snd_use_lock_free(&(fifo)->use_lock)

/* get a cell from fifo - fifo should be locked */
int snd_seq_fifo_cell_out(struct snd_seq_fifo *f, struct snd_seq_event_cell **cellp, int nonblock);

/* free dequeued cell - fifo should be locked */
void snd_seq_fifo_cell_putback(struct snd_seq_fifo *f, struct snd_seq_event_cell *cell);

/* clean up queue */
void snd_seq_fifo_clear(struct snd_seq_fifo *f);

/* polling */
int snd_seq_fifo_poll_wait(struct snd_seq_fifo *f, struct file *file, poll_table *wait);

/* resize pool in fifo */
int snd_seq_fifo_resize(struct snd_seq_fifo *f, int poolsize);


#endif
