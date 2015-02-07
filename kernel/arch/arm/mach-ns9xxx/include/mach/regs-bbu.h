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
 * arch/arm/mach-ns9xxx/include/mach/regs-bbu.h
 *
 * Copyright (C) 2006 by Digi International Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */
#ifndef __ASM_ARCH_REGSBBU_H
#define __ASM_ARCH_REGSBBU_H

#include <mach/hardware.h>

/* BBus Utility */

/* GPIO Configuration Registers block 1 */
/* NOTE: the HRM starts counting at 1 for the GPIO registers, here the start is
 * at 0 for each block.  That is, BBU_GCONFb1(0) is GPIO Configuration Register
 * #1, BBU_GCONFb2(0) is GPIO Configuration Register #8. */
#define BBU_GCONFb1(x)	__REG2(0x90600010, (x))
#define BBU_GCONFb2(x)	__REG2(0x90600100, (x))

#define BBU_GCONFx_DIR(m)	__REGBIT(3 + (((m) & 7) << 2))
#define BBU_GCONFx_DIR_INPUT(m)	__REGVAL(BBU_GCONFx_DIR(m), 0)
#define BBU_GCONFx_DIR_OUTPUT(m)	__REGVAL(BBU_GCONFx_DIR(m), 1)
#define BBU_GCONFx_INV(m)	__REGBIT(2 + (((m) & 7) << 2))
#define BBU_GCONFx_INV_NO(m)		__REGVAL(BBU_GCONFx_INV(m), 0)
#define BBU_GCONFx_INV_YES(m)		__REGVAL(BBU_GCONFx_INV(m), 1)
#define BBU_GCONFx_FUNC(m)	__REGBITS(1 + (((m) & 7) << 2), ((m) & 7) << 2)
#define BBU_GCONFx_FUNC_0(m)		__REGVAL(BBU_GCONFx_FUNC(m), 0)
#define BBU_GCONFx_FUNC_1(m)		__REGVAL(BBU_GCONFx_FUNC(m), 1)
#define BBU_GCONFx_FUNC_2(m)		__REGVAL(BBU_GCONFx_FUNC(m), 2)
#define BBU_GCONFx_FUNC_3(m)		__REGVAL(BBU_GCONFx_FUNC(m), 3)

#define BBU_GCTRL1	__REG(0x90600030)
#define BBU_GCTRL2	__REG(0x90600034)
#define BBU_GCTRL3	__REG(0x90600120)

#define BBU_GSTAT1	__REG(0x90600040)
#define BBU_GSTAT2	__REG(0x90600044)
#define BBU_GSTAT3	__REG(0x90600130)

#endif /* ifndef __ASM_ARCH_REGSBBU_H */
