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
 * Copyright 2004 James Cleverdon, IBM.
 * Subject to the GNU Public License, v.2
 *
 * Generic APIC sub-arch probe layer.
 *
 * Hacked for x86-64 by James Cleverdon from i386 architecture code by
 * Martin Bligh, Andi Kleen, James Bottomley, John Stultz, and
 * James Cleverdon.
 */
#include <linux/threads.h>
#include <linux/cpumask.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ctype.h>
#include <linux/init.h>
#include <linux/hardirq.h>
#include <linux/dmar.h>

#include <asm/smp.h>
#include <asm/ipi.h>
#include <asm/genapic.h>
#include <asm/setup.h>

extern struct genapic apic_flat;
extern struct genapic apic_physflat;
extern struct genapic apic_x2xpic_uv_x;
extern struct genapic apic_x2apic_phys;
extern struct genapic apic_x2apic_cluster;

struct genapic __read_mostly *genapic = &apic_flat;

static struct genapic *apic_probe[] __initdata = {
	&apic_x2apic_uv_x,
	&apic_x2apic_phys,
	&apic_x2apic_cluster,
	&apic_physflat,
	NULL,
};

/*
 * Check the APIC IDs in bios_cpu_apicid and choose the APIC mode.
 */
void __init setup_apic_routing(void)
{
	if (genapic == &apic_x2apic_phys || genapic == &apic_x2apic_cluster) {
		if (!intr_remapping_enabled)
			genapic = &apic_flat;
	}

	if (genapic == &apic_flat) {
		if (max_physical_apicid >= 8)
			genapic = &apic_physflat;
		printk(KERN_INFO "Setting APIC routing to %s\n", genapic->name);
	}

	if (x86_quirks->update_genapic)
		x86_quirks->update_genapic();
}

/* Same for both flat and physical. */

void apic_send_IPI_self(int vector)
{
	__send_IPI_shortcut(APIC_DEST_SELF, vector, APIC_DEST_PHYSICAL);
}

int __init acpi_madt_oem_check(char *oem_id, char *oem_table_id)
{
	int i;

	for (i = 0; apic_probe[i]; ++i) {
		if (apic_probe[i]->acpi_madt_oem_check(oem_id, oem_table_id)) {
			genapic = apic_probe[i];
			printk(KERN_INFO "Setting APIC routing to %s.\n",
				genapic->name);
			return 1;
		}
	}
	return 0;
}
