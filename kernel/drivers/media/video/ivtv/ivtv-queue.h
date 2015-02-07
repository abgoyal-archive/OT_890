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
    buffer queues.
    Copyright (C) 2003-2004  Kevin Thayer <nufan_wfk at yahoo.com>
    Copyright (C) 2004  Chris Kennedy <c@groovy.org>
    Copyright (C) 2005-2007  Hans Verkuil <hverkuil@xs4all.nl>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef IVTV_QUEUE_H
#define IVTV_QUEUE_H

#define IVTV_DMA_UNMAPPED	((u32) -1)
#define SLICED_VBI_PIO 0

/* ivtv_buffer utility functions */

static inline int ivtv_might_use_pio(struct ivtv_stream *s)
{
	return s->dma == PCI_DMA_NONE || (SLICED_VBI_PIO && s->type == IVTV_ENC_STREAM_TYPE_VBI);
}

static inline int ivtv_use_pio(struct ivtv_stream *s)
{
	struct ivtv *itv = s->itv;

	return s->dma == PCI_DMA_NONE ||
	    (SLICED_VBI_PIO && s->type == IVTV_ENC_STREAM_TYPE_VBI && itv->vbi.sliced_in->service_set);
}

static inline int ivtv_might_use_dma(struct ivtv_stream *s)
{
	return s->dma != PCI_DMA_NONE;
}

static inline int ivtv_use_dma(struct ivtv_stream *s)
{
	return !ivtv_use_pio(s);
}

static inline void ivtv_buf_sync_for_cpu(struct ivtv_stream *s, struct ivtv_buffer *buf)
{
	if (ivtv_use_dma(s))
		pci_dma_sync_single_for_cpu(s->itv->dev, buf->dma_handle,
				s->buf_size + 256, s->dma);
}

static inline void ivtv_buf_sync_for_device(struct ivtv_stream *s, struct ivtv_buffer *buf)
{
	if (ivtv_use_dma(s))
		pci_dma_sync_single_for_device(s->itv->dev, buf->dma_handle,
				s->buf_size + 256, s->dma);
}

int ivtv_buf_copy_from_user(struct ivtv_stream *s, struct ivtv_buffer *buf, const char __user *src, int copybytes);
void ivtv_buf_swap(struct ivtv_buffer *buf);

/* ivtv_queue utility functions */
void ivtv_queue_init(struct ivtv_queue *q);
void ivtv_enqueue(struct ivtv_stream *s, struct ivtv_buffer *buf, struct ivtv_queue *q);
struct ivtv_buffer *ivtv_dequeue(struct ivtv_stream *s, struct ivtv_queue *q);
int ivtv_queue_move(struct ivtv_stream *s, struct ivtv_queue *from, struct ivtv_queue *steal,
		    struct ivtv_queue *to, int needed_bytes);
void ivtv_flush_queues(struct ivtv_stream *s);

/* ivtv_stream utility functions */
int ivtv_stream_alloc(struct ivtv_stream *s);
void ivtv_stream_free(struct ivtv_stream *s);

static inline void ivtv_stream_sync_for_cpu(struct ivtv_stream *s)
{
	if (ivtv_use_dma(s))
		pci_dma_sync_single_for_cpu(s->itv->dev, s->sg_handle,
			sizeof(struct ivtv_sg_element), PCI_DMA_TODEVICE);
}

static inline void ivtv_stream_sync_for_device(struct ivtv_stream *s)
{
	if (ivtv_use_dma(s))
		pci_dma_sync_single_for_device(s->itv->dev, s->sg_handle,
			sizeof(struct ivtv_sg_element), PCI_DMA_TODEVICE);
}

#endif
