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
 *  linux/drivers/acorn/scsi/msgqueue.h
 *
 *  Copyright (C) 1997 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  message queue handling
 */
#ifndef MSGQUEUE_H
#define MSGQUEUE_H

struct message {
    char msg[8];
    int length;
    int fifo;
};

struct msgqueue_entry {
    struct message msg;
    struct msgqueue_entry *next;
};

#define NR_MESSAGES 4

typedef struct {
    struct msgqueue_entry *qe;
    struct msgqueue_entry *free;
    struct msgqueue_entry entries[NR_MESSAGES];
} MsgQueue_t;

/*
 * Function: void msgqueue_initialise(MsgQueue_t *msgq)
 * Purpose : initialise a message queue
 * Params  : msgq - queue to initialise
 */
extern void msgqueue_initialise(MsgQueue_t *msgq);

/*
 * Function: void msgqueue_free(MsgQueue_t *msgq)
 * Purpose : free a queue
 * Params  : msgq - queue to free
 */
extern void msgqueue_free(MsgQueue_t *msgq);

/*
 * Function: int msgqueue_msglength(MsgQueue_t *msgq)
 * Purpose : calculate the total length of all messages on the message queue
 * Params  : msgq - queue to examine
 * Returns : number of bytes of messages in queue
 */
extern int msgqueue_msglength(MsgQueue_t *msgq);

/*
 * Function: struct message *msgqueue_getmsg(MsgQueue_t *msgq, int msgno)
 * Purpose : return a message & its length
 * Params  : msgq   - queue to obtain message from
 *         : msgno  - message number
 * Returns : pointer to message string, or NULL
 */
extern struct message *msgqueue_getmsg(MsgQueue_t *msgq, int msgno);

/*
 * Function: int msgqueue_addmsg(MsgQueue_t *msgq, int length, ...)
 * Purpose : add a message onto a message queue
 * Params  : msgq   - queue to add message on
 *	     length - length of message
 *	     ...    - message bytes
 * Returns : != 0 if successful
 */
extern int msgqueue_addmsg(MsgQueue_t *msgq, int length, ...);

/*
 * Function: void msgqueue_flush(MsgQueue_t *msgq)
 * Purpose : flush all messages from message queue
 * Params  : msgq - queue to flush
 */
extern void msgqueue_flush(MsgQueue_t *msgq);

#endif
