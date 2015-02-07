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
 * sleep.c - x86-specific ACPI sleep support.
 *
 *  Copyright (C) 2001-2003 Patrick Mochel
 *  Copyright (C) 2001-2003 Pavel Machek <pavel@suse.cz>
 */

#include <linux/acpi.h>
#include <linux/bootmem.h>
#include <linux/dmi.h>
#include <linux/cpumask.h>
#include <asm/segment.h>
#include <asm/desc.h>

#include "realmode/wakeup.h"
#include "sleep.h"

unsigned long acpi_wakeup_address;
unsigned long acpi_realmode_flags;

/* address in low memory of the wakeup routine. */
static unsigned long acpi_realmode;

#if defined(CONFIG_SMP) && defined(CONFIG_64BIT)
static char temp_stack[4096];
#endif

/**
 * acpi_save_state_mem - save kernel state
 *
 * Create an identity mapped page table and copy the wakeup routine to
 * low memory.
 *
 * Note that this is too late to change acpi_wakeup_address.
 */
int acpi_save_state_mem(void)
{
	struct wakeup_header *header;

	if (!acpi_realmode) {
		printk(KERN_ERR "Could not allocate memory during boot, "
		       "S3 disabled\n");
		return -ENOMEM;
	}
	memcpy((void *)acpi_realmode, &wakeup_code_start, WAKEUP_SIZE);

	header = (struct wakeup_header *)(acpi_realmode + HEADER_OFFSET);
	if (header->signature != 0x51ee1111) {
		printk(KERN_ERR "wakeup header does not match\n");
		return -EINVAL;
	}

	header->video_mode = saved_video_mode;

	header->wakeup_jmp_seg = acpi_wakeup_address >> 4;

	/*
	 * Set up the wakeup GDT.  We set these up as Big Real Mode,
	 * that is, with limits set to 4 GB.  At least the Lenovo
	 * Thinkpad X61 is known to need this for the video BIOS
	 * initialization quirk to work; this is likely to also
	 * be the case for other laptops or integrated video devices.
	 */

	/* GDT[0]: GDT self-pointer */
	header->wakeup_gdt[0] =
		(u64)(sizeof(header->wakeup_gdt) - 1) +
		((u64)(acpi_wakeup_address +
			((char *)&header->wakeup_gdt - (char *)acpi_realmode))
				<< 16);
	/* GDT[1]: big real mode-like code segment */
	header->wakeup_gdt[1] =
		GDT_ENTRY(0x809b, acpi_wakeup_address, 0xfffff);
	/* GDT[2]: big real mode-like data segment */
	header->wakeup_gdt[2] =
		GDT_ENTRY(0x8093, acpi_wakeup_address, 0xfffff);

#ifndef CONFIG_64BIT
	store_gdt((struct desc_ptr *)&header->pmode_gdt);

	header->pmode_efer_low = nx_enabled;
	if (header->pmode_efer_low & 1) {
		/* This is strange, why not save efer, always? */
		rdmsr(MSR_EFER, header->pmode_efer_low,
			header->pmode_efer_high);
	}
#endif /* !CONFIG_64BIT */

	header->pmode_cr0 = read_cr0();
	header->pmode_cr4 = read_cr4_safe();
	header->realmode_flags = acpi_realmode_flags;
	header->real_magic = 0x12345678;

#ifndef CONFIG_64BIT
	header->pmode_entry = (u32)&wakeup_pmode_return;
	header->pmode_cr3 = (u32)(swsusp_pg_dir - __PAGE_OFFSET);
	saved_magic = 0x12345678;
#else /* CONFIG_64BIT */
	header->trampoline_segment = setup_trampoline() >> 4;
#ifdef CONFIG_SMP
	stack_start.sp = temp_stack + sizeof(temp_stack);
	early_gdt_descr.address =
			(unsigned long)get_cpu_gdt_table(smp_processor_id());
#endif
	initial_code = (unsigned long)wakeup_long64;
	saved_magic = 0x123456789abcdef0;
#endif /* CONFIG_64BIT */

	return 0;
}

/*
 * acpi_restore_state - undo effects of acpi_save_state_mem
 */
void acpi_restore_state_mem(void)
{
}


/**
 * acpi_reserve_bootmem - do _very_ early ACPI initialisation
 *
 * We allocate a page from the first 1MB of memory for the wakeup
 * routine for when we come back from a sleep state. The
 * runtime allocator allows specification of <16MB pages, but not
 * <1MB pages.
 */
void __init acpi_reserve_bootmem(void)
{
	if ((&wakeup_code_end - &wakeup_code_start) > WAKEUP_SIZE) {
		printk(KERN_ERR
		       "ACPI: Wakeup code way too big, S3 disabled.\n");
		return;
	}

	acpi_realmode = (unsigned long)alloc_bootmem_low(WAKEUP_SIZE);

	if (!acpi_realmode) {
		printk(KERN_ERR "ACPI: Cannot allocate lowmem, S3 disabled.\n");
		return;
	}

	acpi_wakeup_address = virt_to_phys((void *)acpi_realmode);
}


static int __init acpi_sleep_setup(char *str)
{
	while ((str != NULL) && (*str != '\0')) {
		if (strncmp(str, "s3_bios", 7) == 0)
			acpi_realmode_flags |= 1;
		if (strncmp(str, "s3_mode", 7) == 0)
			acpi_realmode_flags |= 2;
		if (strncmp(str, "s3_beep", 7) == 0)
			acpi_realmode_flags |= 4;
#ifdef CONFIG_HIBERNATION
		if (strncmp(str, "s4_nohwsig", 10) == 0)
			acpi_no_s4_hw_signature();
		if (strncmp(str, "s4_nonvs", 8) == 0)
			acpi_s4_no_nvs();
#endif
		if (strncmp(str, "old_ordering", 12) == 0)
			acpi_old_suspend_ordering();
		str = strchr(str, ',');
		if (str != NULL)
			str += strspn(str, ", \t");
	}
	return 1;
}

__setup("acpi_sleep=", acpi_sleep_setup);
