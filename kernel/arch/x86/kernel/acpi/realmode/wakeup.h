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
 * Definitions for the wakeup data structure at the head of the
 * wakeup code.
 */

#ifndef ARCH_X86_KERNEL_ACPI_RM_WAKEUP_H
#define ARCH_X86_KERNEL_ACPI_RM_WAKEUP_H

#ifndef __ASSEMBLY__
#include <linux/types.h>

/* This must match data at wakeup.S */
struct wakeup_header {
	u16 video_mode;		/* Video mode number */
	u16 _jmp1;		/* ljmpl opcode, 32-bit only */
	u32 pmode_entry;	/* Protected mode resume point, 32-bit only */
	u16 _jmp2;		/* CS value, 32-bit only */
	u32 pmode_cr0;		/* Protected mode cr0 */
	u32 pmode_cr3;		/* Protected mode cr3 */
	u32 pmode_cr4;		/* Protected mode cr4 */
	u32 pmode_efer_low;	/* Protected mode EFER */
	u32 pmode_efer_high;
	u64 pmode_gdt;
	u32 realmode_flags;
	u32 real_magic;
	u16 trampoline_segment;	/* segment with trampoline code, 64-bit only */
	u8  _pad1;
	u8  wakeup_jmp;
	u16 wakeup_jmp_off;
	u16 wakeup_jmp_seg;
	u64 wakeup_gdt[3];
	u32 signature;		/* To check we have correct structure */
} __attribute__((__packed__));

extern struct wakeup_header wakeup_header;
#endif

#define HEADER_OFFSET 0x3f00
#define WAKEUP_SIZE   0x4000

#endif /* ARCH_X86_KERNEL_ACPI_RM_WAKEUP_H */
