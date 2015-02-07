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
 * Defitions for the address spaces of the AVR32 CPUs. Heavily based on
 * include/asm-sh/addrspace.h
 *
 * Copyright (C) 2004-2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __ASM_AVR32_ADDRSPACE_H
#define __ASM_AVR32_ADDRSPACE_H

#ifdef CONFIG_MMU

/* Memory segments when segmentation is enabled */
#define P0SEG		0x00000000
#define P1SEG		0x80000000
#define P2SEG		0xa0000000
#define P3SEG		0xc0000000
#define P4SEG		0xe0000000

/* Returns the privileged segment base of a given address */
#define PXSEG(a)	(((unsigned long)(a)) & 0xe0000000)

/* Returns the physical address of a PnSEG (n=1,2) address */
#define PHYSADDR(a)	(((unsigned long)(a)) & 0x1fffffff)

/*
 * Map an address to a certain privileged segment
 */
#define P1SEGADDR(a) ((__typeof__(a))(((unsigned long)(a) & 0x1fffffff) \
				      | P1SEG))
#define P2SEGADDR(a) ((__typeof__(a))(((unsigned long)(a) & 0x1fffffff) \
				      | P2SEG))
#define P3SEGADDR(a) ((__typeof__(a))(((unsigned long)(a) & 0x1fffffff) \
				      | P3SEG))
#define P4SEGADDR(a) ((__typeof__(a))(((unsigned long)(a) & 0x1fffffff) \
				      | P4SEG))

#endif /* CONFIG_MMU */

#endif /* __ASM_AVR32_ADDRSPACE_H */
