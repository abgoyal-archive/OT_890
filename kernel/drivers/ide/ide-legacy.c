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
#include <linux/ide.h>

static void ide_legacy_init_one(hw_regs_t **hws, hw_regs_t *hw,
				u8 port_no, const struct ide_port_info *d,
				unsigned long config)
{
	unsigned long base, ctl;
	int irq;

	if (port_no == 0) {
		base = 0x1f0;
		ctl  = 0x3f6;
		irq  = 14;
	} else {
		base = 0x170;
		ctl  = 0x376;
		irq  = 15;
	}

	if (!request_region(base, 8, d->name)) {
		printk(KERN_ERR "%s: I/O resource 0x%lX-0x%lX not free.\n",
				d->name, base, base + 7);
		return;
	}

	if (!request_region(ctl, 1, d->name)) {
		printk(KERN_ERR "%s: I/O resource 0x%lX not free.\n",
				d->name, ctl);
		release_region(base, 8);
		return;
	}

	ide_std_init_ports(hw, base, ctl);
	hw->irq = irq;
	hw->chipset = d->chipset;
	hw->config = config;

	hws[port_no] = hw;
}

int ide_legacy_device_add(const struct ide_port_info *d, unsigned long config)
{
	hw_regs_t hw[2], *hws[] = { NULL, NULL, NULL, NULL };

	memset(&hw, 0, sizeof(hw));

	if ((d->host_flags & IDE_HFLAG_QD_2ND_PORT) == 0)
		ide_legacy_init_one(hws, &hw[0], 0, d, config);
	ide_legacy_init_one(hws, &hw[1], 1, d, config);

	if (hws[0] == NULL && hws[1] == NULL &&
	    (d->host_flags & IDE_HFLAG_SINGLE))
		return -ENOENT;

	return ide_host_add(d, hws, NULL);
}
EXPORT_SYMBOL_GPL(ide_legacy_device_add);
