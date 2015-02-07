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

/* n2-asm.S: Niagara2 RNG hypervisor call assembler.
 *
 * Copyright (C) 2008 David S. Miller <davem@davemloft.net>
 */
#include <linux/linkage.h>
#include <asm/hypervisor.h>
#include "n2rng.h"

	.text

ENTRY(sun4v_rng_get_diag_ctl)
	mov	HV_FAST_RNG_GET_DIAG_CTL, %o5
	ta	HV_FAST_TRAP
	retl
	 nop
ENDPROC(sun4v_rng_get_diag_ctl)

ENTRY(sun4v_rng_ctl_read_v1)
	mov	%o1, %o3
	mov	%o2, %o4
	mov	HV_FAST_RNG_CTL_READ, %o5
	ta	HV_FAST_TRAP
	stx	%o1, [%o3]
	retl
	 stx	%o2, [%o4]
ENDPROC(sun4v_rng_ctl_read_v1)

ENTRY(sun4v_rng_ctl_read_v2)
	save	%sp, -192, %sp
	mov	%i0, %o0
	mov	%i1, %o1
	mov	HV_FAST_RNG_CTL_READ, %o5
	ta	HV_FAST_TRAP
	stx	%o1, [%i2]
	stx	%o2, [%i3]
	stx	%o3, [%i4]
	stx	%o4, [%i5]
	ret
	restore	%g0, %o0, %o0
ENDPROC(sun4v_rng_ctl_read_v2)

ENTRY(sun4v_rng_ctl_write_v1)
	mov	%o3, %o4
	mov	HV_FAST_RNG_CTL_WRITE, %o5
	ta	HV_FAST_TRAP
	retl
	 stx	%o1, [%o4]
ENDPROC(sun4v_rng_ctl_write_v1)

ENTRY(sun4v_rng_ctl_write_v2)
	mov	HV_FAST_RNG_CTL_WRITE, %o5
	ta	HV_FAST_TRAP
	retl
	 nop
ENDPROC(sun4v_rng_ctl_write_v2)

ENTRY(sun4v_rng_data_read_diag_v1)
	mov	%o2, %o4
	mov	HV_FAST_RNG_DATA_READ_DIAG, %o5
	ta	HV_FAST_TRAP
	retl
	 stx	%o1, [%o4]
ENDPROC(sun4v_rng_data_read_diag_v1)

ENTRY(sun4v_rng_data_read_diag_v2)
	mov	%o3, %o4
	mov	HV_FAST_RNG_DATA_READ_DIAG, %o5
	ta	HV_FAST_TRAP
	retl
	 stx	%o1, [%o4]
ENDPROC(sun4v_rng_data_read_diag_v2)

ENTRY(sun4v_rng_data_read)
	mov	%o1, %o4
	mov	HV_FAST_RNG_DATA_READ, %o5
	ta	HV_FAST_TRAP
	retl
	 stx	%o1, [%o4]
ENDPROC(sun4v_rng_data_read)
