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

#ifndef _PPC64_KDUMP_H
#define _PPC64_KDUMP_H

#include <asm/page.h>

/* Kdump kernel runs at 32 MB, change at your peril. */
#define KDUMP_KERNELBASE	0x2000000

/* How many bytes to reserve at zero for kdump. The reserve limit should
 * be greater or equal to the trampoline's end address.
 * Reserve to the end of the FWNMI area, see head_64.S */
#define KDUMP_RESERVE_LIMIT	0x10000 /* 64K */

#ifdef CONFIG_CRASH_DUMP

/*
 * On PPC64 translation is disabled during trampoline setup, so we use
 * physical addresses. Though on PPC32 translation is already enabled,
 * so we can't do the same. Luckily create_trampoline() creates relative
 * branches, so we can just add the PAGE_OFFSET and don't worry about it.
 */
#ifdef __powerpc64__
#define KDUMP_TRAMPOLINE_START	0x0100
#define KDUMP_TRAMPOLINE_END	0x3000
#else
#define KDUMP_TRAMPOLINE_START	(0x0100 + PAGE_OFFSET)
#define KDUMP_TRAMPOLINE_END	(0x3000 + PAGE_OFFSET)
#endif /* __powerpc64__ */

#define KDUMP_MIN_TCE_ENTRIES	2048

#endif /* CONFIG_CRASH_DUMP */

#ifndef __ASSEMBLY__

#if defined(CONFIG_CRASH_DUMP) && !defined(CONFIG_RELOCATABLE)
extern void reserve_kdump_trampoline(void);
extern void setup_kdump_trampoline(void);
#else
/* !CRASH_DUMP || RELOCATABLE */
static inline void reserve_kdump_trampoline(void) { ; }
static inline void setup_kdump_trampoline(void) { ; }
#endif

#endif /* __ASSEMBLY__ */

#endif /* __PPC64_KDUMP_H */
