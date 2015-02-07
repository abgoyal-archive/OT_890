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

#include <linux/module.h>

#include <asm/machvec.h>
#include <asm/system.h>

#ifdef CONFIG_IA64_GENERIC

#include <linux/kernel.h>
#include <linux/string.h>

#include <asm/page.h>

struct ia64_machine_vector ia64_mv;
EXPORT_SYMBOL(ia64_mv);

static struct ia64_machine_vector * __init
lookup_machvec (const char *name)
{
	extern struct ia64_machine_vector machvec_start[];
	extern struct ia64_machine_vector machvec_end[];
	struct ia64_machine_vector *mv;

	for (mv = machvec_start; mv < machvec_end; ++mv)
		if (strcmp (mv->name, name) == 0)
			return mv;

	return 0;
}

void __init
machvec_init (const char *name)
{
	struct ia64_machine_vector *mv;

	if (!name)
		name = acpi_get_sysname();
	mv = lookup_machvec(name);
	if (!mv)
		panic("generic kernel failed to find machine vector for"
		      " platform %s!", name);

	ia64_mv = *mv;
	printk(KERN_INFO "booting generic kernel on platform %s\n", name);
}

void __init
machvec_init_from_cmdline(const char *cmdline)
{
	char str[64];
	const char *start;
	char *end;

	if (! (start = strstr(cmdline, "machvec=")) )
		return machvec_init(NULL);

	strlcpy(str, start + strlen("machvec="), sizeof(str));
	if ( (end = strchr(str, ' ')) )
		*end = '\0';

	return machvec_init(str);
}

#endif /* CONFIG_IA64_GENERIC */

void
machvec_setup (char **arg)
{
}
EXPORT_SYMBOL(machvec_setup);

void
machvec_timer_interrupt (int irq, void *dev_id)
{
}
EXPORT_SYMBOL(machvec_timer_interrupt);

void
machvec_dma_sync_single (struct device *hwdev, dma_addr_t dma_handle, size_t size, int dir)
{
	mb();
}
EXPORT_SYMBOL(machvec_dma_sync_single);

void
machvec_dma_sync_sg (struct device *hwdev, struct scatterlist *sg, int n, int dir)
{
	mb();
}
EXPORT_SYMBOL(machvec_dma_sync_sg);
