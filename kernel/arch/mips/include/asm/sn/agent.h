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
 * This file has definitions for the hub and snac interfaces.
 *
 * Copyright (C) 1992 - 1997, 1999, 2000 Silcon Graphics, Inc.
 * Copyright (C) 1999, 2000 Ralf Baechle (ralf@gnu.org)
 */
#ifndef _ASM_SGI_SN_AGENT_H
#define _ASM_SGI_SN_AGENT_H

#include <linux/topology.h>
#include <asm/sn/addrs.h>
#include <asm/sn/arch.h>

#if defined(CONFIG_SGI_IP27)
#include <asm/sn/sn0/hub.h>
#elif defined(CONFIG_SGI_IP35)
#include <asm/sn/sn1/hub.h>
#endif	/* !CONFIG_SGI_IP27 && !CONFIG_SGI_IP35 */

/*
 * NIC register macros
 */

#if defined(CONFIG_SGI_IP27)
#define HUB_NIC_ADDR(_cpuid) 						   \
	REMOTE_HUB_ADDR(COMPACT_TO_NASID_NODEID(cpu_to_node(_cpuid)),       \
		MD_MLAN_CTL)
#endif

#define SET_HUB_NIC(_my_cpuid, _val) 				  	   \
	(HUB_S(HUB_NIC_ADDR(_my_cpuid), (_val)))

#define SET_MY_HUB_NIC(_v) 					           \
	SET_HUB_NIC(cpuid(), (_v))

#define GET_HUB_NIC(_my_cpuid) 						   \
	(HUB_L(HUB_NIC_ADDR(_my_cpuid)))

#define GET_MY_HUB_NIC() 						   \
	GET_HUB_NIC(cpuid())

#endif /* _ASM_SGI_SN_AGENT_H */
