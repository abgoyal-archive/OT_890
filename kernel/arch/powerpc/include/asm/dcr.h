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
 * (c) Copyright 2006 Benjamin Herrenschmidt, IBM Corp.
 *                    <benh@kernel.crashing.org>
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef _ASM_POWERPC_DCR_H
#define _ASM_POWERPC_DCR_H
#ifdef __KERNEL__
#ifndef __ASSEMBLY__
#ifdef CONFIG_PPC_DCR

#ifdef CONFIG_PPC_DCR_NATIVE
#include <asm/dcr-native.h>
#endif

#ifdef CONFIG_PPC_DCR_MMIO
#include <asm/dcr-mmio.h>
#endif


/* Indirection layer for providing both NATIVE and MMIO support. */

#if defined(CONFIG_PPC_DCR_NATIVE) && defined(CONFIG_PPC_DCR_MMIO)

#include <asm/dcr-generic.h>

#define DCR_MAP_OK(host)	dcr_map_ok_generic(host)
#define dcr_map(dev, dcr_n, dcr_c) dcr_map_generic(dev, dcr_n, dcr_c)
#define dcr_unmap(host, dcr_c) dcr_unmap_generic(host, dcr_c)
#define dcr_read(host, dcr_n) dcr_read_generic(host, dcr_n)
#define dcr_write(host, dcr_n, value) dcr_write_generic(host, dcr_n, value)

#else

#ifdef CONFIG_PPC_DCR_NATIVE
typedef dcr_host_native_t dcr_host_t;
#define DCR_MAP_OK(host)	dcr_map_ok_native(host)
#define dcr_map(dev, dcr_n, dcr_c) dcr_map_native(dev, dcr_n, dcr_c)
#define dcr_unmap(host, dcr_c) dcr_unmap_native(host, dcr_c)
#define dcr_read(host, dcr_n) dcr_read_native(host, dcr_n)
#define dcr_write(host, dcr_n, value) dcr_write_native(host, dcr_n, value)
#else
typedef dcr_host_mmio_t dcr_host_t;
#define DCR_MAP_OK(host)	dcr_map_ok_mmio(host)
#define dcr_map(dev, dcr_n, dcr_c) dcr_map_mmio(dev, dcr_n, dcr_c)
#define dcr_unmap(host, dcr_c) dcr_unmap_mmio(host, dcr_c)
#define dcr_read(host, dcr_n) dcr_read_mmio(host, dcr_n)
#define dcr_write(host, dcr_n, value) dcr_write_mmio(host, dcr_n, value)
#endif

#endif /* defined(CONFIG_PPC_DCR_NATIVE) && defined(CONFIG_PPC_DCR_MMIO) */

/*
 * additional helpers to read the DCR * base from the device-tree
 */
struct device_node;
extern unsigned int dcr_resource_start(const struct device_node *np,
				       unsigned int index);
extern unsigned int dcr_resource_len(const struct device_node *np,
				     unsigned int index);
#endif /* CONFIG_PPC_DCR */
#endif /* __ASSEMBLY__ */
#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_DCR_H */
