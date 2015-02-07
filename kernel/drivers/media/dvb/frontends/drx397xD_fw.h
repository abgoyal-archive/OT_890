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
 * Firmware definitions for Micronas drx397xD
 *
 * Copyright (C) 2007 Henk Vergonet <Henk.Vergonet@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef _FW_ENTRY
	_FW_ENTRY("drx397xD.A2.fw",	DRXD_FW_A2 = 0,	DRXD_FW_A2	),
	_FW_ENTRY("drx397xD.B1.fw",	DRXD_FW_B1,	DRXD_FW_B1	),
#undef _FW_ENTRY
#endif /* _FW_ENTRY */

#ifdef _BLOB_ENTRY
	_BLOB_ENTRY("InitAtomicRead",	DRXD_InitAtomicRead = 0	),
	_BLOB_ENTRY("InitCE",		DRXD_InitCE		),
	_BLOB_ENTRY("InitCP",		DRXD_InitCP		),
	_BLOB_ENTRY("InitEC",		DRXD_InitEC		),
	_BLOB_ENTRY("InitEQ",		DRXD_InitEQ		),
	_BLOB_ENTRY("InitFE_1",		DRXD_InitFE_1		),
	_BLOB_ENTRY("InitFE_2",		DRXD_InitFE_2		),
	_BLOB_ENTRY("InitFT",		DRXD_InitFT		),
	_BLOB_ENTRY("InitSC",		DRXD_InitSC		),
	_BLOB_ENTRY("ResetCEFR",	DRXD_ResetCEFR		),
	_BLOB_ENTRY("ResetECRAM",	DRXD_ResetECRAM		),
	_BLOB_ENTRY("microcode",	DRXD_microcode		),
#undef _BLOB_ENTRY
#endif /* _BLOB_ENTRY */
