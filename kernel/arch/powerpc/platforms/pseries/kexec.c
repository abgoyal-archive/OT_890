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
 *  Copyright 2006 Michael Ellerman, IBM Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <asm/machdep.h>
#include <asm/page.h>
#include <asm/firmware.h>
#include <asm/kexec.h>
#include <asm/mpic.h>
#include <asm/smp.h>

#include "pseries.h"
#include "xics.h"
#include "plpar_wrappers.h"

static void pseries_kexec_cpu_down(int crash_shutdown, int secondary)
{
	/* Don't risk a hypervisor call if we're crashing */
	if (firmware_has_feature(FW_FEATURE_SPLPAR) && !crash_shutdown) {
		unsigned long addr;

		addr = __pa(get_slb_shadow());
		if (unregister_slb_shadow(hard_smp_processor_id(), addr))
			printk("SLB shadow buffer deregistration of "
			       "cpu %u (hw_cpu_id %d) failed\n",
			       smp_processor_id(),
			       hard_smp_processor_id());

		addr = __pa(get_lppaca());
		if (unregister_vpa(hard_smp_processor_id(), addr)) {
			printk("VPA deregistration of cpu %u (hw_cpu_id %d) "
					"failed\n", smp_processor_id(),
					hard_smp_processor_id());
		}
	}
}

static void pseries_kexec_cpu_down_mpic(int crash_shutdown, int secondary)
{
	pseries_kexec_cpu_down(crash_shutdown, secondary);
	mpic_teardown_this_cpu(secondary);
}

void __init setup_kexec_cpu_down_mpic(void)
{
	ppc_md.kexec_cpu_down = pseries_kexec_cpu_down_mpic;
}

static void pseries_kexec_cpu_down_xics(int crash_shutdown, int secondary)
{
	pseries_kexec_cpu_down(crash_shutdown, secondary);
	xics_kexec_teardown_cpu(secondary);
}

void __init setup_kexec_cpu_down_xics(void)
{
	ppc_md.kexec_cpu_down = pseries_kexec_cpu_down_xics;
}

static int __init pseries_kexec_setup(void)
{
	ppc_md.machine_kexec = default_machine_kexec;
	ppc_md.machine_kexec_prepare = default_machine_kexec_prepare;
	ppc_md.machine_crash_shutdown = default_machine_crash_shutdown;

	return 0;
}
machine_device_initcall(pseries, pseries_kexec_setup);
