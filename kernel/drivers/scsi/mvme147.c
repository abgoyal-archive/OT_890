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

#include <linux/types.h>
#include <linux/mm.h>
#include <linux/blkdev.h>
#include <linux/interrupt.h>

#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/mvme147hw.h>
#include <asm/irq.h>

#include "scsi.h"
#include <scsi/scsi_host.h>
#include "wd33c93.h"
#include "mvme147.h"

#include<linux/stat.h>

#define HDATA(ptr) ((struct WD33C93_hostdata *)((ptr)->hostdata))

static struct Scsi_Host *mvme147_host = NULL;

static irqreturn_t mvme147_intr (int irq, void *dummy)
{
    if (irq == MVME147_IRQ_SCSI_PORT)
	wd33c93_intr (mvme147_host);
    else
	m147_pcc->dma_intr = 0x89;	/* Ack and enable ints */
    return IRQ_HANDLED;
}

static int dma_setup(struct scsi_cmnd *cmd, int dir_in)
{
    unsigned char flags = 0x01;
    unsigned long addr = virt_to_bus(cmd->SCp.ptr);

    /* setup dma direction */
    if (!dir_in)
	flags |= 0x04;

    /* remember direction */
    HDATA(mvme147_host)->dma_dir = dir_in;

    if (dir_in)
  	/* invalidate any cache */
	cache_clear (addr, cmd->SCp.this_residual);
    else
	/* push any dirty cache */
	cache_push (addr, cmd->SCp.this_residual);

    /* start DMA */
    m147_pcc->dma_bcr   = cmd->SCp.this_residual | (1<<24);
    m147_pcc->dma_dadr  = addr;
    m147_pcc->dma_cntrl = flags;

    /* return success */
    return 0;
}

static void dma_stop(struct Scsi_Host *instance, struct scsi_cmnd *SCpnt,
		      int status)
{
    m147_pcc->dma_cntrl = 0;
}

int mvme147_detect(struct scsi_host_template *tpnt)
{
    static unsigned char called = 0;
    wd33c93_regs regs;

    if (!MACH_IS_MVME147 || called)
	return 0;
    called++;

    tpnt->proc_name = "MVME147";
    tpnt->proc_info = &wd33c93_proc_info;

    mvme147_host = scsi_register (tpnt, sizeof(struct WD33C93_hostdata));
    if (!mvme147_host)
	    goto err_out;

    mvme147_host->base = 0xfffe4000;
    mvme147_host->irq = MVME147_IRQ_SCSI_PORT;
    regs.SASR = (volatile unsigned char *)0xfffe4000;
    regs.SCMD = (volatile unsigned char *)0xfffe4001;
    HDATA(mvme147_host)->no_sync = 0xff;
    HDATA(mvme147_host)->fast = 0;
    HDATA(mvme147_host)->dma_mode = CTRL_DMA;
    wd33c93_init(mvme147_host, regs, dma_setup, dma_stop, WD33C93_FS_8_10);

    if (request_irq(MVME147_IRQ_SCSI_PORT, mvme147_intr, 0, "MVME147 SCSI PORT", mvme147_intr))
	    goto err_unregister;
    if (request_irq(MVME147_IRQ_SCSI_DMA, mvme147_intr, 0, "MVME147 SCSI DMA", mvme147_intr))
	    goto err_free_irq;
#if 0	/* Disabled; causes problems booting */
    m147_pcc->scsi_interrupt = 0x10;	/* Assert SCSI bus reset */
    udelay(100);
    m147_pcc->scsi_interrupt = 0x00;	/* Negate SCSI bus reset */
    udelay(2000);
    m147_pcc->scsi_interrupt = 0x40;	/* Clear bus reset interrupt */
#endif
    m147_pcc->scsi_interrupt = 0x09;	/* Enable interrupt */

    m147_pcc->dma_cntrl = 0x00;		/* ensure DMA is stopped */
    m147_pcc->dma_intr = 0x89;		/* Ack and enable ints */

    return 1;

 err_free_irq:
    free_irq(MVME147_IRQ_SCSI_PORT, mvme147_intr);
 err_unregister:
    wd33c93_release();
    scsi_unregister(mvme147_host);
 err_out:
    return 0;
}

static int mvme147_bus_reset(struct scsi_cmnd *cmd)
{
	/* FIXME perform bus-specific reset */

	/* FIXME 2: kill this function, and let midlayer fallback to 
	   the same result, calling wd33c93_host_reset() */

	spin_lock_irq(cmd->device->host->host_lock);
	wd33c93_host_reset(cmd);
	spin_unlock_irq(cmd->device->host->host_lock);

	return SUCCESS;
}

#define HOSTS_C

#include "mvme147.h"

static struct scsi_host_template driver_template = {
	.proc_name		= "MVME147",
	.name			= "MVME147 built-in SCSI",
	.detect			= mvme147_detect,
	.release		= mvme147_release,
	.queuecommand		= wd33c93_queuecommand,
	.eh_abort_handler	= wd33c93_abort,
	.eh_bus_reset_handler	= mvme147_bus_reset,
	.eh_host_reset_handler	= wd33c93_host_reset,
	.can_queue		= CAN_QUEUE,
	.this_id		= 7,
	.sg_tablesize		= SG_ALL,
	.cmd_per_lun		= CMD_PER_LUN,
	.use_clustering		= ENABLE_CLUSTERING
};


#include "scsi_module.c"

int mvme147_release(struct Scsi_Host *instance)
{
#ifdef MODULE
    /* XXX Make sure DMA is stopped! */
    wd33c93_release();
    free_irq(MVME147_IRQ_SCSI_PORT, mvme147_intr);
    free_irq(MVME147_IRQ_SCSI_DMA, mvme147_intr);
#endif
    return 1;
}
