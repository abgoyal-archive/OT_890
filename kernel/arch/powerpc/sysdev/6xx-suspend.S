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
 * Enter and leave sleep state on chips with 6xx-style HID0
 * power management bits, which don't leave sleep state via reset.
 *
 * Author: Scott Wood <scottwood@freescale.com>
 *
 * Copyright (c) 2006-2007 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 */

#include <asm/ppc_asm.h>
#include <asm/reg.h>
#include <asm/thread_info.h>
#include <asm/asm-offsets.h>

_GLOBAL(mpc6xx_enter_standby)
	mflr	r4

	mfspr	r5, SPRN_HID0
	rlwinm	r5, r5, 0, ~(HID0_DOZE | HID0_NAP)
	oris	r5, r5, HID0_SLEEP@h
	mtspr	SPRN_HID0, r5
	isync

	lis	r5, ret_from_standby@h
	ori	r5, r5, ret_from_standby@l
	mtlr	r5

	rlwinm	r5, r1, 0, 0, 31-THREAD_SHIFT
	lwz	r6, TI_LOCAL_FLAGS(r5)
	ori	r6, r6, _TLF_SLEEPING
	stw	r6, TI_LOCAL_FLAGS(r5)

	mfmsr	r5
	ori	r5, r5, MSR_EE
	oris	r5, r5, MSR_POW@h
	sync
	mtmsr	r5
	isync

1:	b	1b

ret_from_standby:
	mfspr	r5, SPRN_HID0
	rlwinm	r5, r5, 0, ~HID0_SLEEP
	mtspr	SPRN_HID0, r5

	mtlr	r4
	blr
