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
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{linux.intel,addtoit}.com)
 * Licensed under the GPL
 */

#include <linux/interrupt.h>
#include "irq_kern.h"
#include "os.h"
#include "sigio.h"

/* Protected by sigio_lock() called from write_sigio_workaround */
static int sigio_irq_fd = -1;

static irqreturn_t sigio_interrupt(int irq, void *data)
{
	char c;

	os_read_file(sigio_irq_fd, &c, sizeof(c));
	reactivate_fd(sigio_irq_fd, SIGIO_WRITE_IRQ);
	return IRQ_HANDLED;
}

int write_sigio_irq(int fd)
{
	int err;

	err = um_request_irq(SIGIO_WRITE_IRQ, fd, IRQ_READ, sigio_interrupt,
			     IRQF_DISABLED|IRQF_SAMPLE_RANDOM, "write sigio",
			     NULL);
	if (err) {
		printk(KERN_ERR "write_sigio_irq : um_request_irq failed, "
		       "err = %d\n", err);
		return -1;
	}
	sigio_irq_fd = fd;
	return 0;
}

/* These are called from os-Linux/sigio.c to protect its pollfds arrays. */
static DEFINE_SPINLOCK(sigio_spinlock);

void sigio_lock(void)
{
	spin_lock(&sigio_spinlock);
}

void sigio_unlock(void)
{
	spin_unlock(&sigio_spinlock);
}
