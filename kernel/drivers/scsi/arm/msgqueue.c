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
 *  linux/drivers/acorn/scsi/msgqueue.c
 *
 *  Copyright (C) 1997-1998 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  message queue handling
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stddef.h>
#include <linux/init.h>

#include "msgqueue.h"

/*
 * Function: struct msgqueue_entry *mqe_alloc(MsgQueue_t *msgq)
 * Purpose : Allocate a message queue entry
 * Params  : msgq - message queue to claim entry for
 * Returns : message queue entry or NULL.
 */
static struct msgqueue_entry *mqe_alloc(MsgQueue_t *msgq)
{
	struct msgqueue_entry *mq;

	if ((mq = msgq->free) != NULL)
		msgq->free = mq->next;

	return mq;
}

/*
 * Function: void mqe_free(MsgQueue_t *msgq, struct msgqueue_entry *mq)
 * Purpose : free a message queue entry
 * Params  : msgq - message queue to free entry from
 *	     mq   - message queue entry to free
 */
static void mqe_free(MsgQueue_t *msgq, struct msgqueue_entry *mq)
{
	if (mq) {
		mq->next = msgq->free;
		msgq->free = mq;
	}
}

/*
 * Function: void msgqueue_initialise(MsgQueue_t *msgq)
 * Purpose : initialise a message queue
 * Params  : msgq - queue to initialise
 */
void msgqueue_initialise(MsgQueue_t *msgq)
{
	int i;

	msgq->qe = NULL;
	msgq->free = &msgq->entries[0];

	for (i = 0; i < NR_MESSAGES; i++)
		msgq->entries[i].next = &msgq->entries[i + 1];

	msgq->entries[NR_MESSAGES - 1].next = NULL;
}


/*
 * Function: void msgqueue_free(MsgQueue_t *msgq)
 * Purpose : free a queue
 * Params  : msgq - queue to free
 */
void msgqueue_free(MsgQueue_t *msgq)
{
}

/*
 * Function: int msgqueue_msglength(MsgQueue_t *msgq)
 * Purpose : calculate the total length of all messages on the message queue
 * Params  : msgq - queue to examine
 * Returns : number of bytes of messages in queue
 */
int msgqueue_msglength(MsgQueue_t *msgq)
{
	struct msgqueue_entry *mq = msgq->qe;
	int length = 0;

	for (mq = msgq->qe; mq; mq = mq->next)
		length += mq->msg.length;

	return length;
}

/*
 * Function: struct message *msgqueue_getmsg(MsgQueue_t *msgq, int msgno)
 * Purpose : return a message
 * Params  : msgq   - queue to obtain message from
 *	   : msgno  - message number
 * Returns : pointer to message string, or NULL
 */
struct message *msgqueue_getmsg(MsgQueue_t *msgq, int msgno)
{
	struct msgqueue_entry *mq;

	for (mq = msgq->qe; mq && msgno; mq = mq->next, msgno--);

	return mq ? &mq->msg : NULL;
}

/*
 * Function: int msgqueue_addmsg(MsgQueue_t *msgq, int length, ...)
 * Purpose : add a message onto a message queue
 * Params  : msgq   - queue to add message on
 *	     length - length of message
 *	     ...    - message bytes
 * Returns : != 0 if successful
 */
int msgqueue_addmsg(MsgQueue_t *msgq, int length, ...)
{
	struct msgqueue_entry *mq = mqe_alloc(msgq);
	va_list ap;

	if (mq) {
		struct msgqueue_entry **mqp;
		int i;

		va_start(ap, length);
		for (i = 0; i < length; i++)
			mq->msg.msg[i] = va_arg(ap, unsigned int);
		va_end(ap);

		mq->msg.length = length;
		mq->msg.fifo = 0;
		mq->next = NULL;

		mqp = &msgq->qe;
		while (*mqp)
			mqp = &(*mqp)->next;

		*mqp = mq;
	}

	return mq != NULL;
}

/*
 * Function: void msgqueue_flush(MsgQueue_t *msgq)
 * Purpose : flush all messages from message queue
 * Params  : msgq - queue to flush
 */
void msgqueue_flush(MsgQueue_t *msgq)
{
	struct msgqueue_entry *mq, *mqnext;

	for (mq = msgq->qe; mq; mq = mqnext) {
		mqnext = mq->next;
		mqe_free(msgq, mq);
	}
	msgq->qe = NULL;
}

EXPORT_SYMBOL(msgqueue_initialise);
EXPORT_SYMBOL(msgqueue_free);
EXPORT_SYMBOL(msgqueue_msglength);
EXPORT_SYMBOL(msgqueue_getmsg);
EXPORT_SYMBOL(msgqueue_addmsg);
EXPORT_SYMBOL(msgqueue_flush);

MODULE_AUTHOR("Russell King");
MODULE_DESCRIPTION("SCSI message queue handling");
MODULE_LICENSE("GPL");
