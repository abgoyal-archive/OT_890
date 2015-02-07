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
 * Mailbox internal functions
 *
 * Copyright (C) 2006 Nokia Corporation
 * Written by: Hiroshi DOYU <Hiroshi.DOYU@nokia.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#ifndef __ARCH_ARM_PLAT_MAILBOX_H
#define __ARCH_ARM_PLAT_MAILBOX_H

/*
 * Mailbox sequence bit API
 */
#if defined(CONFIG_ARCH_OMAP1)
#  define MBOX_USE_SEQ_BIT
#elif defined(CONFIG_ARCH_OMAP2)
#  define MBOX_USE_SEQ_BIT
#endif

#ifdef MBOX_USE_SEQ_BIT
/* seq_rcv should be initialized with any value other than
 * 0 and 1 << 31, to allow either value for the first
 * message.  */
static inline void mbox_seq_init(struct omap_mbox *mbox)
{
	/* any value other than 0 and 1 << 31 */
	mbox->seq_rcv = 0xffffffff;
}

static inline void mbox_seq_toggle(struct omap_mbox *mbox, mbox_msg_t * msg)
{
	/* add seq_snd to msg */
	*msg = (*msg & 0x7fffffff) | mbox->seq_snd;
	/* flip seq_snd */
	mbox->seq_snd ^= 1 << 31;
}

static inline int mbox_seq_test(struct omap_mbox *mbox, mbox_msg_t msg)
{
	mbox_msg_t seq = msg & (1 << 31);
	if (seq == mbox->seq_rcv)
		return -1;
	mbox->seq_rcv = seq;
	return 0;
}
#else
static inline void mbox_seq_init(struct omap_mbox *mbox)
{
}
static inline void mbox_seq_toggle(struct omap_mbox *mbox, mbox_msg_t * msg)
{
}
static inline int mbox_seq_test(struct omap_mbox *mbox, mbox_msg_t msg)
{
	return 0;
}
#endif

/* Mailbox FIFO handle functions */
static inline mbox_msg_t mbox_fifo_read(struct omap_mbox *mbox)
{
	return mbox->ops->fifo_read(mbox);
}
static inline void mbox_fifo_write(struct omap_mbox *mbox, mbox_msg_t msg)
{
	mbox->ops->fifo_write(mbox, msg);
}
static inline int mbox_fifo_empty(struct omap_mbox *mbox)
{
	return mbox->ops->fifo_empty(mbox);
}
static inline int mbox_fifo_full(struct omap_mbox *mbox)
{
	return mbox->ops->fifo_full(mbox);
}

/* Mailbox IRQ handle functions */
static inline void enable_mbox_irq(struct omap_mbox *mbox, omap_mbox_irq_t irq)
{
	mbox->ops->enable_irq(mbox, irq);
}
static inline void disable_mbox_irq(struct omap_mbox *mbox, omap_mbox_irq_t irq)
{
	mbox->ops->disable_irq(mbox, irq);
}
static inline void ack_mbox_irq(struct omap_mbox *mbox, omap_mbox_irq_t irq)
{
	if (mbox->ops->ack_irq)
		mbox->ops->ack_irq(mbox, irq);
}
static inline int is_mbox_irq(struct omap_mbox *mbox, omap_mbox_irq_t irq)
{
	return mbox->ops->is_irq(mbox, irq);
}

#endif				/* __ARCH_ARM_PLAT_MAILBOX_H */
