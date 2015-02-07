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
 *  Copyright (C) 2005-2007 Jiri Slaby <jirislaby@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */

#ifndef __PHANTOM_H
#define __PHANTOM_H

#include <linux/types.h>

/* PHN_(G/S)ET_REG param */
struct phm_reg {
	__u32 reg;
	__u32 value;
};

/* PHN_(G/S)ET_REGS param */
struct phm_regs {
	__u32 count;
	__u32 mask;
	__u32 values[8];
};

#define PH_IOC_MAGIC		'p'
#define PHN_GET_REG		_IOWR(PH_IOC_MAGIC, 0, struct phm_reg *)
#define PHN_SET_REG		_IOW(PH_IOC_MAGIC, 1, struct phm_reg *)
#define PHN_GET_REGS		_IOWR(PH_IOC_MAGIC, 2, struct phm_regs *)
#define PHN_SET_REGS		_IOW(PH_IOC_MAGIC, 3, struct phm_regs *)
/* this ioctl tells the driver, that the caller is not OpenHaptics and might
 * use improved registers update (no more phantom switchoffs when using
 * libphantom) */
#define PHN_NOT_OH		_IO(PH_IOC_MAGIC, 4)
#define PHN_GETREG		_IOWR(PH_IOC_MAGIC, 5, struct phm_reg)
#define PHN_SETREG		_IOW(PH_IOC_MAGIC, 6, struct phm_reg)
#define PHN_GETREGS		_IOWR(PH_IOC_MAGIC, 7, struct phm_regs)
#define PHN_SETREGS		_IOW(PH_IOC_MAGIC, 8, struct phm_regs)

#define PHN_CONTROL		0x6     /* control byte in iaddr space */
#define PHN_CTL_AMP		0x1     /*   switch after torques change */
#define PHN_CTL_BUT		0x2     /*   is button switched */
#define PHN_CTL_IRQ		0x10    /*   is irq enabled */

#define PHN_ZERO_FORCE		2048	/* zero torque on motor */

#endif
