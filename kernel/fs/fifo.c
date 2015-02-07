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
 *  linux/fs/fifo.c
 *
 *  written by Paul H. Hargrove
 *
 *  Fixes:
 *	10-06-1999, AV: fixed OOM handling in fifo_open(), moved
 *			initialization there, switched to external
 *			allocation of pipe_inode_info.
 */

#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/pipe_fs_i.h>

static void wait_for_partner(struct inode* inode, unsigned int *cnt)
{
	int cur = *cnt;	

	while (cur == *cnt) {
		pipe_wait(inode->i_pipe);
		if (signal_pending(current))
			break;
	}
}

static void wake_up_partner(struct inode* inode)
{
	wake_up_interruptible(&inode->i_pipe->wait);
}

static int fifo_open(struct inode *inode, struct file *filp)
{
	struct pipe_inode_info *pipe;
	int ret;

	mutex_lock(&inode->i_mutex);
	pipe = inode->i_pipe;
	if (!pipe) {
		ret = -ENOMEM;
		pipe = alloc_pipe_info(inode);
		if (!pipe)
			goto err_nocleanup;
		inode->i_pipe = pipe;
	}
	filp->f_version = 0;

	/* We can only do regular read/write on fifos */
	filp->f_mode &= (FMODE_READ | FMODE_WRITE);

	switch (filp->f_mode) {
	case FMODE_READ:
	/*
	 *  O_RDONLY
	 *  POSIX.1 says that O_NONBLOCK means return with the FIFO
	 *  opened, even when there is no process writing the FIFO.
	 */
		filp->f_op = &read_pipefifo_fops;
		pipe->r_counter++;
		if (pipe->readers++ == 0)
			wake_up_partner(inode);

		if (!pipe->writers) {
			if ((filp->f_flags & O_NONBLOCK)) {
				/* suppress POLLHUP until we have
				 * seen a writer */
				filp->f_version = pipe->w_counter;
			} else 
			{
				wait_for_partner(inode, &pipe->w_counter);
				if(signal_pending(current))
					goto err_rd;
			}
		}
		break;
	
	case FMODE_WRITE:
	/*
	 *  O_WRONLY
	 *  POSIX.1 says that O_NONBLOCK means return -1 with
	 *  errno=ENXIO when there is no process reading the FIFO.
	 */
		ret = -ENXIO;
		if ((filp->f_flags & O_NONBLOCK) && !pipe->readers)
			goto err;

		filp->f_op = &write_pipefifo_fops;
		pipe->w_counter++;
		if (!pipe->writers++)
			wake_up_partner(inode);

		if (!pipe->readers) {
			wait_for_partner(inode, &pipe->r_counter);
			if (signal_pending(current))
				goto err_wr;
		}
		break;
	
	case FMODE_READ | FMODE_WRITE:
	/*
	 *  O_RDWR
	 *  POSIX.1 leaves this case "undefined" when O_NONBLOCK is set.
	 *  This implementation will NEVER block on a O_RDWR open, since
	 *  the process can at least talk to itself.
	 */
		filp->f_op = &rdwr_pipefifo_fops;

		pipe->readers++;
		pipe->writers++;
		pipe->r_counter++;
		pipe->w_counter++;
		if (pipe->readers == 1 || pipe->writers == 1)
			wake_up_partner(inode);
		break;

	default:
		ret = -EINVAL;
		goto err;
	}

	/* Ok! */
	mutex_unlock(&inode->i_mutex);
	return 0;

err_rd:
	if (!--pipe->readers)
		wake_up_interruptible(&pipe->wait);
	ret = -ERESTARTSYS;
	goto err;

err_wr:
	if (!--pipe->writers)
		wake_up_interruptible(&pipe->wait);
	ret = -ERESTARTSYS;
	goto err;

err:
	if (!pipe->readers && !pipe->writers)
		free_pipe_info(inode);

err_nocleanup:
	mutex_unlock(&inode->i_mutex);
	return ret;
}

/*
 * Dummy default file-operations: the only thing this does
 * is contain the open that then fills in the correct operations
 * depending on the access mode of the file...
 */
const struct file_operations def_fifo_fops = {
	.open		= fifo_open,	/* will set read_ or write_pipefifo_fops */
};
