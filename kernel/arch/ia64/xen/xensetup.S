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
 * Support routines for Xen
 *
 * Copyright (C) 2005 Dan Magenheimer <dan.magenheimer@hp.com>
 */

#include <asm/processor.h>
#include <asm/asmmacro.h>
#include <asm/pgtable.h>
#include <asm/system.h>
#include <asm/paravirt.h>
#include <asm/xen/privop.h>
#include <linux/elfnote.h>
#include <linux/init.h>
#include <xen/interface/elfnote.h>

	.section .data.read_mostly
	.align 8
	.global xen_domain_type
xen_domain_type:
	data4 XEN_NATIVE_ASM
	.previous

	__INIT
ENTRY(startup_xen)
	// Calculate load offset.
	// The constant, LOAD_OFFSET, can't be used because the boot
	// loader doesn't always load to the LMA specified by the vmlinux.lds.
	mov r9=ip	// must be the first instruction to make sure
			// that r9 = the physical address of startup_xen.
			// Usually r9 = startup_xen - LOAD_OFFSET
	movl r8=startup_xen
	;;
	sub r9=r9,r8	// Usually r9 = -LOAD_OFFSET.

	mov r10=PARAVIRT_HYPERVISOR_TYPE_XEN
	movl r11=_start
	;;
	add r11=r11,r9
	movl r8=hypervisor_type
	;;
	add r8=r8,r9
	mov b0=r11
	;;
	st8 [r8]=r10
	br.cond.sptk.many b0
	;;
END(startup_xen)

	ELFNOTE(Xen, XEN_ELFNOTE_GUEST_OS,	.asciz "linux")
	ELFNOTE(Xen, XEN_ELFNOTE_GUEST_VERSION,	.asciz "2.6")
	ELFNOTE(Xen, XEN_ELFNOTE_XEN_VERSION,	.asciz "xen-3.0")
	ELFNOTE(Xen, XEN_ELFNOTE_ENTRY,		data8.ua startup_xen - LOAD_OFFSET)

#define isBP	p3	// are we the Bootstrap Processor?

	.text

GLOBAL_ENTRY(xen_setup_hook)
	mov r8=XEN_PV_DOMAIN_ASM
(isBP)	movl r9=xen_domain_type;;
(isBP)	st4 [r9]=r8
	movl r10=xen_ivt;;

	mov cr.iva=r10

	/* Set xsi base.  */
#define FW_HYPERCALL_SET_SHARED_INFO_VA			0x600
(isBP)	mov r2=FW_HYPERCALL_SET_SHARED_INFO_VA
(isBP)	movl r28=XSI_BASE;;
(isBP)	break 0x1000;;

	/* setup pv_ops */
(isBP)	mov r4=rp
	;;
(isBP)	br.call.sptk.many rp=xen_setup_pv_ops
	;;
(isBP)	mov rp=r4
	;;

	br.ret.sptk.many rp
	;;
END(xen_setup_hook)
