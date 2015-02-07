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

/* arch/arm/mach-s3c2410/include/mach/anubis-map.h
 *
 * Copyright (c) 2005 Simtec Electronics
 *	http://www.simtec.co.uk/products/
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * ANUBIS - Memory map definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

/* needs arch/map.h including with this */

#ifndef __ASM_ARCH_ANUBISMAP_H
#define __ASM_ARCH_ANUBISMAP_H

/* start peripherals off after the S3C2410 */

#define ANUBIS_IOADDR(x)	(S3C2410_ADDR((x) + 0x01800000))

#define ANUBIS_PA_CPLD		(S3C2410_CS1 | (1<<26))

/* we put the CPLD registers next, to get them out of the way */

#define ANUBIS_VA_CTRL1	    ANUBIS_IOADDR(0x00000000)	 /* 0x01800000 */
#define ANUBIS_PA_CTRL1	    (ANUBIS_PA_CPLD)

#define ANUBIS_VA_IDREG	    ANUBIS_IOADDR(0x00300000)	 /* 0x01B00000 */
#define ANUBIS_PA_IDREG	    (ANUBIS_PA_CPLD + (3<<23))

#define ANUBIS_IDEPRI	    ANUBIS_IOADDR(0x01000000)
#define ANUBIS_IDEPRIAUX    ANUBIS_IOADDR(0x01100000)
#define ANUBIS_IDESEC	    ANUBIS_IOADDR(0x01200000)
#define ANUBIS_IDESECAUX    ANUBIS_IOADDR(0x01300000)

#endif /* __ASM_ARCH_ANUBISMAP_H */
