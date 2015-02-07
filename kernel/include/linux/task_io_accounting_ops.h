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
 * Task I/O accounting operations
 */
#ifndef __TASK_IO_ACCOUNTING_OPS_INCLUDED
#define __TASK_IO_ACCOUNTING_OPS_INCLUDED

#include <linux/sched.h>

#ifdef CONFIG_TASK_IO_ACCOUNTING
static inline void task_io_account_read(size_t bytes)
{
	current->ioac.read_bytes += bytes;
}

/*
 * We approximate number of blocks, because we account bytes only.
 * A 'block' is 512 bytes
 */
static inline unsigned long task_io_get_inblock(const struct task_struct *p)
{
	return p->ioac.read_bytes >> 9;
}

static inline void task_io_account_write(size_t bytes)
{
	current->ioac.write_bytes += bytes;
}

/*
 * We approximate number of blocks, because we account bytes only.
 * A 'block' is 512 bytes
 */
static inline unsigned long task_io_get_oublock(const struct task_struct *p)
{
	return p->ioac.write_bytes >> 9;
}

static inline void task_io_account_cancelled_write(size_t bytes)
{
	current->ioac.cancelled_write_bytes += bytes;
}

static inline void task_io_accounting_init(struct task_io_accounting *ioac)
{
	memset(ioac, 0, sizeof(*ioac));
}

static inline void task_blk_io_accounting_add(struct task_io_accounting *dst,
						struct task_io_accounting *src)
{
	dst->read_bytes += src->read_bytes;
	dst->write_bytes += src->write_bytes;
	dst->cancelled_write_bytes += src->cancelled_write_bytes;
}

#else

static inline void task_io_account_read(size_t bytes)
{
}

static inline unsigned long task_io_get_inblock(const struct task_struct *p)
{
	return 0;
}

static inline void task_io_account_write(size_t bytes)
{
}

static inline unsigned long task_io_get_oublock(const struct task_struct *p)
{
	return 0;
}

static inline void task_io_account_cancelled_write(size_t bytes)
{
}

static inline void task_io_accounting_init(struct task_io_accounting *ioac)
{
}

static inline void task_blk_io_accounting_add(struct task_io_accounting *dst,
						struct task_io_accounting *src)
{
}

#endif /* CONFIG_TASK_IO_ACCOUNTING */

#ifdef CONFIG_TASK_XACCT
static inline void task_chr_io_accounting_add(struct task_io_accounting *dst,
						struct task_io_accounting *src)
{
	dst->rchar += src->rchar;
	dst->wchar += src->wchar;
	dst->syscr += src->syscr;
	dst->syscw += src->syscw;
}
#else
static inline void task_chr_io_accounting_add(struct task_io_accounting *dst,
						struct task_io_accounting *src)
{
}
#endif /* CONFIG_TASK_XACCT */

static inline void task_io_accounting_add(struct task_io_accounting *dst,
						struct task_io_accounting *src)
{
	task_chr_io_accounting_add(dst, src);
	task_blk_io_accounting_add(dst, src);
}
#endif /* __TASK_IO_ACCOUNTING_OPS_INCLUDED */
