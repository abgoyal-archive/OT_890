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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Reset an IP27.
 *
 * Copyright (C) 1997, 1998, 1999, 2000, 06 by Ralf Baechle
 * Copyright (C) 1999, 2000 Silicon Graphics, Inc.
 */
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/smp.h>
#include <linux/mmzone.h>
#include <linux/nodemask.h>
#include <linux/pm.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/reboot.h>
#include <asm/system.h>
#include <asm/sgialib.h>
#include <asm/sn/addrs.h>
#include <asm/sn/arch.h>
#include <asm/sn/gda.h>
#include <asm/sn/sn0/hub.h>

void machine_restart(char *command) __attribute__((noreturn));
void machine_halt(void) __attribute__((noreturn));
void machine_power_off(void) __attribute__((noreturn));

#define noreturn while(1);				/* Silence gcc.  */

/* XXX How to pass the reboot command to the firmware??? */
static void ip27_machine_restart(char *command)
{
#if 0
	int i;
#endif

	printk("Reboot started from CPU %d\n", smp_processor_id());
#ifdef CONFIG_SMP
	smp_send_stop();
#endif
#if 0
	for_each_online_node(i)
		REMOTE_HUB_S(COMPACT_TO_NASID_NODEID(i), PROMOP_REG,
							PROMOP_REBOOT);
#else
	LOCAL_HUB_S(NI_PORT_RESET, NPR_PORTRESET | NPR_LOCALRESET);
#endif
	noreturn;
}

static void ip27_machine_halt(void)
{
	int i;

#ifdef CONFIG_SMP
	smp_send_stop();
#endif
	for_each_online_node(i)
		REMOTE_HUB_S(COMPACT_TO_NASID_NODEID(i), PROMOP_REG,
							PROMOP_RESTART);
	LOCAL_HUB_S(NI_PORT_RESET, NPR_PORTRESET | NPR_LOCALRESET);
	noreturn;
}

static void ip27_machine_power_off(void)
{
	/* To do ...  */
	noreturn;
}

void ip27_reboot_setup(void)
{
	_machine_restart = ip27_machine_restart;
	_machine_halt = ip27_machine_halt;
	pm_power_off = ip27_machine_power_off;
}
