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
 * A simple kernel FIFO implementation.
 *
 * Copyright (C) 2004 Stelian Pop <stelian@popies.net>
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
#ifndef _LINUX_KFIFO_H
#define _LINUX_KFIFO_H

#include <linux/kernel.h>
#include <linux/spinlock.h>

struct kfifo {
	unsigned char *buffer;	/* the buffer holding the data */
	unsigned int size;	/* the size of the allocated buffer */
	unsigned int in;	/* data is added at offset (in % size) */
	unsigned int out;	/* data is extracted from off. (out % size) */
	spinlock_t *lock;	/* protects concurrent modifications */
};

extern struct kfifo *kfifo_init(unsigned char *buffer, unsigned int size,
				gfp_t gfp_mask, spinlock_t *lock);
extern struct kfifo *kfifo_alloc(unsigned int size, gfp_t gfp_mask,
				 spinlock_t *lock);
extern void kfifo_free(struct kfifo *fifo);
extern unsigned int __kfifo_put(struct kfifo *fifo,
				unsigned char *buffer, unsigned int len);
extern unsigned int __kfifo_get(struct kfifo *fifo,
				unsigned char *buffer, unsigned int len);

/**
 * __kfifo_reset - removes the entire FIFO contents, no locking version
 * @fifo: the fifo to be emptied.
 */
static inline void __kfifo_reset(struct kfifo *fifo)
{
	fifo->in = fifo->out = 0;
}

/**
 * kfifo_reset - removes the entire FIFO contents
 * @fifo: the fifo to be emptied.
 */
static inline void kfifo_reset(struct kfifo *fifo)
{
	unsigned long flags;

	spin_lock_irqsave(fifo->lock, flags);

	__kfifo_reset(fifo);

	spin_unlock_irqrestore(fifo->lock, flags);
}

/**
 * kfifo_put - puts some data into the FIFO
 * @fifo: the fifo to be used.
 * @buffer: the data to be added.
 * @len: the length of the data to be added.
 *
 * This function copies at most @len bytes from the @buffer into
 * the FIFO depending on the free space, and returns the number of
 * bytes copied.
 */
static inline unsigned int kfifo_put(struct kfifo *fifo,
				     unsigned char *buffer, unsigned int len)
{
	unsigned long flags;
	unsigned int ret;

	spin_lock_irqsave(fifo->lock, flags);

	ret = __kfifo_put(fifo, buffer, len);

	spin_unlock_irqrestore(fifo->lock, flags);

	return ret;
}

/**
 * kfifo_get - gets some data from the FIFO
 * @fifo: the fifo to be used.
 * @buffer: where the data must be copied.
 * @len: the size of the destination buffer.
 *
 * This function copies at most @len bytes from the FIFO into the
 * @buffer and returns the number of copied bytes.
 */
static inline unsigned int kfifo_get(struct kfifo *fifo,
				     unsigned char *buffer, unsigned int len)
{
	unsigned long flags;
	unsigned int ret;

	spin_lock_irqsave(fifo->lock, flags);

	ret = __kfifo_get(fifo, buffer, len);

	/*
	 * optimization: if the FIFO is empty, set the indices to 0
	 * so we don't wrap the next time
	 */
	if (fifo->in == fifo->out)
		fifo->in = fifo->out = 0;

	spin_unlock_irqrestore(fifo->lock, flags);

	return ret;
}

/**
 * __kfifo_len - returns the number of bytes available in the FIFO, no locking version
 * @fifo: the fifo to be used.
 */
static inline unsigned int __kfifo_len(struct kfifo *fifo)
{
	return fifo->in - fifo->out;
}

/**
 * kfifo_len - returns the number of bytes available in the FIFO
 * @fifo: the fifo to be used.
 */
static inline unsigned int kfifo_len(struct kfifo *fifo)
{
	unsigned long flags;
	unsigned int ret;

	spin_lock_irqsave(fifo->lock, flags);

	ret = __kfifo_len(fifo);

	spin_unlock_irqrestore(fifo->lock, flags);

	return ret;
}

#endif
