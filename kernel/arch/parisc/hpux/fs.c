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
 *    Implements HPUX syscalls.
 *
 *    Copyright (C) 1999 Matthew Wilcox <willy with parisc-linux.org>
 *    Copyright (C) 2000 Michael Ang <mang with subcarrier.org>
 *    Copyright (C) 2000 John Marvin <jsm with parisc-linux.org>
 *    Copyright (C) 2000 Philipp Rumpf
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/file.h>
#include <linux/slab.h>
#include <linux/ptrace.h>
#include <asm/errno.h>
#include <asm/uaccess.h>

int hpux_execve(struct pt_regs *regs)
{
	int error;
	char *filename;

	filename = getname((char __user *) regs->gr[26]);
	error = PTR_ERR(filename);
	if (IS_ERR(filename))
		goto out;

	error = do_execve(filename, (char __user * __user *) regs->gr[25],
		(char __user * __user *) regs->gr[24], regs);

	if (error == 0) {
		task_lock(current);
		current->ptrace &= ~PT_DTRACE;
		task_unlock(current);
	}
	putname(filename);

out:
	return error;
}

struct hpux_dirent {
	loff_t	d_off;
	ino_t	d_ino;
	short	d_reclen;
	short	d_namlen;
	char	d_name[1];
};

struct getdents_callback {
	struct hpux_dirent __user *current_dir;
	struct hpux_dirent __user *previous;
	int count;
	int error;
};

#define NAME_OFFSET(de) ((int) ((de)->d_name - (char __user *) (de)))

static int filldir(void * __buf, const char * name, int namlen, loff_t offset,
		u64 ino, unsigned d_type)
{
	struct hpux_dirent __user * dirent;
	struct getdents_callback * buf = (struct getdents_callback *) __buf;
	ino_t d_ino;
	int reclen = ALIGN(NAME_OFFSET(dirent) + namlen + 1, sizeof(long));

	buf->error = -EINVAL;	/* only used if we fail.. */
	if (reclen > buf->count)
		return -EINVAL;
	d_ino = ino;
	if (sizeof(d_ino) < sizeof(ino) && d_ino != ino) {
		buf->error = -EOVERFLOW;
		return -EOVERFLOW;
	}
	dirent = buf->previous;
	if (dirent)
		if (put_user(offset, &dirent->d_off))
			goto Efault;
	dirent = buf->current_dir;
	if (put_user(d_ino, &dirent->d_ino) ||
	    put_user(reclen, &dirent->d_reclen) ||
	    put_user(namlen, &dirent->d_namlen) ||
	    copy_to_user(dirent->d_name, name, namlen) ||
	    put_user(0, dirent->d_name + namlen))
		goto Efault;
	buf->previous = dirent;
	buf->current_dir = (void __user *)dirent + reclen;
	buf->count -= reclen;
	return 0;
Efault:
	buf->error = -EFAULT;
	return -EFAULT;
}

#undef NAME_OFFSET

int hpux_getdents(unsigned int fd, struct hpux_dirent __user *dirent, unsigned int count)
{
	struct file * file;
	struct hpux_dirent __user * lastdirent;
	struct getdents_callback buf;
	int error = -EBADF;

	file = fget(fd);
	if (!file)
		goto out;

	buf.current_dir = dirent;
	buf.previous = NULL;
	buf.count = count;
	buf.error = 0;

	error = vfs_readdir(file, filldir, &buf);
	if (error >= 0)
		error = buf.error;
	lastdirent = buf.previous;
	if (lastdirent) {
		if (put_user(file->f_pos, &lastdirent->d_off))
			error = -EFAULT;
		else
			error = count - buf.count;
	}

	fput(file);
out:
	return error;
}

int hpux_mount(const char *fs, const char *path, int mflag,
		const char *fstype, const char *dataptr, int datalen)
{
	return -ENOSYS;
}

static int cp_hpux_stat(struct kstat *stat, struct hpux_stat64 __user *statbuf)
{
	struct hpux_stat64 tmp;

	/* we probably want a different split here - is hpux 12:20? */

	if (!new_valid_dev(stat->dev) || !new_valid_dev(stat->rdev))
		return -EOVERFLOW;

	memset(&tmp, 0, sizeof(tmp));
	tmp.st_dev = new_encode_dev(stat->dev);
	tmp.st_ino = stat->ino;
	tmp.st_mode = stat->mode;
	tmp.st_nlink = stat->nlink;
	tmp.st_uid = stat->uid;
	tmp.st_gid = stat->gid;
	tmp.st_rdev = new_encode_dev(stat->rdev);
	tmp.st_size = stat->size;
	tmp.st_atime = stat->atime.tv_sec;
	tmp.st_mtime = stat->mtime.tv_sec;
	tmp.st_ctime = stat->ctime.tv_sec;
	tmp.st_blocks = stat->blocks;
	tmp.st_blksize = stat->blksize;
	return copy_to_user(statbuf,&tmp,sizeof(tmp)) ? -EFAULT : 0;
}

long hpux_stat64(char __user *filename, struct hpux_stat64 __user *statbuf)
{
	struct kstat stat;
	int error = vfs_stat(filename, &stat);

	if (!error)
		error = cp_hpux_stat(&stat, statbuf);

	return error;
}

long hpux_fstat64(unsigned int fd, struct hpux_stat64 __user *statbuf)
{
	struct kstat stat;
	int error = vfs_fstat(fd, &stat);

	if (!error)
		error = cp_hpux_stat(&stat, statbuf);

	return error;
}

long hpux_lstat64(char __user *filename, struct hpux_stat64 __user *statbuf)
{
	struct kstat stat;
	int error = vfs_lstat(filename, &stat);

	if (!error)
		error = cp_hpux_stat(&stat, statbuf);

	return error;
}