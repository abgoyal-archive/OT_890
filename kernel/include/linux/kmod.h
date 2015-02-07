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

#ifndef __LINUX_KMOD_H__
#define __LINUX_KMOD_H__

/*
 *	include/linux/kmod.h
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/gfp.h>
#include <linux/stddef.h>
#include <linux/errno.h>
#include <linux/compiler.h>

#define KMOD_PATH_LEN 256

#ifdef CONFIG_MODULES
/* modprobe exit status on success, -ve on error.  Return value
 * usually useless though. */
extern int request_module(const char * name, ...) __attribute__ ((format (printf, 1, 2)));
#define try_then_request_module(x, mod...) ((x) ?: (request_module(mod), (x)))
#else
static inline int request_module(const char * name, ...) { return -ENOSYS; }
#define try_then_request_module(x, mod...) (x)
#endif


struct key;
struct file;
struct subprocess_info;

/* Allocate a subprocess_info structure */
struct subprocess_info *call_usermodehelper_setup(char *path, char **argv,
						  char **envp, gfp_t gfp_mask);

/* Set various pieces of state into the subprocess_info structure */
void call_usermodehelper_setkeys(struct subprocess_info *info,
				 struct key *session_keyring);
int call_usermodehelper_stdinpipe(struct subprocess_info *sub_info,
				  struct file **filp);
void call_usermodehelper_setcleanup(struct subprocess_info *info,
				    void (*cleanup)(char **argv, char **envp));

enum umh_wait {
	UMH_NO_WAIT = -1,	/* don't wait at all */
	UMH_WAIT_EXEC = 0,	/* wait for the exec, but not the process */
	UMH_WAIT_PROC = 1,	/* wait for the process to complete */
};

/* Actually execute the sub-process */
int call_usermodehelper_exec(struct subprocess_info *info, enum umh_wait wait);

/* Free the subprocess_info. This is only needed if you're not going
   to call call_usermodehelper_exec */
void call_usermodehelper_freeinfo(struct subprocess_info *info);

static inline int
call_usermodehelper(char *path, char **argv, char **envp, enum umh_wait wait)
{
	struct subprocess_info *info;
	gfp_t gfp_mask = (wait == UMH_NO_WAIT) ? GFP_ATOMIC : GFP_KERNEL;

	info = call_usermodehelper_setup(path, argv, envp, gfp_mask);
	if (info == NULL)
		return -ENOMEM;
	return call_usermodehelper_exec(info, wait);
}

static inline int
call_usermodehelper_keys(char *path, char **argv, char **envp,
			 struct key *session_keyring, enum umh_wait wait)
{
	struct subprocess_info *info;
	gfp_t gfp_mask = (wait == UMH_NO_WAIT) ? GFP_ATOMIC : GFP_KERNEL;

	info = call_usermodehelper_setup(path, argv, envp, gfp_mask);
	if (info == NULL)
		return -ENOMEM;

	call_usermodehelper_setkeys(info, session_keyring);
	return call_usermodehelper_exec(info, wait);
}

extern void usermodehelper_init(void);

struct file;
extern int call_usermodehelper_pipe(char *path, char *argv[], char *envp[],
				    struct file **filp);

extern int usermodehelper_disable(void);
extern void usermodehelper_enable(void);

#endif /* __LINUX_KMOD_H__ */
