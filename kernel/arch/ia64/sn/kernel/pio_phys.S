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
 * Copyright (C) 2000-2005 Silicon Graphics, Inc. All rights reserved.
 *
 * This file contains macros used to access MMR registers via
 * uncached physical addresses.
 *      pio_phys_read_mmr  - read an MMR
 *      pio_phys_write_mmr - write an MMR
 *      pio_atomic_phys_write_mmrs - atomically write 1 or 2 MMRs with psr.ic=0
 *              Second MMR will be skipped if address is NULL
 *
 * Addresses passed to these routines should be uncached physical addresses
 * 	ie., 0x80000....
 */



#include <asm/asmmacro.h>
#include <asm/page.h>

GLOBAL_ENTRY(pio_phys_read_mmr)
	.prologue
	.regstk 1,0,0,0
	.body
	mov r2=psr
	rsm psr.i | psr.dt
	;;
	srlz.d
	ld8.acq r8=[r32]
	;;
	mov psr.l=r2;;
	srlz.d
	br.ret.sptk.many rp
END(pio_phys_read_mmr)

GLOBAL_ENTRY(pio_phys_write_mmr)
	.prologue
	.regstk 2,0,0,0
	.body
	mov r2=psr
	rsm psr.i | psr.dt
	;;
	srlz.d
	st8.rel [r32]=r33
	;;
	mov psr.l=r2;;
	srlz.d
	br.ret.sptk.many rp
END(pio_phys_write_mmr)

GLOBAL_ENTRY(pio_atomic_phys_write_mmrs)
	.prologue
	.regstk 4,0,0,0
	.body
	mov r2=psr
	cmp.ne p9,p0=r34,r0;
	rsm psr.i | psr.dt | psr.ic
	;;
	srlz.d
	st8.rel [r32]=r33
(p9)	st8.rel [r34]=r35
	;;
	mov psr.l=r2;;
	srlz.d
	br.ret.sptk.many rp
END(pio_atomic_phys_write_mmrs)


