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
 *  arch/arm/mach-footbridge/include/mach/memory.h
 *
 *  Copyright (C) 1996-1999 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Changelog:
 *   20-Oct-1996 RMK	Created
 *   31-Dec-1997 RMK	Fixed definitions to reduce warnings.
 *   17-May-1998 DAG	Added __virt_to_bus and __bus_to_virt functions.
 *   21-Nov-1998 RMK	Changed __virt_to_bus and __bus_to_virt to macros.
 *   21-Mar-1999 RMK	Added PAGE_OFFSET for co285 architecture.
 *			Renamed to memory.h
 *			Moved PAGE_OFFSET and TASK_SIZE here
 */
#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H


#if defined(CONFIG_FOOTBRIDGE_ADDIN)
/*
 * If we may be using add-in footbridge mode, then we must
 * use the out-of-line translation that makes use of the
 * PCI BAR
 */
#ifndef __ASSEMBLY__
extern unsigned long __virt_to_bus(unsigned long);
extern unsigned long __bus_to_virt(unsigned long);
#endif
#define __virt_to_bus	__virt_to_bus
#define __bus_to_virt	__bus_to_virt

#elif defined(CONFIG_FOOTBRIDGE_HOST)

/*
 * The footbridge is programmed to expose the system RAM at the corresponding
 * address.  So, if PAGE_OFFSET is 0xc0000000, RAM appears at 0xe0000000.
 * If 0x80000000, then its exposed at 0xa0000000 on the bus. etc.
 * The only requirement is that the RAM isn't placed at bus address 0 which
 * would clash with VGA cards.
 */
#define __virt_to_bus(x)	((x) - 0xe0000000)
#define __bus_to_virt(x)	((x) + 0xe0000000)

#else

#error "Undefined footbridge mode"

#endif

/*
 * Cache flushing area.
 */
#define FLUSH_BASE		0xf9000000

/*
 * Physical DRAM offset.
 */
#define PHYS_OFFSET		UL(0x00000000)

#define FLUSH_BASE_PHYS		0x50000000

#endif
