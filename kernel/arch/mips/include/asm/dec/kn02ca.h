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
 *	include/asm-mips/dec/kn02ca.h
 *
 *	Personal DECstation 5000/xx (Maxine or KN02-CA) definitions.
 *
 *	Copyright (C) 2002, 2003  Maciej W. Rozycki
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */
#ifndef __ASM_MIPS_DEC_KN02CA_H
#define __ASM_MIPS_DEC_KN02CA_H

#include <asm/dec/kn02xa.h>		/* For common definitions. */

/*
 * CPU interrupt bits.
 */
#define KN02CA_CPU_INR_HALT	6	/* HALT from ACCESS.Bus */
#define KN02CA_CPU_INR_CASCADE	5	/* I/O ASIC cascade */
#define KN02CA_CPU_INR_BUS	4	/* memory, I/O bus read/write errors */
#define KN02CA_CPU_INR_RTC	3	/* DS1287 RTC */
#define KN02CA_CPU_INR_TIMER	2	/* ARC periodic timer */

/*
 * I/O ASIC interrupt bits.  Star marks denote non-IRQ status bits.
 */
#define KN02CA_IO_INR_FLOPPY	15	/* 82077 FDC */
#define KN02CA_IO_INR_NVRAM	14	/* (*) NVRAM clear jumper */
#define KN02CA_IO_INR_POWERON	13	/* (*) ACCESS.Bus/power-on reset */
#define KN02CA_IO_INR_TC0	12	/* TURBOchannel slot #0 */
#define KN02CA_IO_INR_TIMER	12	/* ARC periodic timer (?) */
#define KN02CA_IO_INR_ISDN	11	/* Am79C30A ISDN */
#define KN02CA_IO_INR_NRMOD	10	/* (*) NRMOD manufacturing jumper */
#define KN02CA_IO_INR_ASC	9	/* ASC (NCR53C94) SCSI */
#define KN02CA_IO_INR_LANCE	8	/* LANCE (Am7990) Ethernet */
#define KN02CA_IO_INR_HDFLOPPY	7	/* (*) HD (1.44MB) floppy status */
#define KN02CA_IO_INR_SCC0	6	/* SCC (Z85C30) serial #0 */
#define KN02CA_IO_INR_TC1	5	/* TURBOchannel slot #1 */
#define KN02CA_IO_INR_XDFLOPPY	4	/* (*) XD (2.88MB) floppy status */
#define KN02CA_IO_INR_VIDEO	3	/* framebuffer */
#define KN02CA_IO_INR_XVIDEO	2	/* ~framebuffer */
#define KN02CA_IO_INR_AB_XMIT	1	/* ACCESS.bus transmit */
#define KN02CA_IO_INR_AB_RECV	0	/* ACCESS.bus receive */


/*
 * Memory Error Register bits.
 */
#define KN02CA_MER_INTR		(1<<27)		/* ARC IRQ status & ack */

/*
 * Memory Size Register bits.
 */
#define KN02CA_MSR_INTREN	(1<<26)		/* ARC periodic IRQ enable */
#define KN02CA_MSR_MS10EN	(1<<25)		/* 10/1ms IRQ period select */
#define KN02CA_MSR_PFORCE	(0xf<<21)	/* byte lane error force */
#define KN02CA_MSR_MABEN	(1<<20)		/* A side VFB address enable */
#define KN02CA_MSR_LASTBANK	(0x7<<17)	/* onboard RAM bank # */

/*
 * I/O ASIC System Support Register bits.
 */
#define KN03CA_IO_SSR_RES_14	(1<<14)		/* unused */
#define KN03CA_IO_SSR_RES_13	(1<<13)		/* unused */
#define KN03CA_IO_SSR_ISDN_RST	(1<<12)		/* ~ISDN (Am79C30A) reset */

#define KN03CA_IO_SSR_FLOPPY_RST (1<<7)		/* ~FDC (82077) reset */
#define KN03CA_IO_SSR_VIDEO_RST	(1<<6)		/* ~framebuffer reset */
#define KN03CA_IO_SSR_AB_RST	(1<<5)		/* ACCESS.bus reset */
#define KN03CA_IO_SSR_RES_4	(1<<4)		/* unused */
#define KN03CA_IO_SSR_RES_3	(1<<4)		/* unused */
#define KN03CA_IO_SSR_RES_2	(1<<2)		/* unused */
#define KN03CA_IO_SSR_RES_1	(1<<1)		/* unused */
#define KN03CA_IO_SSR_LED	(1<<0)		/* power LED */

#endif /* __ASM_MIPS_DEC_KN02CA_H */
