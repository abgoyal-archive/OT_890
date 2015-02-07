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
 * Copyright (C) 2007 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/delay.h>
#include <linux/kdebug.h>
#include <linux/notifier.h>
#include <linux/sched.h>

#include <asm/irq.h>

enum nmi_action {
	NMI_SHOW_STATE	= 1 << 0,
	NMI_SHOW_REGS	= 1 << 1,
	NMI_DIE		= 1 << 2,
	NMI_DEBOUNCE	= 1 << 3,
};

static unsigned long nmi_actions;

static int nmi_debug_notify(struct notifier_block *self,
		unsigned long val, void *data)
{
	struct die_args *args = data;

	if (likely(val != DIE_NMI))
		return NOTIFY_DONE;

	if (nmi_actions & NMI_SHOW_STATE)
		show_state();
	if (nmi_actions & NMI_SHOW_REGS)
		show_regs(args->regs);
	if (nmi_actions & NMI_DEBOUNCE)
		mdelay(10);
	if (nmi_actions & NMI_DIE)
		return NOTIFY_BAD;

	return NOTIFY_OK;
}

static struct notifier_block nmi_debug_nb = {
	.notifier_call = nmi_debug_notify,
};

static int __init nmi_debug_setup(char *str)
{
	char *p, *sep;

	register_die_notifier(&nmi_debug_nb);
	if (nmi_enable()) {
		printk(KERN_WARNING "Unable to enable NMI.\n");
		return 0;
	}

	if (*str != '=')
		return 0;

	for (p = str + 1; *p; p = sep + 1) {
		sep = strchr(p, ',');
		if (sep)
			*sep = 0;
		if (strcmp(p, "state") == 0)
			nmi_actions |= NMI_SHOW_STATE;
		else if (strcmp(p, "regs") == 0)
			nmi_actions |= NMI_SHOW_REGS;
		else if (strcmp(p, "debounce") == 0)
			nmi_actions |= NMI_DEBOUNCE;
		else if (strcmp(p, "die") == 0)
			nmi_actions |= NMI_DIE;
		else
			printk(KERN_WARNING "NMI: Unrecognized action `%s'\n",
				p);
		if (!sep)
			break;
	}

	return 0;
}
__setup("nmi_debug", nmi_debug_setup);
