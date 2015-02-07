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
 * This contains the io-permission bitmap code - written by obz, with changes
 * by Linus. 32/64 bits code unification by Miguel Botón.
 */

#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/capability.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/ioport.h>
#include <linux/smp.h>
#include <linux/stddef.h>
#include <linux/slab.h>
#include <linux/thread_info.h>
#include <linux/syscalls.h>
#include <asm/syscalls.h>

/* Set EXTENT bits starting at BASE in BITMAP to value TURN_ON. */
static void set_bitmap(unsigned long *bitmap, unsigned int base,
		       unsigned int extent, int new_value)
{
	unsigned int i;

	for (i = base; i < base + extent; i++) {
		if (new_value)
			__set_bit(i, bitmap);
		else
			__clear_bit(i, bitmap);
	}
}

/*
 * this changes the io permissions bitmap in the current task.
 */
asmlinkage long sys_ioperm(unsigned long from, unsigned long num, int turn_on)
{
	struct thread_struct *t = &current->thread;
	struct tss_struct *tss;
	unsigned int i, max_long, bytes, bytes_updated;

	if ((from + num <= from) || (from + num > IO_BITMAP_BITS))
		return -EINVAL;
	if (turn_on && !capable(CAP_SYS_RAWIO))
		return -EPERM;

	/*
	 * If it's the first ioperm() call in this thread's lifetime, set the
	 * IO bitmap up. ioperm() is much less timing critical than clone(),
	 * this is why we delay this operation until now:
	 */
	if (!t->io_bitmap_ptr) {
		unsigned long *bitmap = kmalloc(IO_BITMAP_BYTES, GFP_KERNEL);

		if (!bitmap)
			return -ENOMEM;

		memset(bitmap, 0xff, IO_BITMAP_BYTES);
		t->io_bitmap_ptr = bitmap;
		set_thread_flag(TIF_IO_BITMAP);
	}

	/*
	 * do it in the per-thread copy and in the TSS ...
	 *
	 * Disable preemption via get_cpu() - we must not switch away
	 * because the ->io_bitmap_max value must match the bitmap
	 * contents:
	 */
	tss = &per_cpu(init_tss, get_cpu());

	set_bitmap(t->io_bitmap_ptr, from, num, !turn_on);

	/*
	 * Search for a (possibly new) maximum. This is simple and stupid,
	 * to keep it obviously correct:
	 */
	max_long = 0;
	for (i = 0; i < IO_BITMAP_LONGS; i++)
		if (t->io_bitmap_ptr[i] != ~0UL)
			max_long = i;

	bytes = (max_long + 1) * sizeof(unsigned long);
	bytes_updated = max(bytes, t->io_bitmap_max);

	t->io_bitmap_max = bytes;

#ifdef CONFIG_X86_32
	/*
	 * Sets the lazy trigger so that the next I/O operation will
	 * reload the correct bitmap.
	 * Reset the owner so that a process switch will not set
	 * tss->io_bitmap_base to IO_BITMAP_OFFSET.
	 */
	tss->x86_tss.io_bitmap_base = INVALID_IO_BITMAP_OFFSET_LAZY;
	tss->io_bitmap_owner = NULL;
#else
	/* Update the TSS: */
	memcpy(tss->io_bitmap, t->io_bitmap_ptr, bytes_updated);
#endif

	put_cpu();

	return 0;
}

/*
 * sys_iopl has to be used when you want to access the IO ports
 * beyond the 0x3ff range: to get the full 65536 ports bitmapped
 * you'd need 8kB of bitmaps/process, which is a bit excessive.
 *
 * Here we just change the flags value on the stack: we allow
 * only the super-user to do it. This depends on the stack-layout
 * on system-call entry - see also fork() and the signal handling
 * code.
 */
static int do_iopl(unsigned int level, struct pt_regs *regs)
{
	unsigned int old = (regs->flags >> 12) & 3;

	if (level > 3)
		return -EINVAL;
	/* Trying to gain more privileges? */
	if (level > old) {
		if (!capable(CAP_SYS_RAWIO))
			return -EPERM;
	}
	regs->flags = (regs->flags & ~X86_EFLAGS_IOPL) | (level << 12);

	return 0;
}

#ifdef CONFIG_X86_32
asmlinkage long sys_iopl(unsigned long regsp)
{
	struct pt_regs *regs = (struct pt_regs *)&regsp;
	unsigned int level = regs->bx;
	struct thread_struct *t = &current->thread;
	int rc;

	rc = do_iopl(level, regs);
	if (rc < 0)
		goto out;

	t->iopl = level << 12;
	set_iopl_mask(t->iopl);
out:
	return rc;
}
#else
asmlinkage long sys_iopl(unsigned int level, struct pt_regs *regs)
{
	return do_iopl(level, regs);
}
#endif
