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

/******************************************************************************
 * features.h
 *
 * Feature flags, reported by XENVER_get_features.
 *
 * Copyright (c) 2006, Keir Fraser <keir@xensource.com>
 */

#ifndef __XEN_PUBLIC_FEATURES_H__
#define __XEN_PUBLIC_FEATURES_H__

/*
 * If set, the guest does not need to write-protect its pagetables, and can
 * update them via direct writes.
 */
#define XENFEAT_writable_page_tables       0

/*
 * If set, the guest does not need to write-protect its segment descriptor
 * tables, and can update them via direct writes.
 */
#define XENFEAT_writable_descriptor_tables 1

/*
 * If set, translation between the guest's 'pseudo-physical' address space
 * and the host's machine address space are handled by the hypervisor. In this
 * mode the guest does not need to perform phys-to/from-machine translations
 * when performing page table operations.
 */
#define XENFEAT_auto_translated_physmap    2

/* If set, the guest is running in supervisor mode (e.g., x86 ring 0). */
#define XENFEAT_supervisor_mode_kernel     3

/*
 * If set, the guest does not need to allocate x86 PAE page directories
 * below 4GB. This flag is usually implied by auto_translated_physmap.
 */
#define XENFEAT_pae_pgdir_above_4gb        4

/* x86: Does this Xen host support the MMU_PT_UPDATE_PRESERVE_AD hypercall? */
#define XENFEAT_mmu_pt_update_preserve_ad  5

#define XENFEAT_NR_SUBMAPS 1

#endif /* __XEN_PUBLIC_FEATURES_H__ */
