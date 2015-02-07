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
 * Derived from IRIX <sys/SN/SN0/sn0_fru.h>
 *
 * Copyright (C) 1992 - 1997, 1999 Silcon Graphics, Inc.
 * Copyright (C) 1999, 2006 Ralf Baechle (ralf@linux-mips)
 */
#ifndef __ASM_SN_FRU_H
#define __ASM_SN_FRU_H

#define MAX_DIMMS			8	 /* max # of dimm banks */
#define MAX_PCIDEV			8	 /* max # of pci devices on a pci bus */

typedef unsigned char confidence_t;

typedef struct kf_mem_s {
	confidence_t km_confidence; /* confidence level that the memory is bad
				     * is this necessary ?
				     */
	confidence_t km_dimm[MAX_DIMMS];
	                            /* confidence level that dimm[i] is bad
				     *I think this is the right number
				     */

} kf_mem_t;

typedef struct kf_cpu_s {
	confidence_t  	kc_confidence; /* confidence level that cpu is bad */
	confidence_t  	kc_icache; /* confidence level that instr. cache is bad */
	confidence_t  	kc_dcache; /* confidence level that data   cache is bad */
	confidence_t  	kc_scache; /* confidence level that sec.   cache is bad */
	confidence_t	kc_sysbus; /* confidence level that sysad/cmd/state bus is bad */
} kf_cpu_t;

typedef struct kf_pci_bus_s {
	confidence_t	kpb_belief;	/* confidence level  that the  pci bus is bad */
	confidence_t	kpb_pcidev_belief[MAX_PCIDEV];
	                                /* confidence level that the pci dev is bad */
} kf_pci_bus_t;

#endif /* __ASM_SN_FRU_H */
