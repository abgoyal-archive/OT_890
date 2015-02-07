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

/* mailbox.h */

#ifndef MAILBOX_H
#define MAILBOX_H

#include <linux/wait.h>
#include <linux/workqueue.h>
#include <linux/blkdev.h>

typedef u32 mbox_msg_t;
typedef void (mbox_receiver_t)(mbox_msg_t msg);
struct omap_mbox;

typedef int __bitwise omap_mbox_irq_t;
#define IRQ_TX ((__force omap_mbox_irq_t) 1)
#define IRQ_RX ((__force omap_mbox_irq_t) 2)

typedef int __bitwise omap_mbox_type_t;
#define OMAP_MBOX_TYPE1 ((__force omap_mbox_type_t) 1)
#define OMAP_MBOX_TYPE2 ((__force omap_mbox_type_t) 2)

struct omap_mbox_ops {
	omap_mbox_type_t	type;
	int		(*startup)(struct omap_mbox *mbox);
	void		(*shutdown)(struct omap_mbox *mbox);
	/* fifo */
	mbox_msg_t	(*fifo_read)(struct omap_mbox *mbox);
	void		(*fifo_write)(struct omap_mbox *mbox, mbox_msg_t msg);
	int		(*fifo_empty)(struct omap_mbox *mbox);
	int		(*fifo_full)(struct omap_mbox *mbox);
	/* irq */
	void		(*enable_irq)(struct omap_mbox *mbox, omap_mbox_irq_t irq);
	void		(*disable_irq)(struct omap_mbox *mbox, omap_mbox_irq_t irq);
	void		(*ack_irq)(struct omap_mbox *mbox, omap_mbox_irq_t irq);
	int		(*is_irq)(struct omap_mbox *mbox, omap_mbox_irq_t irq);
};

struct omap_mbox_queue {
	spinlock_t		lock;
	struct request_queue	*queue;
	struct work_struct	work;
	int	(*callback)(void *);
	struct omap_mbox	*mbox;
};

struct omap_mbox {
	char			*name;
	unsigned int		irq;

	struct omap_mbox_queue	*txq, *rxq;

	struct omap_mbox_ops	*ops;

	mbox_msg_t		seq_snd, seq_rcv;

	struct device		dev;

	struct omap_mbox	*next;
	void			*priv;

	void			(*err_notify)(void);
};

int omap_mbox_msg_send(struct omap_mbox *, mbox_msg_t msg, void *);
void omap_mbox_init_seq(struct omap_mbox *);

struct omap_mbox *omap_mbox_get(const char *);
void omap_mbox_put(struct omap_mbox *);

int omap_mbox_register(struct omap_mbox *);
int omap_mbox_unregister(struct omap_mbox *);

#endif /* MAILBOX_H */
