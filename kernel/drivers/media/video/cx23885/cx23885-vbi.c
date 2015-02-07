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
 *  Driver for the Conexant CX23885 PCIe bridge
 *
 *  Copyright (c) 2007 Steven Toth <stoth@linuxtv.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/slab.h>

#include "cx23885.h"

static unsigned int vbibufs = 4;
module_param(vbibufs, int, 0644);
MODULE_PARM_DESC(vbibufs, "number of vbi buffers, range 2-32");

static unsigned int vbi_debug;
module_param(vbi_debug, int, 0644);
MODULE_PARM_DESC(vbi_debug, "enable debug messages [vbi]");

#define dprintk(level, fmt, arg...)\
	do { if (vbi_debug >= level)\
		printk(KERN_DEBUG "%s/0: " fmt, dev->name, ## arg);\
	} while (0)

/* ------------------------------------------------------------------ */

int cx23885_vbi_fmt(struct file *file, void *priv,
	struct v4l2_format *f)
{
	struct cx23885_fh *fh = priv;
	struct cx23885_dev *dev = fh->dev;

	if (dev->tvnorm & V4L2_STD_525_60) {
		/* ntsc */
		f->fmt.vbi.sampling_rate = 28636363;
		f->fmt.vbi.start[0] = 10;
		f->fmt.vbi.start[1] = 273;

	} else if (dev->tvnorm & V4L2_STD_625_50) {
		/* pal */
		f->fmt.vbi.sampling_rate = 35468950;
		f->fmt.vbi.start[0] = 7 - 1;
		f->fmt.vbi.start[1] = 319 - 1;
	}
	return 0;
}

static int cx23885_start_vbi_dma(struct cx23885_dev    *dev,
			 struct cx23885_dmaqueue *q,
			 struct cx23885_buffer   *buf)
{
	/* setup fifo + format */
	cx23885_sram_channel_setup(dev, &dev->sram_channels[SRAM_CH02],
				buf->vb.width, buf->risc.dma);

	/* reset counter */
	q->count = 1;

	/* enable irqs */
	cx_set(PCI_INT_MSK, cx_read(PCI_INT_MSK) | 0x01);
	cx_set(VID_A_INT_MSK, 0x000022);

	/* start dma */
	cx_set(DEV_CNTRL2, (1<<5));
	cx_set(VID_A_DMA_CTL, 0x00000022);

	return 0;
}


static int cx23885_restart_vbi_queue(struct cx23885_dev    *dev,
			     struct cx23885_dmaqueue *q)
{
	struct cx23885_buffer *buf;
	struct list_head *item;

	if (list_empty(&q->active))
		return 0;

	buf = list_entry(q->active.next, struct cx23885_buffer, vb.queue);
	dprintk(2, "restart_queue [%p/%d]: restart dma\n",
		buf, buf->vb.i);
	cx23885_start_vbi_dma(dev, q, buf);
	list_for_each(item, &q->active) {
		buf = list_entry(item, struct cx23885_buffer, vb.queue);
		buf->count = q->count++;
	}
	mod_timer(&q->timeout, jiffies+BUFFER_TIMEOUT);
	return 0;
}

void cx23885_vbi_timeout(unsigned long data)
{
	struct cx23885_dev *dev = (struct cx23885_dev *)data;
	struct cx23885_dmaqueue *q = &dev->vbiq;
	struct cx23885_buffer *buf;
	unsigned long flags;

	cx23885_sram_channel_dump(dev, &dev->sram_channels[SRAM_CH02]);

	cx_clear(VID_A_DMA_CTL, 0x22);

	spin_lock_irqsave(&dev->slock, flags);
	while (!list_empty(&q->active)) {
		buf = list_entry(q->active.next, struct cx23885_buffer,
			vb.queue);
		list_del(&buf->vb.queue);
		buf->vb.state = VIDEOBUF_ERROR;
		wake_up(&buf->vb.done);
		printk("%s/0: [%p/%d] timeout - dma=0x%08lx\n", dev->name,
		       buf, buf->vb.i, (unsigned long)buf->risc.dma);
	}
	cx23885_restart_vbi_queue(dev, q);
	spin_unlock_irqrestore(&dev->slock, flags);
}

/* ------------------------------------------------------------------ */
#define VBI_LINE_LENGTH 2048
#define VBI_LINE_COUNT 17

static int
vbi_setup(struct videobuf_queue *q, unsigned int *count, unsigned int *size)
{
	*size = VBI_LINE_COUNT * VBI_LINE_LENGTH * 2;
	if (0 == *count)
		*count = vbibufs;
	if (*count < 2)
		*count = 2;
	if (*count > 32)
		*count = 32;
	return 0;
}

static int
vbi_prepare(struct videobuf_queue *q, struct videobuf_buffer *vb,
	    enum v4l2_field field)
{
	struct cx23885_fh *fh  = q->priv_data;
	struct cx23885_dev *dev = fh->dev;
	struct cx23885_buffer *buf = container_of(vb,
		struct cx23885_buffer, vb);
	struct videobuf_dmabuf *dma = videobuf_to_dma(&buf->vb);
	unsigned int size;
	int rc;

	size = VBI_LINE_COUNT * VBI_LINE_LENGTH * 2;
	if (0 != buf->vb.baddr  &&  buf->vb.bsize < size)
		return -EINVAL;

	if (VIDEOBUF_NEEDS_INIT == buf->vb.state) {
		buf->vb.width  = VBI_LINE_LENGTH;
		buf->vb.height = VBI_LINE_COUNT;
		buf->vb.size   = size;
		buf->vb.field  = V4L2_FIELD_SEQ_TB;

		rc = videobuf_iolock(q, &buf->vb, NULL);
		if (0 != rc)
			goto fail;
		cx23885_risc_buffer(dev->pci, &buf->risc,
				 dma->sglist,
				 0, buf->vb.width * buf->vb.height,
				 buf->vb.width, 0,
				 buf->vb.height);
	}
	buf->vb.state = VIDEOBUF_PREPARED;
	return 0;

 fail:
	cx23885_free_buffer(q, buf);
	return rc;
}

static void
vbi_queue(struct videobuf_queue *vq, struct videobuf_buffer *vb)
{
	struct cx23885_buffer   *buf =
		container_of(vb, struct cx23885_buffer, vb);
	struct cx23885_buffer   *prev;
	struct cx23885_fh       *fh   = vq->priv_data;
	struct cx23885_dev      *dev  = fh->dev;
	struct cx23885_dmaqueue *q    = &dev->vbiq;

	/* add jump to stopper */
	buf->risc.jmp[0] = cpu_to_le32(RISC_JUMP | RISC_IRQ1 | RISC_CNT_INC);
	buf->risc.jmp[1] = cpu_to_le32(q->stopper.dma);
	buf->risc.jmp[2] = cpu_to_le32(0); /* bits 63-32 */

	if (list_empty(&q->active)) {
		list_add_tail(&buf->vb.queue, &q->active);
		cx23885_start_vbi_dma(dev, q, buf);
		buf->vb.state = VIDEOBUF_ACTIVE;
		buf->count    = q->count++;
		mod_timer(&q->timeout, jiffies+BUFFER_TIMEOUT);
		dprintk(2, "[%p/%d] vbi_queue - first active\n",
			buf, buf->vb.i);

	} else {
		prev = list_entry(q->active.prev, struct cx23885_buffer,
			vb.queue);
		list_add_tail(&buf->vb.queue, &q->active);
		buf->vb.state = VIDEOBUF_ACTIVE;
		buf->count    = q->count++;
		prev->risc.jmp[1] = cpu_to_le32(buf->risc.dma);
		prev->risc.jmp[2] = cpu_to_le32(0); /* Bits 63-32 */
		dprintk(2, "[%p/%d] buffer_queue - append to active\n",
			buf, buf->vb.i);
	}
}

static void vbi_release(struct videobuf_queue *q, struct videobuf_buffer *vb)
{
	struct cx23885_buffer *buf =
		container_of(vb, struct cx23885_buffer, vb);

	cx23885_free_buffer(q, buf);
}

struct videobuf_queue_ops cx23885_vbi_qops = {
	.buf_setup    = vbi_setup,
	.buf_prepare  = vbi_prepare,
	.buf_queue    = vbi_queue,
	.buf_release  = vbi_release,
};

/* ------------------------------------------------------------------ */
/*
 * Local variables:
 * c-basic-offset: 8
 * End:
 */
