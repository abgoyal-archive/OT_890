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
 * include/asm-sh/cpu-sh3/ubc.h
 *
 * Copyright (C) 1999 Niibe Yutaka
 * Copyright (C) 2003 Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#ifndef __ASM_CPU_SH3_UBC_H
#define __ASM_CPU_SH3_UBC_H

#if defined(CONFIG_CPU_SUBTYPE_SH7710) || \
    defined(CONFIG_CPU_SUBTYPE_SH7720) || \
    defined(CONFIG_CPU_SUBTYPE_SH7721)
#define UBC_BARA		0xa4ffffb0
#define UBC_BAMRA		0xa4ffffb4
#define UBC_BBRA		0xa4ffffb8
#define UBC_BASRA		0xffffffe4
#define UBC_BARB		0xa4ffffa0
#define UBC_BAMRB		0xa4ffffa4
#define UBC_BBRB		0xa4ffffa8
#define UBC_BASRB		0xffffffe8
#define UBC_BDRB		0xa4ffff90
#define UBC_BDMRB		0xa4ffff94
#define UBC_BRCR		0xa4ffff98
#else
#define UBC_BARA                0xffffffb0
#define UBC_BAMRA               0xffffffb4
#define UBC_BBRA                0xffffffb8
#define UBC_BASRA               0xffffffe4
#define UBC_BARB                0xffffffa0
#define UBC_BAMRB               0xffffffa4
#define UBC_BBRB                0xffffffa8
#define UBC_BASRB               0xffffffe8
#define UBC_BDRB                0xffffff90
#define UBC_BDMRB               0xffffff94
#define UBC_BRCR                0xffffff98
#endif

#endif /* __ASM_CPU_SH3_UBC_H */
