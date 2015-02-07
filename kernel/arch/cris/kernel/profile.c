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
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/types.h>
#include <asm/ptrace.h>
#include <asm/uaccess.h>

#define SAMPLE_BUFFER_SIZE 8192

static char* sample_buffer;
static char* sample_buffer_pos;
static int prof_running = 0;

void
cris_profile_sample(struct pt_regs* regs)
{
	if (!prof_running)
		return;

	if (user_mode(regs))
		*(unsigned int*)sample_buffer_pos = current->pid;
	else
		*(unsigned int*)sample_buffer_pos = 0;

	*(unsigned int*)(sample_buffer_pos + 4) = instruction_pointer(regs);
	sample_buffer_pos += 8;

	if (sample_buffer_pos == sample_buffer + SAMPLE_BUFFER_SIZE)
		sample_buffer_pos = sample_buffer;
}

static ssize_t
read_cris_profile(struct file *file, char __user *buf,
		  size_t count, loff_t *ppos)
{
	unsigned long p = *ppos;
	ssize_t ret;

	ret = simple_read_from_buffer(buf, count, ppos, sample_buffer,
						SAMPLE_BUFFER_SIZE);
	if (ret < 0)
		return ret;

	memset(sample_buffer + p, 0, ret);

	return ret;
}

static ssize_t
write_cris_profile(struct file *file, const char __user *buf,
		   size_t count, loff_t *ppos)
{
	sample_buffer_pos = sample_buffer;
	memset(sample_buffer, 0, SAMPLE_BUFFER_SIZE);
}

static const struct file_operations cris_proc_profile_operations = {
	.read		= read_cris_profile,
	.write		= write_cris_profile,
};

static int
__init init_cris_profile(void)
{
	struct proc_dir_entry *entry;

	sample_buffer = kmalloc(SAMPLE_BUFFER_SIZE, GFP_KERNEL);
	if (!sample_buffer) {
		return -ENOMEM;
	}

	sample_buffer_pos = sample_buffer;

	entry = proc_create("system_profile", S_IWUSR | S_IRUGO, NULL,
			    &cris_proc_profile_operations);
	if (entry) {
		entry->size = SAMPLE_BUFFER_SIZE;
	}
	prof_running = 1;

	return 0;
}

__initcall(init_cris_profile);
