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

#ifndef __ASM_SH_SH7763RDP_H
#define __ASM_SH_SH7763RDP_H

/*
 * linux/include/asm-sh/sh7763drp.h
 *
 * Copyright (C) 2008 Renesas Solutions
 * Copyright (C) 2008 Nobuhiro Iwamatsu <iwamatsu.nobuhiro@renesas.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 */
#include <asm/addrspace.h>

/* clock control */
#define MSTPCR1 0xFFC80038

/* PORT */
#define PORT_PSEL0	0xFFEF0070
#define PORT_PSEL1	0xFFEF0072
#define PORT_PSEL2	0xFFEF0074
#define PORT_PSEL3	0xFFEF0076
#define PORT_PSEL4	0xFFEF0078

#define PORT_PACR	0xFFEF0000
#define PORT_PCCR	0xFFEF0004
#define PORT_PFCR	0xFFEF000A
#define PORT_PGCR	0xFFEF000C
#define PORT_PHCR	0xFFEF000E
#define PORT_PICR	0xFFEF0010
#define PORT_PJCR	0xFFEF0012
#define PORT_PKCR	0xFFEF0014
#define PORT_PLCR	0xFFEF0016
#define PORT_PMCR	0xFFEF0018
#define PORT_PNCR	0xFFEF001A

/* FPGA */
#define CPLD_BOARD_ID_ERV_REG	0xB1000000
#define CPLD_CPLD_CMD_REG		0xB1000006

/*
 * USB SH7763RDP board can use Host only.
 */
#define USB_USBHSC	0xFFEC80f0

/* arch/sh/boards/renesas/sh7763rdp/irq.c */
void init_sh7763rdp_IRQ(void);
int sh7763rdp_irq_demux(int irq);
#define __IO_PREFIX	sh7763rdp
#include <asm/io_generic.h>

#endif /* __ASM_SH_SH7763RDP_H */
