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
 *  include/asm-s390/setup.h
 *
 *  S390 version
 *    Copyright IBM Corp. 1999,2006
 */

#ifndef _ASM_S390_SETUP_H
#define _ASM_S390_SETUP_H

#define COMMAND_LINE_SIZE	1024

#define ARCH_COMMAND_LINE_SIZE	896

#ifdef __KERNEL__

#include <asm/types.h>

#define PARMAREA		0x10400
#define MEMORY_CHUNKS		256

#ifndef __ASSEMBLY__

#ifndef __s390x__
#define IPL_DEVICE        (*(unsigned long *)  (0x10404))
#define INITRD_START      (*(unsigned long *)  (0x1040C))
#define INITRD_SIZE       (*(unsigned long *)  (0x10414))
#else /* __s390x__ */
#define IPL_DEVICE        (*(unsigned long *)  (0x10400))
#define INITRD_START      (*(unsigned long *)  (0x10408))
#define INITRD_SIZE       (*(unsigned long *)  (0x10410))
#endif /* __s390x__ */
#define COMMAND_LINE      ((char *)            (0x10480))

#define CHUNK_READ_WRITE 0
#define CHUNK_READ_ONLY  1

struct mem_chunk {
	unsigned long addr;
	unsigned long size;
	int type;
};

extern struct mem_chunk memory_chunk[];
extern unsigned long real_memory_size;
extern int memory_end_set;
extern unsigned long memory_end;

void detect_memory_layout(struct mem_chunk chunk[]);

#ifdef CONFIG_S390_SWITCH_AMODE
extern unsigned int switch_amode;
#else
#define switch_amode	(0)
#endif

#ifdef CONFIG_S390_EXEC_PROTECT
extern unsigned int s390_noexec;
#else
#define s390_noexec	(0)
#endif

/*
 * Machine features detected in head.S
 */
extern unsigned long machine_flags;

#define MACHINE_FLAG_VM		(1UL << 0)
#define MACHINE_FLAG_IEEE	(1UL << 1)
#define MACHINE_FLAG_CSP	(1UL << 3)
#define MACHINE_FLAG_MVPG	(1UL << 4)
#define MACHINE_FLAG_DIAG44	(1UL << 5)
#define MACHINE_FLAG_IDTE	(1UL << 6)
#define MACHINE_FLAG_DIAG9C	(1UL << 7)
#define MACHINE_FLAG_MVCOS	(1UL << 8)
#define MACHINE_FLAG_KVM	(1UL << 9)
#define MACHINE_FLAG_HPAGE	(1UL << 10)
#define MACHINE_FLAG_PFMF	(1UL << 11)

#define MACHINE_IS_VM		(machine_flags & MACHINE_FLAG_VM)
#define MACHINE_IS_KVM		(machine_flags & MACHINE_FLAG_KVM)
#define MACHINE_HAS_DIAG9C	(machine_flags & MACHINE_FLAG_DIAG9C)

#ifndef __s390x__
#define MACHINE_HAS_IEEE	(machine_flags & MACHINE_FLAG_IEEE)
#define MACHINE_HAS_CSP		(machine_flags & MACHINE_FLAG_CSP)
#define MACHINE_HAS_IDTE	(0)
#define MACHINE_HAS_DIAG44	(1)
#define MACHINE_HAS_MVPG	(machine_flags & MACHINE_FLAG_MVPG)
#define MACHINE_HAS_MVCOS	(0)
#define MACHINE_HAS_HPAGE	(0)
#define MACHINE_HAS_PFMF	(0)
#else /* __s390x__ */
#define MACHINE_HAS_IEEE	(1)
#define MACHINE_HAS_CSP		(1)
#define MACHINE_HAS_IDTE	(machine_flags & MACHINE_FLAG_IDTE)
#define MACHINE_HAS_DIAG44	(machine_flags & MACHINE_FLAG_DIAG44)
#define MACHINE_HAS_MVPG	(1)
#define MACHINE_HAS_MVCOS	(machine_flags & MACHINE_FLAG_MVCOS)
#define MACHINE_HAS_HPAGE	(machine_flags & MACHINE_FLAG_HPAGE)
#define MACHINE_HAS_PFMF	(machine_flags & MACHINE_FLAG_PFMF)
#endif /* __s390x__ */

#define ZFCPDUMP_HSA_SIZE	(32UL<<20)

/*
 * Console mode. Override with conmode=
 */
extern unsigned int console_mode;
extern unsigned int console_devno;
extern unsigned int console_irq;

extern char vmhalt_cmd[];
extern char vmpoff_cmd[];

#define CONSOLE_IS_UNDEFINED	(console_mode == 0)
#define CONSOLE_IS_SCLP		(console_mode == 1)
#define CONSOLE_IS_3215		(console_mode == 2)
#define CONSOLE_IS_3270		(console_mode == 3)
#define SET_CONSOLE_SCLP	do { console_mode = 1; } while (0)
#define SET_CONSOLE_3215	do { console_mode = 2; } while (0)
#define SET_CONSOLE_3270	do { console_mode = 3; } while (0)

#define NSS_NAME_SIZE	8
extern char kernel_nss_name[];

#else /* __ASSEMBLY__ */

#ifndef __s390x__
#define IPL_DEVICE        0x10404
#define INITRD_START      0x1040C
#define INITRD_SIZE       0x10414
#else /* __s390x__ */
#define IPL_DEVICE        0x10400
#define INITRD_START      0x10408
#define INITRD_SIZE       0x10410
#endif /* __s390x__ */
#define COMMAND_LINE      0x10480

#endif /* __ASSEMBLY__ */
#endif /* __KERNEL__ */
#endif /* _ASM_S390_SETUP_H */
