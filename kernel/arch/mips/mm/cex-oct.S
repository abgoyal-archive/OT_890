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
 * Copyright (C) 2006 Cavium Networks
 * Cache error handler
 */

#include <asm/asm.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include <asm/stackframe.h>

/*
 * Handle cache error. Indicate to the second level handler whether
 * the exception is recoverable.
 */
	LEAF(except_vec2_octeon)

	.set    push
	.set	mips64r2
	.set	noreorder
	.set	noat


	/* due to an errata we need to read the COP0 CacheErr (Dcache)
	 * before any cache/DRAM access	 */

	rdhwr   k0, $0        /* get core_id */
	PTR_LA  k1, cache_err_dcache
	sll     k0, k0, 3
	PTR_ADDU k1, k0, k1    /* k1 = &cache_err_dcache[core_id] */

	dmfc0   k0, CP0_CACHEERR, 1
	sd      k0, (k1)
	dmtc0   $0, CP0_CACHEERR, 1

        /* check whether this is a nested exception */
	mfc0    k1, CP0_STATUS
	andi    k1, k1, ST0_EXL
	beqz    k1, 1f
	 nop
	j	cache_parity_error_octeon_non_recoverable
	 nop

	/* exception is recoverable */
1:	j	handle_cache_err
	 nop

	.set    pop
	END(except_vec2_octeon)

 /* We need to jump to handle_cache_err so that the previous handler
  * can fit within 0x80 bytes. We also move from 0xFFFFFFFFAXXXXXXX
  * space (uncached) to the 0xFFFFFFFF8XXXXXXX space (cached).  */
	LEAF(handle_cache_err)
	.set    push
        .set    noreorder
        .set    noat

	SAVE_ALL
	KMODE
	jal     cache_parity_error_octeon_recoverable
	nop
	j       ret_from_exception
	nop

	.set pop
	END(handle_cache_err)
