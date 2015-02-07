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
 * Copyright 2006-2007, Michael Ellerman, IBM Corporation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2 of the
 * License.
 *
 */

#include <linux/irq.h>
#include <linux/bitmap.h>
#include <linux/msi.h>
#include <asm/mpic.h>
#include <asm/prom.h>
#include <asm/hw_irq.h>
#include <asm/ppc-pci.h>
#include <asm/msi_bitmap.h>

#include <sysdev/mpic.h>

void mpic_msi_reserve_hwirq(struct mpic *mpic, irq_hw_number_t hwirq)
{
	/* The mpic calls this even when there is no allocator setup */
	if (!mpic->msi_bitmap.bitmap)
		return;

	msi_bitmap_reserve_hwirq(&mpic->msi_bitmap, hwirq);
}

#ifdef CONFIG_MPIC_U3_HT_IRQS
static int mpic_msi_reserve_u3_hwirqs(struct mpic *mpic)
{
	irq_hw_number_t hwirq;
	struct irq_host_ops *ops = mpic->irqhost->ops;
	struct device_node *np;
	int flags, index, i;
	struct of_irq oirq;

	pr_debug("mpic: found U3, guessing msi allocator setup\n");

	/* Reserve source numbers we know are reserved in the HW */
	for (i = 0;   i < 8;   i++)
		msi_bitmap_reserve_hwirq(&mpic->msi_bitmap, i);

	for (i = 42;  i < 46;  i++)
		msi_bitmap_reserve_hwirq(&mpic->msi_bitmap, i);

	for (i = 100; i < 105; i++)
		msi_bitmap_reserve_hwirq(&mpic->msi_bitmap, i);

	np = NULL;
	while ((np = of_find_all_nodes(np))) {
		pr_debug("mpic: mapping hwirqs for %s\n", np->full_name);

		index = 0;
		while (of_irq_map_one(np, index++, &oirq) == 0) {
			ops->xlate(mpic->irqhost, NULL, oirq.specifier,
						oirq.size, &hwirq, &flags);
			msi_bitmap_reserve_hwirq(&mpic->msi_bitmap, hwirq);
		}
	}

	return 0;
}
#else
static int mpic_msi_reserve_u3_hwirqs(struct mpic *mpic)
{
	return -1;
}
#endif

int mpic_msi_init_allocator(struct mpic *mpic)
{
	int rc;

	rc = msi_bitmap_alloc(&mpic->msi_bitmap, mpic->irq_count,
			      mpic->irqhost->of_node);
	if (rc)
		return rc;

	rc = msi_bitmap_reserve_dt_hwirqs(&mpic->msi_bitmap);
	if (rc > 0) {
		if (mpic->flags & MPIC_U3_HT_IRQS)
			rc = mpic_msi_reserve_u3_hwirqs(mpic);

		if (rc) {
			msi_bitmap_free(&mpic->msi_bitmap);
			return rc;
		}
	}

	return 0;
}
