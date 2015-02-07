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

#ifndef _PLATFORMS_ISERIES_PCI_H
#define _PLATFORMS_ISERIES_PCI_H

/*
 * Created by Allan Trautman on Tue Feb 20, 2001.
 *
 * Define some useful macros for the iSeries pci routines.
 * Copyright (C) 2001  Allan H Trautman, IBM Corporation
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
 * along with this program; if not, write to the:
 * Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330,
 * Boston, MA  02111-1307  USA
 *
 * Change Activity:
 *   Created Feb 20, 2001
 *   Added device reset, March 22, 2001
 *   Ported to ppc64, May 25, 2001
 * End Change Activity
 */

/*
 * Decodes Linux DevFn to iSeries DevFn, bridge device, or function.
 * For Linux, see PCI_SLOT and PCI_FUNC in include/linux/pci.h
 */

#define ISERIES_PCI_AGENTID(idsel, func)	\
	(((idsel & 0x0F) << 4) | (func & 0x07))
#define ISERIES_ENCODE_DEVICE(agentid)		\
	((0x10) | ((agentid & 0x20) >> 2) | (agentid & 0x07))

#define ISERIES_GET_DEVICE_FROM_SUBBUS(subbus)		((subbus >> 5) & 0x7)
#define ISERIES_GET_FUNCTION_FROM_SUBBUS(subbus)	((subbus >> 2) & 0x7)

struct pci_dev;

#ifdef CONFIG_PCI
extern void	iSeries_pcibios_init(void);
extern void	iSeries_pci_final_fixup(void);
extern void 	iSeries_pcibios_fixup_resources(struct pci_dev *dev);
#else
static inline void	iSeries_pcibios_init(void) { }
static inline void	iSeries_pci_final_fixup(void) { }
static inline void 	iSeries_pcibios_fixup_resources(struct pci_dev *dev) {}
#endif

#endif /* _PLATFORMS_ISERIES_PCI_H */
