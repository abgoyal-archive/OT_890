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
 *  linux/fs/binfmt_em86.c
 *
 *  Based on linux/fs/binfmt_script.c
 *  Copyright (C) 1996  Martin von Löwis
 *  original #!-checking implemented by tytso.
 *
 *  em86 changes Copyright (C) 1997  Jim Paradis
 */

#include <linux/module.h>
#include <linux/string.h>
#include <linux/stat.h>
#include <linux/slab.h>
#include <linux/binfmts.h>
#include <linux/elf.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/errno.h>


#define EM86_INTERP	"/usr/bin/em86"
#define EM86_I_NAME	"em86"

static int load_em86(struct linux_binprm *bprm,struct pt_regs *regs)
{
	char *interp, *i_name, *i_arg;
	struct file * file;
	int retval;
	struct elfhdr	elf_ex;

	/* Make sure this is a Linux/Intel ELF executable... */
	elf_ex = *((struct elfhdr *)bprm->buf);

	if (memcmp(elf_ex.e_ident, ELFMAG, SELFMAG) != 0)
		return  -ENOEXEC;

	/* First of all, some simple consistency checks */
	if ((elf_ex.e_type != ET_EXEC && elf_ex.e_type != ET_DYN) ||
		(!((elf_ex.e_machine == EM_386) || (elf_ex.e_machine == EM_486))) ||
		(!bprm->file->f_op || !bprm->file->f_op->mmap)) {
			return -ENOEXEC;
	}

	bprm->recursion_depth++; /* Well, the bang-shell is implicit... */
	allow_write_access(bprm->file);
	fput(bprm->file);
	bprm->file = NULL;

	/* Unlike in the script case, we don't have to do any hairy
	 * parsing to find our interpreter... it's hardcoded!
	 */
	interp = EM86_INTERP;
	i_name = EM86_I_NAME;
	i_arg = NULL;		/* We reserve the right to add an arg later */

	/*
	 * Splice in (1) the interpreter's name for argv[0]
	 *           (2) (optional) argument to interpreter
	 *           (3) filename of emulated file (replace argv[0])
	 *
	 * This is done in reverse order, because of how the
	 * user environment and arguments are stored.
	 */
	remove_arg_zero(bprm);
	retval = copy_strings_kernel(1, &bprm->filename, bprm);
	if (retval < 0) return retval; 
	bprm->argc++;
	if (i_arg) {
		retval = copy_strings_kernel(1, &i_arg, bprm);
		if (retval < 0) return retval; 
		bprm->argc++;
	}
	retval = copy_strings_kernel(1, &i_name, bprm);
	if (retval < 0)	return retval;
	bprm->argc++;

	/*
	 * OK, now restart the process with the interpreter's inode.
	 * Note that we use open_exec() as the name is now in kernel
	 * space, and we don't need to copy it.
	 */
	file = open_exec(interp);
	if (IS_ERR(file))
		return PTR_ERR(file);

	bprm->file = file;

	retval = prepare_binprm(bprm);
	if (retval < 0)
		return retval;

	return search_binary_handler(bprm, regs);
}

static struct linux_binfmt em86_format = {
	.module		= THIS_MODULE,
	.load_binary	= load_em86,
};

static int __init init_em86_binfmt(void)
{
	return register_binfmt(&em86_format);
}

static void __exit exit_em86_binfmt(void)
{
	unregister_binfmt(&em86_format);
}

core_initcall(init_em86_binfmt);
module_exit(exit_em86_binfmt);
MODULE_LICENSE("GPL");
