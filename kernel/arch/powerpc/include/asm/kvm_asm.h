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
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * Copyright IBM Corp. 2008
 *
 * Authors: Hollis Blanchard <hollisb@us.ibm.com>
 */

#ifndef __POWERPC_KVM_ASM_H__
#define __POWERPC_KVM_ASM_H__

/* IVPR must be 64KiB-aligned. */
#define VCPU_SIZE_ORDER 4
#define VCPU_SIZE_LOG   (VCPU_SIZE_ORDER + 12)
#define VCPU_TLB_PGSZ   PPC44x_TLB_64K
#define VCPU_SIZE_BYTES (1<<VCPU_SIZE_LOG)

#define BOOKE_INTERRUPT_CRITICAL 0
#define BOOKE_INTERRUPT_MACHINE_CHECK 1
#define BOOKE_INTERRUPT_DATA_STORAGE 2
#define BOOKE_INTERRUPT_INST_STORAGE 3
#define BOOKE_INTERRUPT_EXTERNAL 4
#define BOOKE_INTERRUPT_ALIGNMENT 5
#define BOOKE_INTERRUPT_PROGRAM 6
#define BOOKE_INTERRUPT_FP_UNAVAIL 7
#define BOOKE_INTERRUPT_SYSCALL 8
#define BOOKE_INTERRUPT_AP_UNAVAIL 9
#define BOOKE_INTERRUPT_DECREMENTER 10
#define BOOKE_INTERRUPT_FIT 11
#define BOOKE_INTERRUPT_WATCHDOG 12
#define BOOKE_INTERRUPT_DTLB_MISS 13
#define BOOKE_INTERRUPT_ITLB_MISS 14
#define BOOKE_INTERRUPT_DEBUG 15
#define BOOKE_MAX_INTERRUPT 15

#define RESUME_FLAG_NV          (1<<0)  /* Reload guest nonvolatile state? */
#define RESUME_FLAG_HOST        (1<<1)  /* Resume host? */

#define RESUME_GUEST            0
#define RESUME_GUEST_NV         RESUME_FLAG_NV
#define RESUME_HOST             RESUME_FLAG_HOST
#define RESUME_HOST_NV          (RESUME_FLAG_HOST|RESUME_FLAG_NV)

#endif /* __POWERPC_KVM_ASM_H__ */
