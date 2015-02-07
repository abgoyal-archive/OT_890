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
 * IR port driver for the Cirrus Logic EP7211 processor.
 *
 * Copyright 2001, Blue Mug Inc.  All rights reserved.
 * Copyright 2007, Samuel Ortiz <samuel@sortiz.org>
 */
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/tty.h>
#include <linux/init.h>
#include <linux/spinlock.h>

#include <net/irda/irda.h>
#include <net/irda/irda_device.h>

#include <asm/io.h>
#include <mach/hardware.h>

#include "sir-dev.h"

#define MIN_DELAY 25      /* 15 us, but wait a little more to be sure */
#define MAX_DELAY 10000   /* 1 ms */

static int ep7211_open(struct sir_dev *dev);
static int ep7211_close(struct sir_dev *dev);
static int ep7211_change_speed(struct sir_dev *dev, unsigned speed);
static int ep7211_reset(struct sir_dev *dev);

static struct dongle_driver ep7211 = {
	.owner		= THIS_MODULE,
	.driver_name	= "EP7211 IR driver",
	.type		= IRDA_EP7211_DONGLE,
	.open		= ep7211_open,
	.close		= ep7211_close,
	.reset		= ep7211_reset,
	.set_speed	= ep7211_change_speed,
};

static int __init ep7211_sir_init(void)
{
	return irda_register_dongle(&ep7211);
}

static void __exit ep7211_sir_cleanup(void)
{
	irda_unregister_dongle(&ep7211);
}

static int ep7211_open(struct sir_dev *dev)
{
	unsigned int syscon;

	/* Turn on the SIR encoder. */
	syscon = clps_readl(SYSCON1);
	syscon |= SYSCON1_SIREN;
	clps_writel(syscon, SYSCON1);

	return 0;
}

static int ep7211_close(struct sir_dev *dev)
{
	unsigned int syscon;

	/* Turn off the SIR encoder. */
	syscon = clps_readl(SYSCON1);
	syscon &= ~SYSCON1_SIREN;
	clps_writel(syscon, SYSCON1);

	return 0;
}

static int ep7211_change_speed(struct sir_dev *dev, unsigned speed)
{
	return 0;
}

static int ep7211_reset(struct sir_dev *dev)
{
	return 0;
}

MODULE_AUTHOR("Samuel Ortiz <samuel@sortiz.org>");
MODULE_DESCRIPTION("EP7211 IR dongle driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("irda-dongle-13"); /* IRDA_EP7211_DONGLE */

module_init(ep7211_sir_init);
module_exit(ep7211_sir_cleanup);
