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

#include <asm/setup.h>
#include <asm/bios_ebda.h>

#define BIOS_LOWMEM_KILOBYTES 0x413

/*
 * The BIOS places the EBDA/XBDA at the top of conventional
 * memory, and usually decreases the reported amount of
 * conventional memory (int 0x12) too. This also contains a
 * workaround for Dell systems that neglect to reserve EBDA.
 * The same workaround also avoids a problem with the AMD768MPX
 * chipset: reserve a page before VGA to prevent PCI prefetch
 * into it (errata #56). Usually the page is reserved anyways,
 * unless you have no PS/2 mouse plugged in.
 */
void __init reserve_ebda_region(void)
{
	unsigned int lowmem, ebda_addr;

	/* To determine the position of the EBDA and the */
	/* end of conventional memory, we need to look at */
	/* the BIOS data area. In a paravirtual environment */
	/* that area is absent. We'll just have to assume */
	/* that the paravirt case can handle memory setup */
	/* correctly, without our help. */
	if (paravirt_enabled())
		return;

	/* end of low (conventional) memory */
	lowmem = *(unsigned short *)__va(BIOS_LOWMEM_KILOBYTES);
	lowmem <<= 10;

	/* start of EBDA area */
	ebda_addr = get_bios_ebda();

	/* Fixup: bios puts an EBDA in the top 64K segment */
	/* of conventional memory, but does not adjust lowmem. */
	if ((lowmem - ebda_addr) <= 0x10000)
		lowmem = ebda_addr;

	/* Fixup: bios does not report an EBDA at all. */
	/* Some old Dells seem to need 4k anyhow (bugzilla 2990) */
	if ((ebda_addr == 0) && (lowmem >= 0x9f000))
		lowmem = 0x9f000;

	/* Paranoia: should never happen, but... */
	if ((lowmem == 0) || (lowmem >= 0x100000))
		lowmem = 0x9f000;

	/* reserve all memory between lowmem and the 1MB mark */
	reserve_early_overlap_ok(lowmem, 0x100000, "BIOS reserved");
}
