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

/* two abstractions specific to kernel/smpboot.c, mainly to cater to visws
 * which needs to alter them. */

static inline void smpboot_clear_io_apic_irqs(void)
{
#ifdef CONFIG_X86_IO_APIC
	io_apic_irqs = 0;
#endif
}

static inline void smpboot_setup_warm_reset_vector(unsigned long start_eip)
{
	CMOS_WRITE(0xa, 0xf);
	local_flush_tlb();
	pr_debug("1.\n");
	*((volatile unsigned short *)phys_to_virt(TRAMPOLINE_PHYS_HIGH)) =
								 start_eip >> 4;
	pr_debug("2.\n");
	*((volatile unsigned short *)phys_to_virt(TRAMPOLINE_PHYS_LOW)) =
							 start_eip & 0xf;
	pr_debug("3.\n");
}

static inline void smpboot_restore_warm_reset_vector(void)
{
	/*
	 * Install writable page 0 entry to set BIOS data area.
	 */
	local_flush_tlb();

	/*
	 * Paranoid:  Set warm reset code and vector here back
	 * to default values.
	 */
	CMOS_WRITE(0, 0xf);

	*((volatile long *)phys_to_virt(TRAMPOLINE_PHYS_LOW)) = 0;
}

static inline void __init smpboot_setup_io_apic(void)
{
#ifdef CONFIG_X86_IO_APIC
	/*
	 * Here we can be sure that there is an IO-APIC in the system. Let's
	 * go and set it up:
	 */
	if (!skip_ioapic_setup && nr_ioapics)
		setup_IO_APIC();
	else {
		nr_ioapics = 0;
		localise_nmi_watchdog();
	}
#endif
}

static inline void smpboot_clear_io_apic(void)
{
#ifdef CONFIG_X86_IO_APIC
	nr_ioapics = 0;
#endif
}
