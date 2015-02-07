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
 *  drivers/s390/char/sclp_quiesce.c
 *     signal quiesce handler
 *
 *  (C) Copyright IBM Corp. 1999,2004
 *  Author(s): Martin Schwidefsky <schwidefsky@de.ibm.com>
 *             Peter Oberparleiter <peter.oberparleiter@de.ibm.com>
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/cpumask.h>
#include <linux/smp.h>
#include <linux/init.h>
#include <linux/reboot.h>
#include <asm/atomic.h>
#include <asm/ptrace.h>
#include <asm/sigp.h>
#include <asm/smp.h>

#include "sclp.h"

/* Shutdown handler. Signal completion of shutdown by loading special PSW. */
static void
do_machine_quiesce(void)
{
	psw_t quiesce_psw;

	smp_send_stop();
	quiesce_psw.mask = PSW_BASE_BITS | PSW_MASK_WAIT;
	quiesce_psw.addr = 0xfff;
	__load_psw(quiesce_psw);
}

/* Handler for quiesce event. Start shutdown procedure. */
static void
sclp_quiesce_handler(struct evbuf_header *evbuf)
{
	_machine_restart = (void *) do_machine_quiesce;
	_machine_halt = do_machine_quiesce;
	_machine_power_off = do_machine_quiesce;
	ctrl_alt_del();
}

static struct sclp_register sclp_quiesce_event = {
	.receive_mask = EVTYP_SIGQUIESCE_MASK,
	.receiver_fn = sclp_quiesce_handler
};

/* Initialize quiesce driver. */
static int __init
sclp_quiesce_init(void)
{
	return sclp_register(&sclp_quiesce_event);
}

module_init(sclp_quiesce_init);
