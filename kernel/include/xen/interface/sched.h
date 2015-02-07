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

/******************************************************************************
 * sched.h
 *
 * Scheduler state interactions
 *
 * Copyright (c) 2005, Keir Fraser <keir@xensource.com>
 */

#ifndef __XEN_PUBLIC_SCHED_H__
#define __XEN_PUBLIC_SCHED_H__

#include "event_channel.h"

/*
 * The prototype for this hypercall is:
 *  long sched_op_new(int cmd, void *arg)
 * @cmd == SCHEDOP_??? (scheduler operation).
 * @arg == Operation-specific extra argument(s), as described below.
 *
 * **NOTE**:
 * Versions of Xen prior to 3.0.2 provide only the following legacy version
 * of this hypercall, supporting only the commands yield, block and shutdown:
 *  long sched_op(int cmd, unsigned long arg)
 * @cmd == SCHEDOP_??? (scheduler operation).
 * @arg == 0               (SCHEDOP_yield and SCHEDOP_block)
 *      == SHUTDOWN_* code (SCHEDOP_shutdown)
 */

/*
 * Voluntarily yield the CPU.
 * @arg == NULL.
 */
#define SCHEDOP_yield       0

/*
 * Block execution of this VCPU until an event is received for processing.
 * If called with event upcalls masked, this operation will atomically
 * reenable event delivery and check for pending events before blocking the
 * VCPU. This avoids a "wakeup waiting" race.
 * @arg == NULL.
 */
#define SCHEDOP_block       1

/*
 * Halt execution of this domain (all VCPUs) and notify the system controller.
 * @arg == pointer to sched_shutdown structure.
 */
#define SCHEDOP_shutdown    2
struct sched_shutdown {
    unsigned int reason; /* SHUTDOWN_* */
};
DEFINE_GUEST_HANDLE_STRUCT(sched_shutdown);

/*
 * Poll a set of event-channel ports. Return when one or more are pending. An
 * optional timeout may be specified.
 * @arg == pointer to sched_poll structure.
 */
#define SCHEDOP_poll        3
struct sched_poll {
    GUEST_HANDLE(evtchn_port_t) ports;
    unsigned int nr_ports;
    uint64_t timeout;
};
DEFINE_GUEST_HANDLE_STRUCT(sched_poll);

/*
 * Reason codes for SCHEDOP_shutdown. These may be interpreted by control
 * software to determine the appropriate action. For the most part, Xen does
 * not care about the shutdown code.
 */
#define SHUTDOWN_poweroff   0  /* Domain exited normally. Clean up and kill. */
#define SHUTDOWN_reboot     1  /* Clean up, kill, and then restart.          */
#define SHUTDOWN_suspend    2  /* Clean up, save suspend info, kill.         */
#define SHUTDOWN_crash      3  /* Tell controller we've crashed.             */

#endif /* __XEN_PUBLIC_SCHED_H__ */
