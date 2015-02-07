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
 * Linux/PA-RISC Project (http://www.parisc-linux.org/)
 *
 * Floating-point emulation code
 *  Copyright (C) 2001 Hewlett-Packard (Paul Bame) <bame@debian.org>
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/*
 * BEGIN_DESC
 * 
 *  File: 
 *      @(#)	pa/fp/fpu.h		$Revision$
 * 
 *  Purpose:
 *      <<please update with a synopis of the functionality provided by this file>>
 * 
 * 
 * END_DESC  
*/

#ifdef __NO_PA_HDRS
    PA header file -- do not include this header file for non-PA builds.
#endif


#ifndef _MACHINE_FPU_INCLUDED /* allows multiple inclusion */
#define _MACHINE_FPU_INCLUDED

#if 0
#ifndef _SYS_STDSYMS_INCLUDED
#    include <sys/stdsyms.h>
#endif   /* _SYS_STDSYMS_INCLUDED  */
#include  <machine/pdc/pdc_rqsts.h>
#endif

#define PA83_FPU_FLAG    0x00000001
#define PA89_FPU_FLAG    0x00000002
#define PA2_0_FPU_FLAG   0x00000010

#define TIMEX_EXTEN_FLAG 0x00000004

#define ROLEX_EXTEN_FLAG 0x00000008
#define COPR_FP 	0x00000080	/* Floating point -- Coprocessor 0 */
#define SFU_MPY_DIVIDE	0x00008000	/* Multiply/Divide __ SFU 0 */


#define EM_FPU_TYPE_OFFSET 272

/* version of EMULATION software for COPR,0,0 instruction */
#define EMULATION_VERSION 4

/*
 * The only was to differeniate between TIMEX and ROLEX (or PCX-S and PCX-T)
 * is thorough the potential type field from the PDC_MODEL call.  The 
 * following flags are used at assist this differeniation.
 */

#define ROLEX_POTENTIAL_KEY_FLAGS	PDC_MODEL_CPU_KEY_WORD_TO_IO
#define TIMEX_POTENTIAL_KEY_FLAGS	(PDC_MODEL_CPU_KEY_QUAD_STORE | \
					 PDC_MODEL_CPU_KEY_RECIP_SQRT)


#endif /* ! _MACHINE_FPU_INCLUDED */
