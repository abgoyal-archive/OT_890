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
 * include/asm/xor.h
 *
 * High speed xor_block operation for RAID4/5 utilizing the
 * UltraSparc Visual Instruction Set and Niagara block-init
 * twin-load instructions.
 *
 * Copyright (C) 1997, 1999 Jakub Jelinek (jj@ultra.linux.cz)
 * Copyright (C) 2006 David S. Miller <davem@davemloft.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * You should have received a copy of the GNU General Public License
 * (for example /usr/src/linux/COPYING); if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <asm/spitfire.h>

extern void xor_vis_2(unsigned long, unsigned long *, unsigned long *);
extern void xor_vis_3(unsigned long, unsigned long *, unsigned long *,
		      unsigned long *);
extern void xor_vis_4(unsigned long, unsigned long *, unsigned long *,
		      unsigned long *, unsigned long *);
extern void xor_vis_5(unsigned long, unsigned long *, unsigned long *,
		      unsigned long *, unsigned long *, unsigned long *);

/* XXX Ugh, write cheetah versions... -DaveM */

static struct xor_block_template xor_block_VIS = {
        .name	= "VIS",
        .do_2	= xor_vis_2,
        .do_3	= xor_vis_3,
        .do_4	= xor_vis_4,
        .do_5	= xor_vis_5,
};

extern void xor_niagara_2(unsigned long, unsigned long *, unsigned long *);
extern void xor_niagara_3(unsigned long, unsigned long *, unsigned long *,
			  unsigned long *);
extern void xor_niagara_4(unsigned long, unsigned long *, unsigned long *,
			  unsigned long *, unsigned long *);
extern void xor_niagara_5(unsigned long, unsigned long *, unsigned long *,
			  unsigned long *, unsigned long *, unsigned long *);

static struct xor_block_template xor_block_niagara = {
        .name	= "Niagara",
        .do_2	= xor_niagara_2,
        .do_3	= xor_niagara_3,
        .do_4	= xor_niagara_4,
        .do_5	= xor_niagara_5,
};

#undef XOR_TRY_TEMPLATES
#define XOR_TRY_TEMPLATES				\
	do {						\
		xor_speed(&xor_block_VIS);		\
		xor_speed(&xor_block_niagara);		\
	} while (0)

/* For VIS for everything except Niagara.  */
#define XOR_SELECT_TEMPLATE(FASTEST) \
	((tlb_type == hypervisor && \
	  (sun4v_chip_type == SUN4V_CHIP_NIAGARA1 || \
	   sun4v_chip_type == SUN4V_CHIP_NIAGARA2)) ? \
	 &xor_block_niagara : \
	 &xor_block_VIS)
