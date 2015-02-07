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
 * drivers/power/process.c - Functions for saving/restoring console.
 *
 * Originally from swsusp.
 */

#include <linux/vt_kern.h>
#include <linux/kbd_kern.h>
#include <linux/console.h>
#include <linux/module.h>
#include "power.h"

#if defined(CONFIG_VT) && defined(CONFIG_VT_CONSOLE)
#define SUSPEND_CONSOLE	(MAX_NR_CONSOLES-1)

static int orig_fgconsole, orig_kmsg;
static int disable_vt_switch;

/*
 * Normally during a suspend, we allocate a new console and switch to it.
 * When we resume, we switch back to the original console.  This switch
 * can be slow, so on systems where the framebuffer can handle restoration
 * of video registers anyways, there's little point in doing the console
 * switch.  This function allows you to disable it by passing it '0'.
 */
void pm_set_vt_switch(int do_switch)
{
	acquire_console_sem();
	disable_vt_switch = !do_switch;
	release_console_sem();
}
EXPORT_SYMBOL(pm_set_vt_switch);

int pm_prepare_console(void)
{
	acquire_console_sem();

	if (disable_vt_switch) {
		release_console_sem();
		return 0;
	}

	orig_fgconsole = fg_console;

	if (vc_allocate(SUSPEND_CONSOLE)) {
	  /* we can't have a free VC for now. Too bad,
	   * we don't want to mess the screen for now. */
		release_console_sem();
		return 1;
	}

	if (set_console(SUSPEND_CONSOLE)) {
		/*
		 * We're unable to switch to the SUSPEND_CONSOLE.
		 * Let the calling function know so it can decide
		 * what to do.
		 */
		release_console_sem();
		return 1;
	}
	release_console_sem();

	if (vt_waitactive(SUSPEND_CONSOLE)) {
		pr_debug("Suspend: Can't switch VCs.");
		return 1;
	}
	orig_kmsg = kmsg_redirect;
	kmsg_redirect = SUSPEND_CONSOLE;
	return 0;
}

void pm_restore_console(void)
{
	acquire_console_sem();
	if (disable_vt_switch) {
		release_console_sem();
		return;
	}
	set_console(orig_fgconsole);
	release_console_sem();

	if (vt_waitactive(orig_fgconsole)) {
		pr_debug("Resume: Can't switch VCs.");
		return;
	}

	kmsg_redirect = orig_kmsg;
}
#endif
