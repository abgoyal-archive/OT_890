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
 * sgi.h: Definitions specific to SGI machines.
 *
 * Copyright (C) 1996 David S. Miller (dm@sgi.com)
 */
#ifndef _ASM_SGI_SGI_H
#define _ASM_SGI_SGI_H

/* UP=UniProcessor MP=MultiProcessor(capable) */
enum sgi_mach {
	ip4,	/* R2k UP */
	ip5,	/* R2k MP */
	ip6,	/* R3k UP */
	ip7,	/* R3k MP */
	ip9,	/* R3k UP */
	ip12,	/* R3kA UP, Indigo */
	ip15,	/* R3kA MP */
	ip17,	/* R4K UP */
	ip19,	/* R4K MP */
	ip20,	/* R4K UP, Indigo */
	ip21,	/* TFP MP */
	ip22,	/* R4x00 UP, Indigo2 */
	ip25,	/* R10k MP */
	ip26,	/* TFP UP, Indigo2 */
	ip27,	/* R10k MP, R12k MP, Origin */
	ip28,	/* R10k UP, Indigo2 */
	ip30,	/* Octane */
	ip32,	/* O2 */
};

extern enum sgi_mach sgimach;
extern void sgi_sysinit(void);

/* Many I/O space registers are byte sized and are contained within
 * one byte per word, specifically the MSB, this macro helps out.
 */
#ifdef __MIPSEL__
#define SGI_MSB(regaddr)   (regaddr)
#else
#define SGI_MSB(regaddr)   ((regaddr) | 0x3)
#endif

#endif /* _ASM_SGI_SGI_H */
