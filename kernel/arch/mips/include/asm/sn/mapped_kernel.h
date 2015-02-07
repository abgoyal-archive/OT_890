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
 * File created by Kanoj Sarcar 06/06/00.
 * Copyright 2000 Silicon Graphics, Inc.
 */
#ifndef __ASM_SN_MAPPED_KERNEL_H
#define __ASM_SN_MAPPED_KERNEL_H

#include <linux/mmzone.h>

/*
 * Note on how mapped kernels work: the text and data section is
 * compiled at cksseg segment (LOADADDR = 0xc001c000), and the
 * init/setup/data section gets a 16M virtual address bump in the
 * ld.script file (so that tlblo0 and tlblo1 maps the sections).
 * The vmlinux.64 section addresses are put in the xkseg range
 * using the change-addresses makefile option. Use elfdump -of
 * on IRIX to see where the sections go. The Origin loader loads
 * the two sections contiguously in physical memory. The loader
 * sets the entry point into kernel_entry using a xkphys address,
 * but instead of using 0xa800000001160000, it uses the address
 * 0xa800000000160000, which is where it physically loaded that
 * code. So no jumps can be done before we have switched to using
 * cksseg addresses.
 */
#include <asm/addrspace.h>

#define REP_BASE	CAC_BASE

#ifdef CONFIG_MAPPED_KERNEL

#define MAPPED_ADDR_RO_TO_PHYS(x)	(x - REP_BASE)
#define MAPPED_ADDR_RW_TO_PHYS(x)	(x - REP_BASE - 16777216)

#define MAPPED_KERN_RO_PHYSBASE(n) (hub_data(n)->kern_vars.kv_ro_baseaddr)
#define MAPPED_KERN_RW_PHYSBASE(n) (hub_data(n)->kern_vars.kv_rw_baseaddr)

#define MAPPED_KERN_RO_TO_PHYS(x) \
				((unsigned long)MAPPED_ADDR_RO_TO_PHYS(x) | \
				MAPPED_KERN_RO_PHYSBASE(get_compact_nodeid()))
#define MAPPED_KERN_RW_TO_PHYS(x) \
				((unsigned long)MAPPED_ADDR_RW_TO_PHYS(x) | \
				MAPPED_KERN_RW_PHYSBASE(get_compact_nodeid()))

#else /* CONFIG_MAPPED_KERNEL */

#define MAPPED_KERN_RO_TO_PHYS(x)	(x - REP_BASE)
#define MAPPED_KERN_RW_TO_PHYS(x)	(x - REP_BASE)

#endif /* CONFIG_MAPPED_KERNEL */

#define MAPPED_KERN_RO_TO_K0(x)	PHYS_TO_K0(MAPPED_KERN_RO_TO_PHYS(x))
#define MAPPED_KERN_RW_TO_K0(x)	PHYS_TO_K0(MAPPED_KERN_RW_TO_PHYS(x))

#endif /* __ASM_SN_MAPPED_KERNEL_H  */
