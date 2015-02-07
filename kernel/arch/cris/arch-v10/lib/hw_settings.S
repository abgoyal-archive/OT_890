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
 * $Id: hw_settings.S,v 1.1 2001/12/17 13:59:27 bjornw Exp $
 * 
 * This table is used by some tools to extract hardware parameters.
 * The table should be included in the kernel and the decompressor.
 * Don't forget to update the tools if you change this table.
 *
 * Copyright (C) 2001 Axis Communications AB
 *
 * Authors:  Mikael Starvik (starvik@axis.com)	
 */

#define PA_SET_VALUE ((CONFIG_ETRAX_DEF_R_PORT_PA_DIR << 8) | \
		(CONFIG_ETRAX_DEF_R_PORT_PA_DATA))
#define PB_SET_VALUE ((CONFIG_ETRAX_DEF_R_PORT_PB_CONFIG << 16) | \
		(CONFIG_ETRAX_DEF_R_PORT_PB_DIR << 8) | \
		(CONFIG_ETRAX_DEF_R_PORT_PB_DATA))
	
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

	; SDRAM or EDO DRAM?
#ifdef CONFIG_ETRAX_SDRAM
	.dword 1
#else
	.dword 0
#endif

	; Register values 
	.dword R_WAITSTATES
	.dword CONFIG_ETRAX_DEF_R_WAITSTATES
	.dword R_BUS_CONFIG
	.dword CONFIG_ETRAX_DEF_R_BUS_CONFIG
#ifdef CONFIG_ETRAX_SDRAM
	.dword R_SDRAM_CONFIG
	.dword CONFIG_ETRAX_DEF_R_SDRAM_CONFIG
	.dword R_SDRAM_TIMING
	.dword CONFIG_ETRAX_DEF_R_SDRAM_TIMING
#else
	.dword R_DRAM_CONFIG
	.dword CONFIG_ETRAX_DEF_R_DRAM_CONFIG
	.dword R_DRAM_TIMING
	.dword CONFIG_ETRAX_DEF_R_DRAM_TIMING
#endif
	.dword R_PORT_PA_SET
	.dword PA_SET_VALUE 
	.dword R_PORT_PB_SET
	.dword PB_SET_VALUE
	.dword 0 ; No more register values
