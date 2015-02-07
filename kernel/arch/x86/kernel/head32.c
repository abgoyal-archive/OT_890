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
 *  linux/arch/i386/kernel/head32.c -- prepare to run common code
 *
 *  Copyright (C) 2000 Andrea Arcangeli <andrea@suse.de> SuSE
 *  Copyright (C) 2007 Eric Biederman <ebiederm@xmission.com>
 */

#include <linux/init.h>
#include <linux/start_kernel.h>

#include <asm/setup.h>
#include <asm/sections.h>
#include <asm/e820.h>
#include <asm/bios_ebda.h>
#include <asm/trampoline.h>

void __init i386_start_kernel(void)
{
	reserve_trampoline_memory();

	reserve_early(__pa_symbol(&_text), __pa_symbol(&_end), "TEXT DATA BSS");

#ifdef CONFIG_BLK_DEV_INITRD
	/* Reserve INITRD */
	if (boot_params.hdr.type_of_loader && boot_params.hdr.ramdisk_image) {
		u64 ramdisk_image = boot_params.hdr.ramdisk_image;
		u64 ramdisk_size  = boot_params.hdr.ramdisk_size;
		u64 ramdisk_end   = ramdisk_image + ramdisk_size;
		reserve_early(ramdisk_image, ramdisk_end, "RAMDISK");
	}
#endif
	reserve_early(init_pg_tables_start, init_pg_tables_end,
			"INIT_PG_TABLE");

	reserve_ebda_region();

	/*
	 * At this point everything still needed from the boot loader
	 * or BIOS or kernel text should be early reserved or marked not
	 * RAM in e820. All other memory is free game.
	 */

	start_kernel();
}
