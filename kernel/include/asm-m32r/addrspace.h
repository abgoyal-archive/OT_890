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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 by Hiroyuki Kondo
 *
 * Defitions for the address spaces of the M32R CPUs.
 */
#ifndef __ASM_M32R_ADDRSPACE_H
#define __ASM_M32R_ADDRSPACE_H

/*
 * Memory segments (32bit kernel mode addresses)
 */
#define KUSEG                   0x00000000
#define KSEG0                   0x80000000
#define KSEG1                   0xa0000000
#define KSEG2                   0xc0000000
#define KSEG3                   0xe0000000

#define K0BASE  KSEG0

/*
 * Returns the kernel segment base of a given address
 */
#ifndef __ASSEMBLY__
#define KSEGX(a)                (((unsigned long)(a)) & 0xe0000000)
#else
#define KSEGX(a)                ((a) & 0xe0000000)
#endif

/*
 * Returns the physical address of a KSEG0/KSEG1 address
 */
#ifndef __ASSEMBLY__
#define PHYSADDR(a)		(((unsigned long)(a)) & 0x1fffffff)
#else
#define PHYSADDR(a)		((a) & 0x1fffffff)
#endif

/*
 * Map an address to a certain kernel segment
 */
#ifndef __ASSEMBLY__
#define KSEG0ADDR(a)		((__typeof__(a))(((unsigned long)(a) & 0x1fffffff) | KSEG0))
#define KSEG1ADDR(a)		((__typeof__(a))(((unsigned long)(a) & 0x1fffffff) | KSEG1))
#define KSEG2ADDR(a)		((__typeof__(a))(((unsigned long)(a) & 0x1fffffff) | KSEG2))
#define KSEG3ADDR(a)		((__typeof__(a))(((unsigned long)(a) & 0x1fffffff) | KSEG3))
#else
#define KSEG0ADDR(a)		(((a) & 0x1fffffff) | KSEG0)
#define KSEG1ADDR(a)		(((a) & 0x1fffffff) | KSEG1)
#define KSEG2ADDR(a)		(((a) & 0x1fffffff) | KSEG2)
#define KSEG3ADDR(a)		(((a) & 0x1fffffff) | KSEG3)
#endif

#endif /* __ASM_M32R_ADDRSPACE_H */
