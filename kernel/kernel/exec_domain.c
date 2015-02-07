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
 * Handling of different ABIs (personalities).
 *
 * We group personalities into execution domains which have their
 * own handlers for kernel entry points, signal mapping, etc...
 *
 * 2001-05-06	Complete rewrite,  Christoph Hellwig (hch@infradead.org)
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kmod.h>
#include <linux/module.h>
#include <linux/personality.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/syscalls.h>
#include <linux/sysctl.h>
#include <linux/types.h>


static void default_handler(int, struct pt_regs *);

static struct exec_domain *exec_domains = &default_exec_domain;
static DEFINE_RWLOCK(exec_domains_lock);


static u_long ident_map[32] = {
	0,	1,	2,	3,	4,	5,	6,	7,
	8,	9,	10,	11,	12,	13,	14,	15,
	16,	17,	18,	19,	20,	21,	22,	23,
	24,	25,	26,	27,	28,	29,	30,	31
};

struct exec_domain default_exec_domain = {
	.name		= "Linux",		/* name */
	.handler	= default_handler,	/* lcall7 causes a seg fault. */
	.pers_low	= 0, 			/* PER_LINUX personality. */
	.pers_high	= 0,			/* PER_LINUX personality. */
	.signal_map	= ident_map,		/* Identity map signals. */
	.signal_invmap	= ident_map,		/*  - both ways. */
};


static void
default_handler(int segment, struct pt_regs *regp)
{
	set_personality(0);

	if (current_thread_info()->exec_domain->handler != default_handler)
		current_thread_info()->exec_domain->handler(segment, regp);
	else
		send_sig(SIGSEGV, current, 1);
}

static struct exec_domain *
lookup_exec_domain(u_long personality)
{
	struct exec_domain *	ep;
	u_long			pers = personality(personality);

	read_lock(&exec_domains_lock);
	for (ep = exec_domains; ep; ep = ep->next) {
		if (pers >= ep->pers_low && pers <= ep->pers_high)
			if (try_module_get(ep->module))
				goto out;
	}

#ifdef CONFIG_MODULES
	read_unlock(&exec_domains_lock);
	request_module("personality-%ld", pers);
	read_lock(&exec_domains_lock);

	for (ep = exec_domains; ep; ep = ep->next) {
		if (pers >= ep->pers_low && pers <= ep->pers_high)
			if (try_module_get(ep->module))
				goto out;
	}
#endif

	ep = &default_exec_domain;
out:
	read_unlock(&exec_domains_lock);
	return (ep);
}

int
register_exec_domain(struct exec_domain *ep)
{
	struct exec_domain	*tmp;
	int			err = -EBUSY;

	if (ep == NULL)
		return -EINVAL;

	if (ep->next != NULL)
		return -EBUSY;

	write_lock(&exec_domains_lock);
	for (tmp = exec_domains; tmp; tmp = tmp->next) {
		if (tmp == ep)
			goto out;
	}

	ep->next = exec_domains;
	exec_domains = ep;
	err = 0;

out:
	write_unlock(&exec_domains_lock);
	return (err);
}

int
unregister_exec_domain(struct exec_domain *ep)
{
	struct exec_domain	**epp;

	epp = &exec_domains;
	write_lock(&exec_domains_lock);
	for (epp = &exec_domains; *epp; epp = &(*epp)->next) {
		if (ep == *epp)
			goto unregister;
	}
	write_unlock(&exec_domains_lock);
	return -EINVAL;

unregister:
	*epp = ep->next;
	ep->next = NULL;
	write_unlock(&exec_domains_lock);
	return 0;
}

int
__set_personality(u_long personality)
{
	struct exec_domain	*ep, *oep;

	ep = lookup_exec_domain(personality);
	if (ep == current_thread_info()->exec_domain) {
		current->personality = personality;
		module_put(ep->module);
		return 0;
	}

	if (atomic_read(&current->fs->count) != 1) {
		struct fs_struct *fsp, *ofsp;

		fsp = copy_fs_struct(current->fs);
		if (fsp == NULL) {
			module_put(ep->module);
			return -ENOMEM;
		}

		task_lock(current);
		ofsp = current->fs;
		current->fs = fsp;
		task_unlock(current);

		put_fs_struct(ofsp);
	}

	/*
	 * At that point we are guaranteed to be the sole owner of
	 * current->fs.
	 */

	current->personality = personality;
	oep = current_thread_info()->exec_domain;
	current_thread_info()->exec_domain = ep;

	module_put(oep->module);
	return 0;
}

#ifdef CONFIG_PROC_FS
static int execdomains_proc_show(struct seq_file *m, void *v)
{
	struct exec_domain	*ep;

	read_lock(&exec_domains_lock);
	for (ep = exec_domains; ep; ep = ep->next)
		seq_printf(m, "%d-%d\t%-16s\t[%s]\n",
			       ep->pers_low, ep->pers_high, ep->name,
			       module_name(ep->module));
	read_unlock(&exec_domains_lock);
	return 0;
}

static int execdomains_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, execdomains_proc_show, NULL);
}

static const struct file_operations execdomains_proc_fops = {
	.open		= execdomains_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int __init proc_execdomains_init(void)
{
	proc_create("execdomains", 0, NULL, &execdomains_proc_fops);
	return 0;
}
module_init(proc_execdomains_init);
#endif

SYSCALL_DEFINE1(personality, u_long, personality)
{
	u_long old = current->personality;

	if (personality != 0xffffffff) {
		set_personality(personality);
		if (current->personality != personality)
			return -EINVAL;
	}

	return (long)old;
}


EXPORT_SYMBOL(register_exec_domain);
EXPORT_SYMBOL(unregister_exec_domain);
EXPORT_SYMBOL(__set_personality);
