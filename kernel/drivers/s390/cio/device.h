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

#ifndef S390_DEVICE_H
#define S390_DEVICE_H

#include <asm/ccwdev.h>
#include <asm/atomic.h>
#include <linux/wait.h>

#include "io_sch.h"

/*
 * states of the device statemachine
 */
enum dev_state {
	DEV_STATE_NOT_OPER,
	DEV_STATE_SENSE_PGID,
	DEV_STATE_SENSE_ID,
	DEV_STATE_OFFLINE,
	DEV_STATE_VERIFY,
	DEV_STATE_ONLINE,
	DEV_STATE_W4SENSE,
	DEV_STATE_DISBAND_PGID,
	DEV_STATE_BOXED,
	/* states to wait for i/o completion before doing something */
	DEV_STATE_CLEAR_VERIFY,
	DEV_STATE_TIMEOUT_KILL,
	DEV_STATE_QUIESCE,
	/* special states for devices gone not operational */
	DEV_STATE_DISCONNECTED,
	DEV_STATE_DISCONNECTED_SENSE_ID,
	DEV_STATE_CMFCHANGE,
	DEV_STATE_CMFUPDATE,
	/* last element! */
	NR_DEV_STATES
};

/*
 * asynchronous events of the device statemachine
 */
enum dev_event {
	DEV_EVENT_NOTOPER,
	DEV_EVENT_INTERRUPT,
	DEV_EVENT_TIMEOUT,
	DEV_EVENT_VERIFY,
	/* last element! */
	NR_DEV_EVENTS
};

struct ccw_device;

/*
 * action called through jumptable
 */
typedef void (fsm_func_t)(struct ccw_device *, enum dev_event);
extern fsm_func_t *dev_jumptable[NR_DEV_STATES][NR_DEV_EVENTS];

static inline void
dev_fsm_event(struct ccw_device *cdev, enum dev_event dev_event)
{
	dev_jumptable[cdev->private->state][dev_event](cdev, dev_event);
}

/*
 * Delivers 1 if the device state is final.
 */
static inline int
dev_fsm_final_state(struct ccw_device *cdev)
{
	return (cdev->private->state == DEV_STATE_NOT_OPER ||
		cdev->private->state == DEV_STATE_OFFLINE ||
		cdev->private->state == DEV_STATE_ONLINE ||
		cdev->private->state == DEV_STATE_BOXED);
}

extern struct workqueue_struct *ccw_device_work;
extern wait_queue_head_t ccw_device_init_wq;
extern atomic_t ccw_device_init_count;

void io_subchannel_recog_done(struct ccw_device *cdev);
void io_subchannel_init_config(struct subchannel *sch);

int ccw_device_cancel_halt_clear(struct ccw_device *);

void ccw_device_do_unreg_rereg(struct work_struct *);
void ccw_device_move_to_orphanage(struct work_struct *);
int ccw_device_is_orphan(struct ccw_device *);

int ccw_device_recognition(struct ccw_device *);
int ccw_device_online(struct ccw_device *);
int ccw_device_offline(struct ccw_device *);
int ccw_purge_blacklisted(void);

/* Function prototypes for device status and basic sense stuff. */
void ccw_device_accumulate_irb(struct ccw_device *, struct irb *);
void ccw_device_accumulate_basic_sense(struct ccw_device *, struct irb *);
int ccw_device_accumulate_and_sense(struct ccw_device *, struct irb *);
int ccw_device_do_sense(struct ccw_device *, struct irb *);

/* Function prototypes for sense id stuff. */
void ccw_device_sense_id_start(struct ccw_device *);
void ccw_device_sense_id_irq(struct ccw_device *, enum dev_event);
void ccw_device_sense_id_done(struct ccw_device *, int);

/* Function prototypes for path grouping stuff. */
void ccw_device_sense_pgid_start(struct ccw_device *);
void ccw_device_sense_pgid_irq(struct ccw_device *, enum dev_event);
void ccw_device_sense_pgid_done(struct ccw_device *, int);

void ccw_device_verify_start(struct ccw_device *);
void ccw_device_verify_irq(struct ccw_device *, enum dev_event);
void ccw_device_verify_done(struct ccw_device *, int);

void ccw_device_disband_start(struct ccw_device *);
void ccw_device_disband_irq(struct ccw_device *, enum dev_event);
void ccw_device_disband_done(struct ccw_device *, int);

int ccw_device_call_handler(struct ccw_device *);

int ccw_device_stlck(struct ccw_device *);

/* Helper function for machine check handling. */
void ccw_device_trigger_reprobe(struct ccw_device *);
void ccw_device_kill_io(struct ccw_device *);
int ccw_device_notify(struct ccw_device *, int);
void ccw_device_set_notoper(struct ccw_device *cdev);

/* qdio needs this. */
void ccw_device_set_timeout(struct ccw_device *, int);
extern struct subchannel_id ccw_device_get_subchannel_id(struct ccw_device *);
extern struct bus_type ccw_bus_type;

/* Channel measurement facility related */
void retry_set_schib(struct ccw_device *cdev);
void cmf_retry_copy_block(struct ccw_device *);
int cmf_reenable(struct ccw_device *);
extern struct device_attribute dev_attr_cmb_enable;
#endif
