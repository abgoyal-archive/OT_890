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
 * linux/kernel/dma.c: A DMA channel allocator. Inspired by linux/kernel/irq.c.
 *
 * Written by Hennus Bergman, 1992.
 *
 * 1994/12/26: Changes by Alex Nash to fix a minor bug in /proc/dma.
 *   In the previous version the reported device could end up being wrong,
 *   if a device requested a DMA channel that was already in use.
 *   [It also happened to remove the sizeof(char *) == sizeof(int)
 *   assumption introduced because of those /proc/dma patches. -- Hennus]
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <asm/dma.h>
#include <asm/system.h>



/* A note on resource allocation:
 *
 * All drivers needing DMA channels, should allocate and release them
 * through the public routines `request_dma()' and `free_dma()'.
 *
 * In order to avoid problems, all processes should allocate resources in
 * the same sequence and release them in the reverse order.
 *
 * So, when allocating DMAs and IRQs, first allocate the IRQ, then the DMA.
 * When releasing them, first release the DMA, then release the IRQ.
 * If you don't, you may cause allocation requests to fail unnecessarily.
 * This doesn't really matter now, but it will once we get real semaphores
 * in the kernel.
 */


DEFINE_SPINLOCK(dma_spin_lock);

/*
 *	If our port doesn't define this it has no PC like DMA
 */

#ifdef MAX_DMA_CHANNELS


/* Channel n is busy iff dma_chan_busy[n].lock != 0.
 * DMA0 used to be reserved for DRAM refresh, but apparently not any more...
 * DMA4 is reserved for cascading.
 */

struct dma_chan {
	int  lock;
	const char *device_id;
};

static struct dma_chan dma_chan_busy[MAX_DMA_CHANNELS] = {
	[4] = { 1, "cascade" },
};


/**
 * request_dma - request and reserve a system DMA channel
 * @dmanr: DMA channel number
 * @device_id: reserving device ID string, used in /proc/dma
 */
int request_dma(unsigned int dmanr, const char * device_id)
{
	if (dmanr >= MAX_DMA_CHANNELS)
		return -EINVAL;

	if (xchg(&dma_chan_busy[dmanr].lock, 1) != 0)
		return -EBUSY;

	dma_chan_busy[dmanr].device_id = device_id;

	/* old flag was 0, now contains 1 to indicate busy */
	return 0;
} /* request_dma */

/**
 * free_dma - free a reserved system DMA channel
 * @dmanr: DMA channel number
 */
void free_dma(unsigned int dmanr)
{
	if (dmanr >= MAX_DMA_CHANNELS) {
		printk(KERN_WARNING "Trying to free DMA%d\n", dmanr);
		return;
	}

	if (xchg(&dma_chan_busy[dmanr].lock, 0) == 0) {
		printk(KERN_WARNING "Trying to free free DMA%d\n", dmanr);
		return;
	}

} /* free_dma */

#else

int request_dma(unsigned int dmanr, const char *device_id)
{
	return -EINVAL;
}

void free_dma(unsigned int dmanr)
{
}

#endif

#ifdef CONFIG_PROC_FS

#ifdef MAX_DMA_CHANNELS
static int proc_dma_show(struct seq_file *m, void *v)
{
	int i;

	for (i = 0 ; i < MAX_DMA_CHANNELS ; i++) {
		if (dma_chan_busy[i].lock) {
			seq_printf(m, "%2d: %s\n", i,
				   dma_chan_busy[i].device_id);
		}
	}
	return 0;
}
#else
static int proc_dma_show(struct seq_file *m, void *v)
{
	seq_puts(m, "No DMA\n");
	return 0;
}
#endif /* MAX_DMA_CHANNELS */

static int proc_dma_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_dma_show, NULL);
}

static const struct file_operations proc_dma_operations = {
	.open		= proc_dma_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int __init proc_dma_init(void)
{
	proc_create("dma", 0, NULL, &proc_dma_operations);
	return 0;
}

__initcall(proc_dma_init);
#endif

EXPORT_SYMBOL(request_dma);
EXPORT_SYMBOL(free_dma);
EXPORT_SYMBOL(dma_spin_lock);
