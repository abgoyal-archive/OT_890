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

#ifndef _I8042_H
#define _I8042_H


/*
 *  Copyright (c) 1999-2002 Vojtech Pavlik
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */

/*
 * Arch-dependent inline functions and defines.
 */

#if defined(CONFIG_MACH_JAZZ)
#include "i8042-jazzio.h"
#elif defined(CONFIG_SGI_HAS_I8042)
#include "i8042-ip22io.h"
#elif defined(CONFIG_SNI_RM)
#include "i8042-snirm.h"
#elif defined(CONFIG_PPC)
#include "i8042-ppcio.h"
#elif defined(CONFIG_SPARC)
#include "i8042-sparcio.h"
#elif defined(CONFIG_X86) || defined(CONFIG_IA64)
#include "i8042-x86ia64io.h"
#else
#include "i8042-io.h"
#endif

/*
 * This is in 50us units, the time we wait for the i8042 to react. This
 * has to be long enough for the i8042 itself to timeout on sending a byte
 * to a non-existent mouse.
 */

#define I8042_CTL_TIMEOUT	10000

/*
 * Status register bits.
 */

#define I8042_STR_PARITY	0x80
#define I8042_STR_TIMEOUT	0x40
#define I8042_STR_AUXDATA	0x20
#define I8042_STR_KEYLOCK	0x10
#define I8042_STR_CMDDAT	0x08
#define I8042_STR_MUXERR	0x04
#define I8042_STR_IBF		0x02
#define	I8042_STR_OBF		0x01

/*
 * Control register bits.
 */

#define I8042_CTR_KBDINT	0x01
#define I8042_CTR_AUXINT	0x02
#define I8042_CTR_IGNKEYLOCK	0x08
#define I8042_CTR_KBDDIS	0x10
#define I8042_CTR_AUXDIS	0x20
#define I8042_CTR_XLATE		0x40

/*
 * Return codes.
 */

#define I8042_RET_CTL_TEST	0x55

/*
 * Expected maximum internal i8042 buffer size. This is used for flushing
 * the i8042 buffers.
 */

#define I8042_BUFFER_SIZE	16

/*
 * Number of AUX ports on controllers supporting active multiplexing
 * specification
 */

#define I8042_NUM_MUX_PORTS	4

/*
 * Debug.
 */

#ifdef DEBUG
static unsigned long i8042_start_time;
#define dbg_init() do { i8042_start_time = jiffies; } while (0)
#define dbg(format, arg...) 							\
	do { 									\
		if (i8042_debug)						\
			printk(KERN_DEBUG __FILE__ ": " format " [%d]\n" ,	\
	 			## arg, (int) (jiffies - i8042_start_time));	\
	} while (0)
#else
#define dbg_init() do { } while (0)
#define dbg(format, arg...) do {} while (0)
#endif

#endif /* _I8042_H */
