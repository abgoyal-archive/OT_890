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
 * This table is used by some tools to extract hardware parameters.
 * The table should be included in the kernel and the decompressor.
 * Don't forget to update the tools if you change this table.
 *
 * Copyright (C) 2001-2007 Axis Communications AB
 *
 * Authors:  Mikael Starvik <starvik@axis.com>
 */

#include <hwregs/asm/reg_map_asm.h>
#include <hwregs/asm/ddr2_defs_asm.h>
#include <hwregs/asm/gio_defs_asm.h>

	.ascii "HW_PARAM_MAGIC" ; Magic number
	.dword 0xc0004000	; Kernel start address

	; Debug port
#ifdef CONFIG_ETRAX_DEBUG_PORT0
	.dword 0
#elif defined(CONFIG_ETRAX_DEBUG_PORT1)
	.dword 1
#elif defined(CONFIG_ETRAX_DEBUG_PORT2)
	.dword 2
#elif defined(CONFIG_ETRAX_DEBUG_PORT3)
	.dword 3
#else
	.dword 4 ; No debug
#endif

	; Register values
	.dword REG_ADDR(ddr2, regi_ddr2_ctrl, rw_cfg)
	.dword CONFIG_ETRAX_DDR2_CONFIG
	.dword REG_ADDR(ddr2, regi_ddr2_ctrl, rw_timing)
	.dword CONFIG_ETRAX_DDR2_TIMING
	.dword CONFIG_ETRAX_DDR2_MRS

	.dword REG_ADDR(gio, regi_gio, rw_pa_dout)
	.dword CONFIG_ETRAX_DEF_GIO_PA_OUT
	.dword REG_ADDR(gio, regi_gio, rw_pa_oe)
	.dword CONFIG_ETRAX_DEF_GIO_PA_OE
	.dword REG_ADDR(gio, regi_gio, rw_pb_dout)
	.dword CONFIG_ETRAX_DEF_GIO_PB_OUT
	.dword REG_ADDR(gio, regi_gio, rw_pb_oe)
	.dword CONFIG_ETRAX_DEF_GIO_PB_OE
	.dword REG_ADDR(gio, regi_gio, rw_pc_dout)
	.dword CONFIG_ETRAX_DEF_GIO_PC_OUT
	.dword REG_ADDR(gio, regi_gio, rw_pc_oe)
	.dword CONFIG_ETRAX_DEF_GIO_PC_OE

	.dword 0 ; No more register values
