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

#ifndef _SPARC_VADDRS_H
#define _SPARC_VADDRS_H

#include <asm/head.h>

/*
 * asm/vaddrs.h:  Here we define the virtual addresses at
 *                      which important things will be mapped.
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 * Copyright (C) 2000 Anton Blanchard (anton@samba.org)
 */

#define SRMMU_MAXMEM		0x0c000000

#define SRMMU_NOCACHE_VADDR	(KERNBASE + SRMMU_MAXMEM)
				/* = 0x0fc000000 */
/* XXX Empiricals - this needs to go away - KMW */
#define SRMMU_MIN_NOCACHE_PAGES (550)
#define SRMMU_MAX_NOCACHE_PAGES	(1280)

/* The following constant is used in mm/srmmu.c::srmmu_nocache_calcsize()
 * to determine the amount of memory that will be reserved as nocache:
 *
 * 256 pages will be taken as nocache per each
 * SRMMU_NOCACHE_ALCRATIO MB of system memory.
 *
 * limits enforced:	nocache minimum = 256 pages
 *			nocache maximum = 1280 pages
 */
#define SRMMU_NOCACHE_ALCRATIO	64	/* 256 pages per 64MB of system RAM */

#define SUN4M_IOBASE_VADDR	0xfd000000 /* Base for mapping pages */
#define IOBASE_VADDR		0xfe000000
#define IOBASE_END		0xfe600000

/*
 * On the sun4/4c we need a place
 * to reliably map locked down kernel data.  This includes the
 * task_struct and kernel stack pages of each process plus the
 * scsi buffers during dvma IO transfers, also the floppy buffers
 * during pseudo dma which runs with traps off (no faults allowed).
 * Some quick calculations yield:
 *       NR_TASKS <512> * (3 * PAGE_SIZE) == 0x600000
 * Subtract this from 0xc00000 and you get 0x927C0 of vm left
 * over to map SCSI dvma + floppy pseudo-dma buffers.  So be
 * careful if you change NR_TASKS or else there won't be enough
 * room for it all.
 */
#define SUN4C_LOCK_VADDR	0xff000000
#define SUN4C_LOCK_END		0xffc00000

#define KADB_DEBUGGER_BEGVM	0xffc00000 /* Where kern debugger is in virt-mem */
#define KADB_DEBUGGER_ENDVM	0xffd00000
#define DEBUG_FIRSTVADDR	KADB_DEBUGGER_BEGVM
#define DEBUG_LASTVADDR		KADB_DEBUGGER_ENDVM

#define LINUX_OPPROM_BEGVM	0xffd00000
#define LINUX_OPPROM_ENDVM	0xfff00000

#define DVMA_VADDR		0xfff00000 /* Base area of the DVMA on suns */
#define DVMA_END		0xfffc0000

#endif /* !(_SPARC_VADDRS_H) */
