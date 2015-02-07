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
 * This file contains low level CPU setup functions.
 * Valentine Barshak <vbarshak@ru.mvista.com>
 * MontaVista Software, Inc (c) 2007
 *
 * Based on cpu_setup_6xx code by
 * Benjamin Herrenschmidt <benh@kernel.crashing.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 */

#include <asm/processor.h>
#include <asm/cputable.h>
#include <asm/ppc_asm.h>

_GLOBAL(__setup_cpu_440ep)
	b	__init_fpu_44x
_GLOBAL(__setup_cpu_440epx)
	mflr	r4
	bl	__init_fpu_44x
	bl	__plb_disable_wrp
	bl	__fixup_440A_mcheck
	mtlr	r4
	blr
_GLOBAL(__setup_cpu_440grx)
	mflr	r4
	bl	__plb_disable_wrp
	bl	__fixup_440A_mcheck
	mtlr	r4
	blr
_GLOBAL(__setup_cpu_460ex)
_GLOBAL(__setup_cpu_460gt)
	mflr	r4
	bl	__init_fpu_44x
	bl	__fixup_440A_mcheck
	mtlr	r4
	blr

_GLOBAL(__setup_cpu_440x5)
_GLOBAL(__setup_cpu_440gx)
_GLOBAL(__setup_cpu_440spe)
	b	__fixup_440A_mcheck

/* enable APU between CPU and FPU */
_GLOBAL(__init_fpu_44x)
	mfspr	r3,SPRN_CCR0
	/* Clear DAPUIB flag in CCR0 */
	rlwinm	r3,r3,0,12,10
	mtspr	SPRN_CCR0,r3
	isync
	blr

/*
 * Workaround for the incorrect write to DDR SDRAM errata.
 * The write address can be corrupted during writes to
 * DDR SDRAM when write pipelining is enabled on PLB0.
 * Disable write pipelining here.
 */
#define DCRN_PLB4A0_ACR	0x81

_GLOBAL(__plb_disable_wrp)
	mfdcr	r3,DCRN_PLB4A0_ACR
	/* clear WRP bit in PLB4A0_ACR */
	rlwinm	r3,r3,0,8,6
	mtdcr	DCRN_PLB4A0_ACR,r3
	isync
	blr

