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

#include <linux/kernel.h>
#include <linux/ide.h>
#include <linux/jiffies.h>
#include <linux/blkdev.h>

DECLARE_WAIT_QUEUE_HEAD(ide_park_wq);

static void issue_park_cmd(ide_drive_t *drive, unsigned long timeout)
{
	ide_hwif_t *hwif = drive->hwif;
	struct request_queue *q = drive->queue;
	struct request *rq;
	int rc;

	timeout += jiffies;
	spin_lock_irq(&hwif->lock);
	if (drive->dev_flags & IDE_DFLAG_PARKED) {
		int reset_timer = time_before(timeout, drive->sleep);
		int start_queue = 0;

		drive->sleep = timeout;
		wake_up_all(&ide_park_wq);
		if (reset_timer && del_timer(&hwif->timer))
			start_queue = 1;
		spin_unlock_irq(&hwif->lock);

		if (start_queue) {
			spin_lock_irq(q->queue_lock);
			blk_start_queueing(q);
			spin_unlock_irq(q->queue_lock);
		}
		return;
	}
	spin_unlock_irq(&hwif->lock);

	rq = blk_get_request(q, READ, __GFP_WAIT);
	rq->cmd[0] = REQ_PARK_HEADS;
	rq->cmd_len = 1;
	rq->cmd_type = REQ_TYPE_SPECIAL;
	rq->special = &timeout;
	rc = blk_execute_rq(q, NULL, rq, 1);
	blk_put_request(rq);
	if (rc)
		goto out;

	/*
	 * Make sure that *some* command is sent to the drive after the
	 * timeout has expired, so power management will be reenabled.
	 */
	rq = blk_get_request(q, READ, GFP_NOWAIT);
	if (unlikely(!rq))
		goto out;

	rq->cmd[0] = REQ_UNPARK_HEADS;
	rq->cmd_len = 1;
	rq->cmd_type = REQ_TYPE_SPECIAL;
	elv_add_request(q, rq, ELEVATOR_INSERT_FRONT, 1);

out:
	return;
}

ssize_t ide_park_show(struct device *dev, struct device_attribute *attr,
		      char *buf)
{
	ide_drive_t *drive = to_ide_device(dev);
	ide_hwif_t *hwif = drive->hwif;
	unsigned long now;
	unsigned int msecs;

	if (drive->dev_flags & IDE_DFLAG_NO_UNLOAD)
		return -EOPNOTSUPP;

	spin_lock_irq(&hwif->lock);
	now = jiffies;
	if (drive->dev_flags & IDE_DFLAG_PARKED &&
	    time_after(drive->sleep, now))
		msecs = jiffies_to_msecs(drive->sleep - now);
	else
		msecs = 0;
	spin_unlock_irq(&hwif->lock);

	return snprintf(buf, 20, "%u\n", msecs);
}

ssize_t ide_park_store(struct device *dev, struct device_attribute *attr,
		       const char *buf, size_t len)
{
#define MAX_PARK_TIMEOUT 30000
	ide_drive_t *drive = to_ide_device(dev);
	long int input;
	int rc;

	rc = strict_strtol(buf, 10, &input);
	if (rc || input < -2)
		return -EINVAL;
	if (input > MAX_PARK_TIMEOUT) {
		input = MAX_PARK_TIMEOUT;
		rc = -EOVERFLOW;
	}

	mutex_lock(&ide_setting_mtx);
	if (input >= 0) {
		if (drive->dev_flags & IDE_DFLAG_NO_UNLOAD)
			rc = -EOPNOTSUPP;
		else if (input || drive->dev_flags & IDE_DFLAG_PARKED)
			issue_park_cmd(drive, msecs_to_jiffies(input));
	} else {
		if (drive->media == ide_disk)
			switch (input) {
			case -1:
				drive->dev_flags &= ~IDE_DFLAG_NO_UNLOAD;
				break;
			case -2:
				drive->dev_flags |= IDE_DFLAG_NO_UNLOAD;
				break;
			}
		else
			rc = -EOPNOTSUPP;
	}
	mutex_unlock(&ide_setting_mtx);

	return rc ? rc : len;
}
