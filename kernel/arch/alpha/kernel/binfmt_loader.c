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

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/mm_types.h>
#include <linux/binfmts.h>
#include <linux/a.out.h>

static int load_binary(struct linux_binprm *bprm, struct pt_regs *regs)
{
	struct exec *eh = (struct exec *)bprm->buf;
	unsigned long loader;
	struct file *file;
	int retval;

	if (eh->fh.f_magic != 0x183 || (eh->fh.f_flags & 0x3000) != 0x3000)
		return -ENOEXEC;

	if (bprm->loader)
		return -ENOEXEC;

	allow_write_access(bprm->file);
	fput(bprm->file);
	bprm->file = NULL;

	loader = bprm->vma->vm_end - sizeof(void *);

	file = open_exec("/sbin/loader");
	retval = PTR_ERR(file);
	if (IS_ERR(file))
		return retval;

	/* Remember if the application is TASO.  */
	bprm->taso = eh->ah.entry < 0x100000000UL;

	bprm->file = file;
	bprm->loader = loader;
	retval = prepare_binprm(bprm);
	if (retval < 0)
		return retval;
	return search_binary_handler(bprm,regs);
}

static struct linux_binfmt loader_format = {
	.load_binary	= load_binary,
};

static int __init init_loader_binfmt(void)
{
	return register_binfmt(&loader_format);
}
arch_initcall(init_loader_binfmt);
