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
#include <linux/init.h>
#include <linux/console.h>

#include "chan_user.h"

/* ----------------------------------------------------------------------------- */
/* trivial console driver -- simply dump everything to stderr                    */

/*
 * Don't register by default -- as this registers very early in the
 * boot process it becomes the default console.
 *
 * Initialized at init time.
 */
static int use_stderr_console = 0;

static void stderr_console_write(struct console *console, const char *string,
				 unsigned len)
{
	generic_write(2 /* stderr */, string, len, NULL);
}

static struct console stderr_console = {
	.name		= "stderr",
	.write		= stderr_console_write,
	.flags		= CON_PRINTBUFFER,
};

static int __init stderr_console_init(void)
{
	if (use_stderr_console)
		register_console(&stderr_console);
	return 0;
}
console_initcall(stderr_console_init);

static int stderr_setup(char *str)
{
	if (!str)
		return 0;
	use_stderr_console = simple_strtoul(str,&str,0);
	return 1;
}
__setup("stderr=", stderr_setup);

/* The previous behavior of not unregistering led to /dev/console being
 * impossible to open.  My FC5 filesystem started having init die, and the
 * system panicing because of this.  Unregistering causes the real
 * console to become the default console, and /dev/console can then be
 * opened.  Making this an initcall makes this happen late enough that
 * there is no added value in dumping everything to stderr, and the
 * normal console is good enough to show you all available output.
 */
static int __init unregister_stderr(void)
{
	unregister_console(&stderr_console);

	return 0;
}

__initcall(unregister_stderr);
