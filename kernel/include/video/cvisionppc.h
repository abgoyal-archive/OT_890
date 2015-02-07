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
 * Phase5 CybervisionPPC (TVP4020) definitions for the Permedia2 framebuffer
 * driver.
 *
 * Copyright (c) 1998-1999 Ilario Nardinocchi (nardinoc@CS.UniBO.IT)
 * --------------------------------------------------------------------------
 * $Id$
 * --------------------------------------------------------------------------
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 */

#ifndef CVISIONPPC_H
#define CVISIONPPC_H

#ifndef PM2FB_H
#include "pm2fb.h"
#endif

struct cvppc_par {
	unsigned char* pci_config;
	unsigned char* pci_bridge;
	u32 user_flags;
};

#define CSPPC_PCI_BRIDGE		0xfffe0000
#define CSPPC_BRIDGE_ENDIAN		0x0000
#define CSPPC_BRIDGE_INT		0x0010

#define	CVPPC_PCI_CONFIG		0xfffc0000
#define CVPPC_ROM_ADDRESS		0xe2000001
#define CVPPC_REGS_REGION		0xef000000
#define CVPPC_FB_APERTURE_ONE		0xe0000000
#define CVPPC_FB_APERTURE_TWO		0xe1000000
#define CVPPC_FB_SIZE			0x00800000
#define CVPPC_MEM_CONFIG_OLD		0xed61fcaa	/* FIXME Fujitsu?? */
#define CVPPC_MEM_CONFIG_NEW		0xed41c532	/* FIXME USA?? */
#define CVPPC_MEMCLOCK			83000		/* in KHz */

/* CVPPC_BRIDGE_ENDIAN */
#define CSPPCF_BRIDGE_BIG_ENDIAN	0x02

/* CVPPC_BRIDGE_INT */
#define CSPPCF_BRIDGE_ACTIVE_INT2	0x01

#endif	/* CVISIONPPC_H */

/*****************************************************************************
 * That's all folks!
 *****************************************************************************/
