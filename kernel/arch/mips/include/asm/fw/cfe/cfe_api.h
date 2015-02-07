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
 * Copyright (C) 2000, 2001, 2002 Broadcom Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
/*
 * Broadcom Common Firmware Environment (CFE)
 *
 * This file contains declarations for doing callbacks to
 * cfe from an application.  It should be the only header
 * needed by the application to use this library
 *
 * Authors:  Mitch Lichtenberg, Chris Demetriou
 */
#ifndef CFE_API_H
#define CFE_API_H

#include <linux/types.h>
#include <linux/string.h>

typedef long intptr_t;


/*
 * Constants
 */

/* Seal indicating CFE's presence, passed to user program. */
#define CFE_EPTSEAL 0x43464531

#define CFE_MI_RESERVED	0	/* memory is reserved, do not use */
#define CFE_MI_AVAILABLE 1	/* memory is available */

#define CFE_FLG_WARMSTART     0x00000001
#define CFE_FLG_FULL_ARENA    0x00000001
#define CFE_FLG_ENV_PERMANENT 0x00000001

#define CFE_CPU_CMD_START 1
#define CFE_CPU_CMD_STOP 0

#define CFE_STDHANDLE_CONSOLE	0

#define CFE_DEV_NETWORK 	1
#define CFE_DEV_DISK		2
#define CFE_DEV_FLASH		3
#define CFE_DEV_SERIAL		4
#define CFE_DEV_CPU		5
#define CFE_DEV_NVRAM		6
#define CFE_DEV_CLOCK           7
#define CFE_DEV_OTHER		8
#define CFE_DEV_MASK		0x0F

#define CFE_CACHE_FLUSH_D	1
#define CFE_CACHE_INVAL_I	2
#define CFE_CACHE_INVAL_D	4
#define CFE_CACHE_INVAL_L2	8

#define CFE_FWI_64BIT		0x00000001
#define CFE_FWI_32BIT		0x00000002
#define CFE_FWI_RELOC		0x00000004
#define CFE_FWI_UNCACHED	0x00000008
#define CFE_FWI_MULTICPU	0x00000010
#define CFE_FWI_FUNCSIM		0x00000020
#define CFE_FWI_RTLSIM		0x00000040

typedef struct {
	int64_t fwi_version;		/* major, minor, eco version */
	int64_t fwi_totalmem;		/* total installed mem */
	int64_t fwi_flags;		/* various flags */
	int64_t fwi_boardid;		/* board ID */
	int64_t fwi_bootarea_va;	/* VA of boot area */
	int64_t fwi_bootarea_pa;	/* PA of boot area */
	int64_t fwi_bootarea_size;	/* size of boot area */
} cfe_fwinfo_t;


/*
 * Defines and prototypes for functions which take no arguments.
 */
int64_t cfe_getticks(void);

/*
 * Defines and prototypes for the rest of the functions.
 */
int cfe_close(int handle);
int cfe_cpu_start(int cpu, void (*fn) (void), long sp, long gp, long a1);
int cfe_cpu_stop(int cpu);
int cfe_enumenv(int idx, char *name, int namelen, char *val, int vallen);
int cfe_enummem(int idx, int flags, uint64_t * start, uint64_t * length,
		uint64_t * type);
int cfe_exit(int warm, int status);
int cfe_flushcache(int flg);
int cfe_getdevinfo(char *name);
int cfe_getenv(char *name, char *dest, int destlen);
int cfe_getfwinfo(cfe_fwinfo_t * info);
int cfe_getstdhandle(int flg);
int cfe_init(uint64_t handle, uint64_t ept);
int cfe_inpstat(int handle);
int cfe_ioctl(int handle, unsigned int ioctlnum, unsigned char *buffer,
	      int length, int *retlen, uint64_t offset);
int cfe_open(char *name);
int cfe_read(int handle, unsigned char *buffer, int length);
int cfe_readblk(int handle, int64_t offset, unsigned char *buffer,
		int length);
int cfe_setenv(char *name, char *val);
int cfe_write(int handle, unsigned char *buffer, int length);
int cfe_writeblk(int handle, int64_t offset, unsigned char *buffer,
		 int length);

#endif				/* CFE_API_H */
