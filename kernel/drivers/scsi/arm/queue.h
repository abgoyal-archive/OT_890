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
 *  linux/drivers/acorn/scsi/queue.h: queue handling
 *
 *  Copyright (C) 1997 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef QUEUE_H
#define QUEUE_H

typedef struct {
	struct list_head head;
	struct list_head free;
	spinlock_t queue_lock;
	void *alloc;			/* start of allocated mem */
} Queue_t;

/*
 * Function: void queue_initialise (Queue_t *queue)
 * Purpose : initialise a queue
 * Params  : queue - queue to initialise
 */
extern int queue_initialise (Queue_t *queue);

/*
 * Function: void queue_free (Queue_t *queue)
 * Purpose : free a queue
 * Params  : queue - queue to free
 */
extern void queue_free (Queue_t *queue);

/*
 * Function: struct scsi_cmnd *queue_remove (queue)
 * Purpose : removes first SCSI command from a queue
 * Params  : queue   - queue to remove command from
 * Returns : struct scsi_cmnd if successful (and a reference), or NULL if no command available
 */
extern struct scsi_cmnd *queue_remove (Queue_t *queue);

/*
 * Function: struct scsi_cmnd *queue_remove_exclude_ref (queue, exclude)
 * Purpose : remove a SCSI command from a queue
 * Params  : queue   - queue to remove command from
 *	     exclude - array of busy LUNs
 * Returns : struct scsi_cmnd if successful (and a reference), or NULL if no command available
 */
extern struct scsi_cmnd *queue_remove_exclude(Queue_t *queue,
					      unsigned long *exclude);

#define queue_add_cmd_ordered(queue,SCpnt) \
	__queue_add(queue,SCpnt,(SCpnt)->cmnd[0] == REQUEST_SENSE)
#define queue_add_cmd_tail(queue,SCpnt) \
	__queue_add(queue,SCpnt,0)
/*
 * Function: int __queue_add(Queue_t *queue, struct scsi_cmnd *SCpnt, int head)
 * Purpose : Add a new command onto a queue
 * Params  : queue - destination queue
 *	     SCpnt - command to add
 *	     head  - add command to head of queue
 * Returns : 0 on error, !0 on success
 */
extern int __queue_add(Queue_t *queue, struct scsi_cmnd *SCpnt, int head);

/*
 * Function: struct scsi_cmnd *queue_remove_tgtluntag (queue, target, lun, tag)
 * Purpose : remove a SCSI command from the queue for a specified target/lun/tag
 * Params  : queue  - queue to remove command from
 *	     target - target that we want
 *	     lun    - lun on device
 *	     tag    - tag on device
 * Returns : struct scsi_cmnd if successful, or NULL if no command satisfies requirements
 */
extern struct scsi_cmnd *queue_remove_tgtluntag(Queue_t *queue, int target,
						int lun, int tag);

/*
 * Function: queue_remove_all_target(queue, target)
 * Purpose : remove all SCSI commands from the queue for a specified target
 * Params  : queue  - queue to remove command from
 *           target - target device id
 * Returns : nothing
 */
extern void queue_remove_all_target(Queue_t *queue, int target);

/*
 * Function: int queue_probetgtlun (queue, target, lun)
 * Purpose : check to see if we have a command in the queue for the specified
 *	     target/lun.
 * Params  : queue  - queue to look in
 *	     target - target we want to probe
 *	     lun    - lun on target
 * Returns : 0 if not found, != 0 if found
 */
extern int queue_probetgtlun (Queue_t *queue, int target, int lun);

/*
 * Function: int queue_remove_cmd (Queue_t *queue, struct scsi_cmnd *SCpnt)
 * Purpose : remove a specific command from the queues
 * Params  : queue - queue to look in
 *	     SCpnt - command to find
 * Returns : 0 if not found
 */
int queue_remove_cmd(Queue_t *queue, struct scsi_cmnd *SCpnt);

#endif /* QUEUE_H */
