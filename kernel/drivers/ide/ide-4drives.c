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


#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/ide.h>

#define DRV_NAME "ide-4drives"

static int probe_4drives;

module_param_named(probe, probe_4drives, bool, 0);
MODULE_PARM_DESC(probe, "probe for generic IDE chipset with 4 drives/port");

static void ide_4drives_init_dev(ide_drive_t *drive)
{
	if (drive->hwif->channel)
		drive->select ^= 0x20;
}

static const struct ide_port_ops ide_4drives_port_ops = {
	.init_dev		= ide_4drives_init_dev,
};

static const struct ide_port_info ide_4drives_port_info = {
	.port_ops		= &ide_4drives_port_ops,
	.host_flags		= IDE_HFLAG_SERIALIZE | IDE_HFLAG_NO_DMA,
};

static int __init ide_4drives_init(void)
{
	unsigned long base = 0x1f0, ctl = 0x3f6;
	hw_regs_t hw, *hws[] = { &hw, &hw, NULL, NULL };

	if (probe_4drives == 0)
		return -ENODEV;

	if (!request_region(base, 8, DRV_NAME)) {
		printk(KERN_ERR "%s: I/O resource 0x%lX-0x%lX not free.\n",
				DRV_NAME, base, base + 7);
		return -EBUSY;
	}

	if (!request_region(ctl, 1, DRV_NAME)) {
		printk(KERN_ERR "%s: I/O resource 0x%lX not free.\n",
				DRV_NAME, ctl);
		release_region(base, 8);
		return -EBUSY;
	}

	memset(&hw, 0, sizeof(hw));

	ide_std_init_ports(&hw, base, ctl);
	hw.irq = 14;
	hw.chipset = ide_4drives;

	return ide_host_add(&ide_4drives_port_info, hws, NULL);
}

module_init(ide_4drives_init);

MODULE_AUTHOR("Bartlomiej Zolnierkiewicz");
MODULE_DESCRIPTION("generic IDE chipset with 4 drives/port support");
MODULE_LICENSE("GPL");
