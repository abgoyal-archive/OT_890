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
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 2000 MIPS Technologies, Inc.  All rights reserved.
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * Register definitions for Intel PIIX4 South Bridge Device.
 */
#ifndef __ASM_MIPS_BOARDS_PIIX4_H
#define __ASM_MIPS_BOARDS_PIIX4_H

/************************************************************************
 *  IO register offsets
 ************************************************************************/
#define PIIX4_ICTLR1_ICW1	0x20
#define PIIX4_ICTLR1_ICW2	0x21
#define PIIX4_ICTLR1_ICW3	0x21
#define PIIX4_ICTLR1_ICW4	0x21
#define PIIX4_ICTLR2_ICW1	0xa0
#define PIIX4_ICTLR2_ICW2	0xa1
#define PIIX4_ICTLR2_ICW3	0xa1
#define PIIX4_ICTLR2_ICW4	0xa1
#define PIIX4_ICTLR1_OCW1	0x21
#define PIIX4_ICTLR1_OCW2	0x20
#define PIIX4_ICTLR1_OCW3	0x20
#define PIIX4_ICTLR1_OCW4	0x20
#define PIIX4_ICTLR2_OCW1	0xa1
#define PIIX4_ICTLR2_OCW2	0xa0
#define PIIX4_ICTLR2_OCW3	0xa0
#define PIIX4_ICTLR2_OCW4	0xa0


/************************************************************************
 *  Register encodings.
 ************************************************************************/
#define PIIX4_OCW2_NSEOI	(0x1 << 5)
#define PIIX4_OCW2_SEOI		(0x3 << 5)
#define PIIX4_OCW2_RNSEOI	(0x5 << 5)
#define PIIX4_OCW2_RAEOIS	(0x4 << 5)
#define PIIX4_OCW2_RAEOIC	(0x0 << 5)
#define PIIX4_OCW2_RSEOI	(0x7 << 5)
#define PIIX4_OCW2_SP		(0x6 << 5)
#define PIIX4_OCW2_NOP		(0x2 << 5)

#define PIIX4_OCW2_SEL          (0x0 << 3)

#define PIIX4_OCW2_ILS_0	0
#define PIIX4_OCW2_ILS_1	1
#define PIIX4_OCW2_ILS_2	2
#define PIIX4_OCW2_ILS_3	3
#define PIIX4_OCW2_ILS_4	4
#define PIIX4_OCW2_ILS_5	5
#define PIIX4_OCW2_ILS_6	6
#define PIIX4_OCW2_ILS_7	7
#define PIIX4_OCW2_ILS_8	0
#define PIIX4_OCW2_ILS_9	1
#define PIIX4_OCW2_ILS_10	2
#define PIIX4_OCW2_ILS_11	3
#define PIIX4_OCW2_ILS_12	4
#define PIIX4_OCW2_ILS_13	5
#define PIIX4_OCW2_ILS_14	6
#define PIIX4_OCW2_ILS_15	7

#define PIIX4_OCW3_SEL          (0x1 << 3)

#define PIIX4_OCW3_IRR          0x2
#define PIIX4_OCW3_ISR          0x3

#endif /* __ASM_MIPS_BOARDS_PIIX4_H */
