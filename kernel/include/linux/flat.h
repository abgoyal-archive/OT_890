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
 * Copyright (C) 2002-2003  David McCullough <davidm@snapgear.com>
 * Copyright (C) 1998       Kenneth Albanowski <kjahds@kjahds.com>
 *                          The Silver Hammer Group, Ltd.
 *
 * This file provides the definitions and structures needed to
 * support uClinux flat-format executables.
 */

#ifndef _LINUX_FLAT_H
#define _LINUX_FLAT_H

#ifdef __KERNEL__
#include <asm/flat.h>
#endif

#define	FLAT_VERSION			0x00000004L

#ifdef CONFIG_BINFMT_SHARED_FLAT
#define	MAX_SHARED_LIBS			(4)
#else
#define	MAX_SHARED_LIBS			(1)
#endif

/*
 * To make everything easier to port and manage cross platform
 * development,  all fields are in network byte order.
 */

struct flat_hdr {
	char magic[4];
	unsigned long rev;          /* version (as above) */
	unsigned long entry;        /* Offset of first executable instruction
	                               with text segment from beginning of file */
	unsigned long data_start;   /* Offset of data segment from beginning of
	                               file */
	unsigned long data_end;     /* Offset of end of data segment
	                               from beginning of file */
	unsigned long bss_end;      /* Offset of end of bss segment from beginning
	                               of file */

	/* (It is assumed that data_end through bss_end forms the bss segment.) */

	unsigned long stack_size;   /* Size of stack, in bytes */
	unsigned long reloc_start;  /* Offset of relocation records from
	                               beginning of file */
	unsigned long reloc_count;  /* Number of relocation records */
	unsigned long flags;       
	unsigned long build_date;   /* When the program/library was built */
	unsigned long filler[5];    /* Reservered, set to zero */
};

#define FLAT_FLAG_RAM    0x0001 /* load program entirely into RAM */
#define FLAT_FLAG_GOTPIC 0x0002 /* program is PIC with GOT */
#define FLAT_FLAG_GZIP   0x0004 /* all but the header is compressed */
#define FLAT_FLAG_GZDATA 0x0008 /* only data/relocs are compressed (for XIP) */
#define FLAT_FLAG_KTRACE 0x0010 /* output useful kernel trace for debugging */


#ifdef __KERNEL__ /* so systems without linux headers can compile the apps */
/*
 * While it would be nice to keep this header clean,  users of older
 * tools still need this support in the kernel.  So this section is
 * purely for compatibility with old tool chains.
 *
 * DO NOT make changes or enhancements to the old format please,  just work
 *        with the format above,  except to fix bugs with old format support.
 */

#include <asm/byteorder.h>

#define	OLD_FLAT_VERSION			0x00000002L
#define OLD_FLAT_RELOC_TYPE_TEXT	0
#define OLD_FLAT_RELOC_TYPE_DATA	1
#define OLD_FLAT_RELOC_TYPE_BSS		2

typedef union {
	unsigned long	value;
	struct {
# if defined(mc68000) && !defined(CONFIG_COLDFIRE)
		signed long offset : 30;
		unsigned long type : 2;
#   	define OLD_FLAT_FLAG_RAM    0x1 /* load program entirely into RAM */
# elif defined(__BIG_ENDIAN_BITFIELD)
		unsigned long type : 2;
		signed long offset : 30;
#   	define OLD_FLAT_FLAG_RAM    0x1 /* load program entirely into RAM */
# elif defined(__LITTLE_ENDIAN_BITFIELD)
		signed long offset : 30;
		unsigned long type : 2;
#   	define OLD_FLAT_FLAG_RAM    0x1 /* load program entirely into RAM */
# else
#   	error "Unknown bitfield order for flat files."
# endif
	} reloc;
} flat_v2_reloc_t;

#endif /* __KERNEL__ */

#endif /* _LINUX_FLAT_H */
