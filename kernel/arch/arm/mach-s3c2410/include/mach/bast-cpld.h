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

/* arch/arm/mach-s3c2410/include/mach/bast-cpld.h
 *
 * Copyright (c) 2003,2004 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * BAST - CPLD control constants
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ASM_ARCH_BASTCPLD_H
#define __ASM_ARCH_BASTCPLD_H

/* CTRL1 - Audio LR routing */

#define BAST_CPLD_CTRL1_LRCOFF	    (0x00)
#define BAST_CPLD_CTRL1_LRCADC	    (0x01)
#define BAST_CPLD_CTRL1_LRCDAC	    (0x02)
#define BAST_CPLD_CTRL1_LRCARM	    (0x03)
#define BAST_CPLD_CTRL1_LRMASK	    (0x03)

/* CTRL2 - NAND WP control, IDE Reset assert/check */

#define BAST_CPLD_CTRL2_WNAND       (0x04)
#define BAST_CPLD_CTLR2_IDERST      (0x08)

/* CTRL3 - rom write control, CPLD identity */

#define BAST_CPLD_CTRL3_IDMASK      (0x0e)
#define BAST_CPLD_CTRL3_ROMWEN      (0x01)

/* CTRL4 - 8bit LCD interface control/status */

#define BAST_CPLD_CTRL4_LLAT	    (0x01)
#define BAST_CPLD_CTRL4_LCDRW	    (0x02)
#define BAST_CPLD_CTRL4_LCDCMD	    (0x04)
#define BAST_CPLD_CTRL4_LCDE2	    (0x01)

/* CTRL5 - DMA routing */

#define BAST_CPLD_DMA0_PRIIDE      (0<<0)
#define BAST_CPLD_DMA0_SECIDE      (1<<0)
#define BAST_CPLD_DMA0_ISA15       (2<<0)
#define BAST_CPLD_DMA0_ISA36       (3<<0)

#define BAST_CPLD_DMA1_PRIIDE      (0<<2)
#define BAST_CPLD_DMA1_SECIDE      (1<<2)
#define BAST_CPLD_DMA1_ISA15       (2<<2)
#define BAST_CPLD_DMA1_ISA36       (3<<2)

#endif /* __ASM_ARCH_BASTCPLD_H */
