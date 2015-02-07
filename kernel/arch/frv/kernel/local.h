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

/* local.h: local definitions
 *
 * Copyright (C) 2004 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#ifndef _FRV_LOCAL_H
#define _FRV_LOCAL_H

#include <asm/sections.h>

#ifndef __ASSEMBLY__

/* dma.c */
extern unsigned long frv_dma_inprogress;

extern void frv_dma_pause_all(void);
extern void frv_dma_resume_all(void);

/* sleep.S */
extern asmlinkage void frv_cpu_suspend(unsigned long);
extern asmlinkage void frv_cpu_core_sleep(void);

/* setup.c */
extern unsigned long __nongprelbss pdm_suspend_mode;
extern void determine_clocks(int verbose);
extern int __nongprelbss clock_p0_current;
extern int __nongprelbss clock_cm_current;
extern int __nongprelbss clock_cmode_current;

#ifdef CONFIG_PM
extern int __nongprelbss clock_cmodes_permitted;
extern unsigned long __nongprelbss clock_bits_settable;
#define CLOCK_BIT_CM		0x0000000f
#define CLOCK_BIT_CM_H		0x00000001	/* CLKC.CM can be set to 0 */
#define CLOCK_BIT_CM_M		0x00000002	/* CLKC.CM can be set to 1 */
#define CLOCK_BIT_CM_L		0x00000004	/* CLKC.CM can be set to 2 */
#define CLOCK_BIT_P0		0x00000010	/* CLKC.P0 can be changed */
#define CLOCK_BIT_CMODE		0x00000020	/* CLKC.CMODE can be changed */

extern void (*__power_switch_wake_setup)(void);
extern int  (*__power_switch_wake_check)(void);
extern void (*__power_switch_wake_cleanup)(void);
#endif

/* time.c */
extern void time_divisor_init(void);

/* cmode.S */
extern asmlinkage void frv_change_cmode(int);


#endif /* __ASSEMBLY__ */
#endif /* _FRV_LOCAL_H */
