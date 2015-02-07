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

#ifndef _LINUX_TTY_LDISC_H
#define _LINUX_TTY_LDISC_H

/*
 * This structure defines the interface between the tty line discipline
 * implementation and the tty routines.  The following routines can be
 * defined; unless noted otherwise, they are optional, and can be
 * filled in with a null pointer.
 *
 * int	(*open)(struct tty_struct *);
 *
 * 	This function is called when the line discipline is associated
 * 	with the tty.  The line discipline can use this as an
 * 	opportunity to initialize any state needed by the ldisc routines.
 * 
 * void	(*close)(struct tty_struct *);
 *
 *	This function is called when the line discipline is being
 * 	shutdown, either because the tty is being closed or because
 * 	the tty is being changed to use a new line discipline
 * 
 * void	(*flush_buffer)(struct tty_struct *tty);
 *
 * 	This function instructs the line discipline to clear its
 * 	buffers of any input characters it may have queued to be
 * 	delivered to the user mode process.
 * 
 * ssize_t (*chars_in_buffer)(struct tty_struct *tty);
 *
 * 	This function returns the number of input characters the line
 *	discipline may have queued up to be delivered to the user mode
 *	process.
 * 
 * ssize_t (*read)(struct tty_struct * tty, struct file * file,
 *		   unsigned char * buf, size_t nr);
 *
 * 	This function is called when the user requests to read from
 * 	the tty.  The line discipline will return whatever characters
 * 	it has buffered up for the user.  If this function is not
 * 	defined, the user will receive an EIO error.
 * 
 * ssize_t (*write)(struct tty_struct * tty, struct file * file,
 * 		    const unsigned char * buf, size_t nr);
 *
 * 	This function is called when the user requests to write to the
 * 	tty.  The line discipline will deliver the characters to the
 * 	low-level tty device for transmission, optionally performing
 * 	some processing on the characters first.  If this function is
 * 	not defined, the user will receive an EIO error.
 * 
 * int	(*ioctl)(struct tty_struct * tty, struct file * file,
 * 		 unsigned int cmd, unsigned long arg);
 *
 *	This function is called when the user requests an ioctl which
 * 	is not handled by the tty layer or the low-level tty driver.
 * 	It is intended for ioctls which affect line discpline
 * 	operation.  Note that the search order for ioctls is (1) tty
 * 	layer, (2) tty low-level driver, (3) line discpline.  So a
 * 	low-level driver can "grab" an ioctl request before the line
 * 	discpline has a chance to see it.
 * 
 * long	(*compat_ioctl)(struct tty_struct * tty, struct file * file,
 * 		        unsigned int cmd, unsigned long arg);
 *
 *      Process ioctl calls from 32-bit process on 64-bit system
 *
 * void	(*set_termios)(struct tty_struct *tty, struct ktermios * old);
 *
 * 	This function notifies the line discpline that a change has
 * 	been made to the termios structure.
 * 
 * int	(*poll)(struct tty_struct * tty, struct file * file,
 * 		  poll_table *wait);
 *
 * 	This function is called when a user attempts to select/poll on a
 * 	tty device.  It is solely the responsibility of the line
 * 	discipline to handle poll requests.
 *
 * void	(*receive_buf)(struct tty_struct *, const unsigned char *cp,
 * 		       char *fp, int count);
 *
 * 	This function is called by the low-level tty driver to send
 * 	characters received by the hardware to the line discpline for
 * 	processing.  <cp> is a pointer to the buffer of input
 * 	character received by the device.  <fp> is a pointer to a
 * 	pointer of flag bytes which indicate whether a character was
 * 	received with a parity error, etc.
 * 
 * void	(*write_wakeup)(struct tty_struct *);
 *
 * 	This function is called by the low-level tty driver to signal
 * 	that line discpline should try to send more characters to the
 * 	low-level driver for transmission.  If the line discpline does
 * 	not have any more data to send, it can just return.
 *
 * int (*hangup)(struct tty_struct *)
 *
 *	Called on a hangup. Tells the discipline that it should
 *	cease I/O to the tty driver. Can sleep. The driver should
 *	seek to perform this action quickly but should wait until
 *	any pending driver I/O is completed.
 */

#include <linux/fs.h>
#include <linux/wait.h>

struct tty_ldisc_ops {
	int	magic;
	char	*name;
	int	num;
	int	flags;
	
	/*
	 * The following routines are called from above.
	 */
	int	(*open)(struct tty_struct *);
	void	(*close)(struct tty_struct *);
	void	(*flush_buffer)(struct tty_struct *tty);
	ssize_t	(*chars_in_buffer)(struct tty_struct *tty);
	ssize_t	(*read)(struct tty_struct * tty, struct file * file,
			unsigned char __user * buf, size_t nr);
	ssize_t	(*write)(struct tty_struct * tty, struct file * file,
			 const unsigned char * buf, size_t nr);	
	int	(*ioctl)(struct tty_struct * tty, struct file * file,
			 unsigned int cmd, unsigned long arg);
	long	(*compat_ioctl)(struct tty_struct * tty, struct file * file,
				unsigned int cmd, unsigned long arg);
	void	(*set_termios)(struct tty_struct *tty, struct ktermios * old);
	unsigned int (*poll)(struct tty_struct *, struct file *,
			     struct poll_table_struct *);
	int	(*hangup)(struct tty_struct *tty);
	
	/*
	 * The following routines are called from below.
	 */
	void	(*receive_buf)(struct tty_struct *, const unsigned char *cp,
			       char *fp, int count);
	void	(*write_wakeup)(struct tty_struct *);

	struct  module *owner;
	
	int refcount;
};

struct tty_ldisc {
	struct tty_ldisc_ops *ops;
	int refcount;
};

#define TTY_LDISC_MAGIC	0x5403

#define LDISC_FLAG_DEFINED	0x00000001

#define MODULE_ALIAS_LDISC(ldisc) \
	MODULE_ALIAS("tty-ldisc-" __stringify(ldisc))

#endif /* _LINUX_TTY_LDISC_H */
