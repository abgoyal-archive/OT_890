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
 *  arch/arm/include/asm/hardware/ioc.h
 *
 *  Copyright (C) Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Use these macros to read/write the IOC.  All it does is perform the actual
 *  read/write.
 */
#ifndef __ASMARM_HARDWARE_IOC_H
#define __ASMARM_HARDWARE_IOC_H

#ifndef __ASSEMBLY__

/*
 * We use __raw_base variants here so that we give the compiler the
 * chance to keep IOC_BASE in a register.
 */
#define ioc_readb(off)		__raw_readb(IOC_BASE + (off))
#define ioc_writeb(val,off)	__raw_writeb(val, IOC_BASE + (off))

#endif

#define IOC_CONTROL	(0x00)
#define IOC_KARTTX	(0x04)
#define IOC_KARTRX	(0x04)

#define IOC_IRQSTATA	(0x10)
#define IOC_IRQREQA	(0x14)
#define IOC_IRQCLRA	(0x14)
#define IOC_IRQMASKA	(0x18)

#define IOC_IRQSTATB	(0x20)
#define IOC_IRQREQB	(0x24)
#define IOC_IRQMASKB	(0x28)

#define IOC_FIQSTAT	(0x30)
#define IOC_FIQREQ	(0x34)
#define IOC_FIQMASK	(0x38)

#define IOC_T0CNTL	(0x40)
#define IOC_T0LTCHL	(0x40)
#define IOC_T0CNTH	(0x44)
#define IOC_T0LTCHH	(0x44)
#define IOC_T0GO	(0x48)
#define IOC_T0LATCH	(0x4c)

#define IOC_T1CNTL	(0x50)
#define IOC_T1LTCHL	(0x50)
#define IOC_T1CNTH	(0x54)
#define IOC_T1LTCHH	(0x54)
#define IOC_T1GO	(0x58)
#define IOC_T1LATCH	(0x5c)

#define IOC_T2CNTL	(0x60)
#define IOC_T2LTCHL	(0x60)
#define IOC_T2CNTH	(0x64)
#define IOC_T2LTCHH	(0x64)
#define IOC_T2GO	(0x68)
#define IOC_T2LATCH	(0x6c)

#define IOC_T3CNTL	(0x70)
#define IOC_T3LTCHL	(0x70)
#define IOC_T3CNTH	(0x74)
#define IOC_T3LTCHH	(0x74)
#define IOC_T3GO	(0x78)
#define IOC_T3LATCH	(0x7c)

#endif
