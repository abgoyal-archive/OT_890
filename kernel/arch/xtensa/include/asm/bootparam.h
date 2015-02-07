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
 * include/asm-xtensa/bootparam.h
 *
 * Definition of the Linux/Xtensa boot parameter structure
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 - 2005  Tensilica Inc.
 *
 * (Concept borrowed from the 68K port)
 */

#ifndef _XTENSA_BOOTPARAM_H
#define _XTENSA_BOOTPARAM_H

#define BP_VERSION 0x0001

#define BP_TAG_COMMAND_LINE	0x1001	/* command line (0-terminated string)*/
#define BP_TAG_INITRD		0x1002	/* ramdisk addr and size (bp_meminfo) */
#define BP_TAG_MEMORY		0x1003	/* memory addr and size (bp_meminfo) */
#define BP_TAG_SERIAL_BAUSRATE	0x1004	/* baud rate of current console. */
#define BP_TAG_SERIAL_PORT	0x1005	/* serial device of current console */

#define BP_TAG_FIRST		0x7B0B  /* first tag with a version number */
#define BP_TAG_LAST 		0x7E0B	/* last tag */

#ifndef __ASSEMBLY__

/* All records are aligned to 4 bytes */

typedef struct bp_tag {
  unsigned short id;		/* tag id */
  unsigned short size;		/* size of this record excluding the structure*/
  unsigned long data[0];	/* data */
} bp_tag_t;

typedef struct meminfo {
  unsigned long type;
  unsigned long start;
  unsigned long end;
} meminfo_t;

#define SYSMEM_BANKS_MAX 5

#define MEMORY_TYPE_CONVENTIONAL	0x1000
#define MEMORY_TYPE_NONE		0x2000

typedef struct sysmem_info {
  int nr_banks;
  meminfo_t bank[SYSMEM_BANKS_MAX];
} sysmem_info_t;

extern sysmem_info_t sysmem;

#endif
#endif



