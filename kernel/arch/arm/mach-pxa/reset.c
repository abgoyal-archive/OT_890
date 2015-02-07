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
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <asm/proc-fns.h>

#include <mach/pxa-regs.h>
#include <mach/reset.h>

unsigned int reset_status;
EXPORT_SYMBOL(reset_status);

static void do_hw_reset(void);

static int reset_gpio = -1;

int init_gpio_reset(int gpio, int output)
{
	int rc;

	rc = gpio_request(gpio, "reset generator");
	if (rc) {
		printk(KERN_ERR "Can't request reset_gpio\n");
		goto out;
	}

	if (output)
		rc = gpio_direction_output(gpio, 0);
	else
		rc = gpio_direction_input(gpio);
	if (rc) {
		printk(KERN_ERR "Can't configure reset_gpio\n");
		gpio_free(gpio);
		goto out;
	}

out:
	if (!rc)
		reset_gpio = gpio;

	return rc;
}

/*
 * Trigger GPIO reset.
 * This covers various types of logic connecting gpio pin
 * to RESET pins (nRESET or GPIO_RESET):
 */
static void do_gpio_reset(void)
{
	BUG_ON(reset_gpio == -1);

	/* drive it low */
	gpio_direction_output(reset_gpio, 0);
	mdelay(2);
	/* rising edge or drive high */
	gpio_set_value(reset_gpio, 1);
	mdelay(2);
	/* falling edge */
	gpio_set_value(reset_gpio, 0);

	/* give it some time */
	mdelay(10);

	WARN_ON(1);
	/* fallback */
	do_hw_reset();
}

static void do_hw_reset(void)
{
	/* Initialize the watchdog and let it fire */
	OWER = OWER_WME;
	OSSR = OSSR_M3;
	OSMR3 = OSCR + 368640;	/* ... in 100 ms */
}

void arch_reset(char mode)
{
	clear_reset_status(RESET_STATUS_ALL);

	switch (mode) {
	case 's':
		/* Jump into ROM at address 0 */
		cpu_reset(0);
		break;
	case 'g':
		do_gpio_reset();
		break;
	case 'h':
	default:
		do_hw_reset();
		break;
	}
}

