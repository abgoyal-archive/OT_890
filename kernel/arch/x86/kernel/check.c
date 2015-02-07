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
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/workqueue.h>
#include <asm/e820.h>
#include <asm/proto.h>

/*
 * Some BIOSes seem to corrupt the low 64k of memory during events
 * like suspend/resume and unplugging an HDMI cable.  Reserve all
 * remaining free memory in that area and fill it with a distinct
 * pattern.
 */
#define MAX_SCAN_AREAS	8

static int __read_mostly memory_corruption_check = -1;

static unsigned __read_mostly corruption_check_size = 64*1024;
static unsigned __read_mostly corruption_check_period = 60; /* seconds */

static struct e820entry scan_areas[MAX_SCAN_AREAS];
static int num_scan_areas;


static __init int set_corruption_check(char *arg)
{
	char *end;

	memory_corruption_check = simple_strtol(arg, &end, 10);

	return (*end == 0) ? 0 : -EINVAL;
}
early_param("memory_corruption_check", set_corruption_check);

static __init int set_corruption_check_period(char *arg)
{
	char *end;

	corruption_check_period = simple_strtoul(arg, &end, 10);

	return (*end == 0) ? 0 : -EINVAL;
}
early_param("memory_corruption_check_period", set_corruption_check_period);

static __init int set_corruption_check_size(char *arg)
{
	char *end;
	unsigned size;

	size = memparse(arg, &end);

	if (*end == '\0')
		corruption_check_size = size;

	return (size == corruption_check_size) ? 0 : -EINVAL;
}
early_param("memory_corruption_check_size", set_corruption_check_size);


void __init setup_bios_corruption_check(void)
{
	u64 addr = PAGE_SIZE;	/* assume first page is reserved anyway */

	if (memory_corruption_check == -1) {
		memory_corruption_check =
#ifdef CONFIG_X86_BOOTPARAM_MEMORY_CORRUPTION_CHECK
			1
#else
			0
#endif
			;
	}

	if (corruption_check_size == 0)
		memory_corruption_check = 0;

	if (!memory_corruption_check)
		return;

	corruption_check_size = round_up(corruption_check_size, PAGE_SIZE);

	while (addr < corruption_check_size && num_scan_areas < MAX_SCAN_AREAS) {
		u64 size;
		addr = find_e820_area_size(addr, &size, PAGE_SIZE);

		if (addr == 0)
			break;

		if ((addr + size) > corruption_check_size)
			size = corruption_check_size - addr;

		if (size == 0)
			break;

		e820_update_range(addr, size, E820_RAM, E820_RESERVED);
		scan_areas[num_scan_areas].addr = addr;
		scan_areas[num_scan_areas].size = size;
		num_scan_areas++;

		/* Assume we've already mapped this early memory */
		memset(__va(addr), 0, size);

		addr += size;
	}

	printk(KERN_INFO "Scanning %d areas for low memory corruption\n",
	       num_scan_areas);
	update_e820();
}


void check_for_bios_corruption(void)
{
	int i;
	int corruption = 0;

	if (!memory_corruption_check)
		return;

	for (i = 0; i < num_scan_areas; i++) {
		unsigned long *addr = __va(scan_areas[i].addr);
		unsigned long size = scan_areas[i].size;

		for (; size; addr++, size -= sizeof(unsigned long)) {
			if (!*addr)
				continue;
			printk(KERN_ERR "Corrupted low memory at %p (%lx phys) = %08lx\n",
			       addr, __pa(addr), *addr);
			corruption = 1;
			*addr = 0;
		}
	}

	WARN_ONCE(corruption, KERN_ERR "Memory corruption detected in low memory\n");
}

static void check_corruption(struct work_struct *dummy);
static DECLARE_DELAYED_WORK(bios_check_work, check_corruption);

static void check_corruption(struct work_struct *dummy)
{
	check_for_bios_corruption();
	schedule_delayed_work(&bios_check_work,
		round_jiffies_relative(corruption_check_period*HZ)); 
}

static int start_periodic_check_for_corruption(void)
{
	if (!memory_corruption_check || corruption_check_period == 0)
		return 0;

	printk(KERN_INFO "Scanning for low memory corruption every %d seconds\n",
	       corruption_check_period);

	/* First time we run the checks right away */
	schedule_delayed_work(&bios_check_work, 0);
	return 0;
}

module_init(start_periodic_check_for_corruption);

