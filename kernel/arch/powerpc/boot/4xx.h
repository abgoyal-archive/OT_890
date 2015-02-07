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
 * PowerPC 4xx related functions
 *
 * Copyright 2007 IBM Corporation.
 * Josh Boyer <jwboyer@linux.vnet.ibm.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#ifndef _POWERPC_BOOT_4XX_H_
#define _POWERPC_BOOT_4XX_H_

void ibm4xx_sdram_fixup_memsize(void);
void ibm440spe_fixup_memsize(void);
void ibm4xx_denali_fixup_memsize(void);
void ibm44x_dbcr_reset(void);
void ibm40x_dbcr_reset(void);
void ibm4xx_quiesce_eth(u32 *emac0, u32 *emac1);
void ibm4xx_fixup_ebc_ranges(const char *ebc);

void ibm405gp_fixup_clocks(unsigned int sys_clk, unsigned int ser_clk);
void ibm405ep_fixup_clocks(unsigned int sys_clk);
void ibm440gp_fixup_clocks(unsigned int sys_clk, unsigned int ser_clk);
void ibm440ep_fixup_clocks(unsigned int sys_clk, unsigned int ser_clk,
			   unsigned int tmr_clk);
void ibm440gx_fixup_clocks(unsigned int sys_clk, unsigned int ser_clk,
			   unsigned int tmr_clk);
void ibm440spe_fixup_clocks(unsigned int sys_clk, unsigned int ser_clk,
			    unsigned int tmr_clk);

#endif /* _POWERPC_BOOT_4XX_H_ */
