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

/*****************************************************************************
 *                                                                           *
 * File: elmer0.h                                                            *
 * $Revision$                                                          *
 * $Date$                                              *
 * Description:                                                              *
 *  part of the Chelsio 10Gb Ethernet Driver.                                *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License, version 2, as       *
 * published by the Free Software Foundation.                                *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with this program; if not, write to the Free Software Foundation, Inc.,   *
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED    *
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF      *
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.                     *
 *                                                                           *
 * http://www.chelsio.com                                                    *
 *                                                                           *
 * Copyright (c) 2003 - 2005 Chelsio Communications, Inc.                    *
 * All rights reserved.                                                      *
 *                                                                           *
 * Maintainers: maintainers@chelsio.com                                      *
 *                                                                           *
 * Authors: Dimitrios Michailidis   <dm@chelsio.com>                         *
 *          Tina Yang               <tainay@chelsio.com>                     *
 *          Felix Marti             <felix@chelsio.com>                      *
 *          Scott Bardone           <sbardone@chelsio.com>                   *
 *          Kurt Ottaway            <kottaway@chelsio.com>                   *
 *          Frank DiMambro          <frank@chelsio.com>                      *
 *                                                                           *
 * History:                                                                  *
 *                                                                           *
 ****************************************************************************/

#ifndef _CXGB_ELMER0_H_
#define _CXGB_ELMER0_H_

/* ELMER0 flavors */
enum {
	ELMER0_XC2S300E_6FT256_C,
	ELMER0_XC2S100E_6TQ144_C
};

/* ELMER0 registers */
#define A_ELMER0_VERSION	0x100000
#define A_ELMER0_PHY_CFG	0x100004
#define A_ELMER0_INT_ENABLE	0x100008
#define A_ELMER0_INT_CAUSE	0x10000c
#define A_ELMER0_GPI_CFG	0x100010
#define A_ELMER0_GPI_STAT	0x100014
#define A_ELMER0_GPO		0x100018
#define A_ELMER0_PORT0_MI1_CFG	0x400000

#define S_MI1_MDI_ENABLE    0
#define V_MI1_MDI_ENABLE(x) ((x) << S_MI1_MDI_ENABLE)
#define F_MI1_MDI_ENABLE    V_MI1_MDI_ENABLE(1U)

#define S_MI1_MDI_INVERT    1
#define V_MI1_MDI_INVERT(x) ((x) << S_MI1_MDI_INVERT)
#define F_MI1_MDI_INVERT    V_MI1_MDI_INVERT(1U)

#define S_MI1_PREAMBLE_ENABLE    2
#define V_MI1_PREAMBLE_ENABLE(x) ((x) << S_MI1_PREAMBLE_ENABLE)
#define F_MI1_PREAMBLE_ENABLE    V_MI1_PREAMBLE_ENABLE(1U)

#define S_MI1_SOF    3
#define M_MI1_SOF    0x3
#define V_MI1_SOF(x) ((x) << S_MI1_SOF)
#define G_MI1_SOF(x) (((x) >> S_MI1_SOF) & M_MI1_SOF)

#define S_MI1_CLK_DIV    5
#define M_MI1_CLK_DIV    0xff
#define V_MI1_CLK_DIV(x) ((x) << S_MI1_CLK_DIV)
#define G_MI1_CLK_DIV(x) (((x) >> S_MI1_CLK_DIV) & M_MI1_CLK_DIV)

#define A_ELMER0_PORT0_MI1_ADDR 0x400004

#define S_MI1_REG_ADDR    0
#define M_MI1_REG_ADDR    0x1f
#define V_MI1_REG_ADDR(x) ((x) << S_MI1_REG_ADDR)
#define G_MI1_REG_ADDR(x) (((x) >> S_MI1_REG_ADDR) & M_MI1_REG_ADDR)

#define S_MI1_PHY_ADDR    5
#define M_MI1_PHY_ADDR    0x1f
#define V_MI1_PHY_ADDR(x) ((x) << S_MI1_PHY_ADDR)
#define G_MI1_PHY_ADDR(x) (((x) >> S_MI1_PHY_ADDR) & M_MI1_PHY_ADDR)

#define A_ELMER0_PORT0_MI1_DATA 0x400008

#define S_MI1_DATA    0
#define M_MI1_DATA    0xffff
#define V_MI1_DATA(x) ((x) << S_MI1_DATA)
#define G_MI1_DATA(x) (((x) >> S_MI1_DATA) & M_MI1_DATA)

#define A_ELMER0_PORT0_MI1_OP 0x40000c

#define S_MI1_OP    0
#define M_MI1_OP    0x3
#define V_MI1_OP(x) ((x) << S_MI1_OP)
#define G_MI1_OP(x) (((x) >> S_MI1_OP) & M_MI1_OP)

#define S_MI1_ADDR_AUTOINC    2
#define V_MI1_ADDR_AUTOINC(x) ((x) << S_MI1_ADDR_AUTOINC)
#define F_MI1_ADDR_AUTOINC    V_MI1_ADDR_AUTOINC(1U)

#define S_MI1_OP_BUSY    31
#define V_MI1_OP_BUSY(x) ((x) << S_MI1_OP_BUSY)
#define F_MI1_OP_BUSY    V_MI1_OP_BUSY(1U)

#define A_ELMER0_PORT1_MI1_CFG	0x500000
#define A_ELMER0_PORT1_MI1_ADDR	0x500004
#define A_ELMER0_PORT1_MI1_DATA	0x500008
#define A_ELMER0_PORT1_MI1_OP	0x50000c
#define A_ELMER0_PORT2_MI1_CFG	0x600000
#define A_ELMER0_PORT2_MI1_ADDR	0x600004
#define A_ELMER0_PORT2_MI1_DATA	0x600008
#define A_ELMER0_PORT2_MI1_OP	0x60000c
#define A_ELMER0_PORT3_MI1_CFG	0x700000
#define A_ELMER0_PORT3_MI1_ADDR	0x700004
#define A_ELMER0_PORT3_MI1_DATA	0x700008
#define A_ELMER0_PORT3_MI1_OP	0x70000c

/* Simple bit definition for GPI and GP0 registers. */
#define     ELMER0_GP_BIT0              0x0001
#define     ELMER0_GP_BIT1              0x0002
#define     ELMER0_GP_BIT2              0x0004
#define     ELMER0_GP_BIT3              0x0008
#define     ELMER0_GP_BIT4              0x0010
#define     ELMER0_GP_BIT5              0x0020
#define     ELMER0_GP_BIT6              0x0040
#define     ELMER0_GP_BIT7              0x0080
#define     ELMER0_GP_BIT8              0x0100
#define     ELMER0_GP_BIT9              0x0200
#define     ELMER0_GP_BIT10             0x0400
#define     ELMER0_GP_BIT11             0x0800
#define     ELMER0_GP_BIT12             0x1000
#define     ELMER0_GP_BIT13             0x2000
#define     ELMER0_GP_BIT14             0x4000
#define     ELMER0_GP_BIT15             0x8000
#define     ELMER0_GP_BIT16             0x10000
#define     ELMER0_GP_BIT17             0x20000
#define     ELMER0_GP_BIT18             0x40000
#define     ELMER0_GP_BIT19             0x80000

#define MI1_OP_DIRECT_WRITE 1
#define MI1_OP_DIRECT_READ  2

#define MI1_OP_INDIRECT_ADDRESS  0
#define MI1_OP_INDIRECT_WRITE    1
#define MI1_OP_INDIRECT_READ_INC 2
#define MI1_OP_INDIRECT_READ     3

#endif /* _CXGB_ELMER0_H_ */

