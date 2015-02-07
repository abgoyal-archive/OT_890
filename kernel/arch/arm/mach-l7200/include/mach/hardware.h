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
 * arch/arm/mach-l7200/include/mach/hardware.h
 *
 * Copyright (C) 2000 Rob Scott (rscott@mtrob.fdns.net)
 *                    Steve Hill (sjhill@cotw.com)
 *
 * This file contains the hardware definitions for the 
 * LinkUp Systems L7200 SOC development board.
 *
 * Changelog:
 *   02-01-2000	 RS	Created L7200 version, derived from rpc code
 *   03-21-2000	SJH	Cleaned up file
 *   04-21-2000	 RS 	Changed mapping of I/O in virtual space
 *   04-25-2000	SJH	Removed unused symbols and such
 *   05-05-2000	SJH	Complete rewrite
 *   07-31-2000	SJH	Added undocumented debug auxillary port to
 *			get at last two columns for keyboard driver
 */
#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H

/* Hardware addresses of major areas.
 *  *_START is the physical address
 *  *_SIZE  is the size of the region
 *  *_BASE  is the virtual address
 */
#define RAM_START		0xf0000000
#define RAM_SIZE		0x02000000
#define RAM_BASE		0xc0000000

#define IO_START		0x80000000      /* I/O */
#define IO_SIZE			0x01000000
#define IO_BASE			0xd0000000

#define IO_START_2		0x90000000      /* I/O */
#define IO_SIZE_2		0x01000000
#define IO_BASE_2		0xd1000000

#define AUX_START		0x1a000000      /* AUX PORT */
#define AUX_SIZE 		0x01000000
#define AUX_BASE		0xd2000000

#define FLASH1_START		0x00000000      /* FLASH BANK 1 */
#define FLASH1_SIZE 		0x01000000
#define FLASH1_BASE		0xd3000000

#define FLASH2_START		0x10000000      /* FLASH BANK 2 */
#define FLASH2_SIZE 		0x01000000
#define FLASH2_BASE		0xd4000000

#define ISA_START		0x20000000	/* ISA */
#define ISA_SIZE		0x20000000
#define ISA_BASE		0xe0000000

#define PCIO_BASE		IO_BASE

#endif
