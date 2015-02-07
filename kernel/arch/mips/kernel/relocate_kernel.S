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
 * relocate_kernel.S for kexec
 * Created by <nschichan@corp.free.fr> on Thu Oct 12 17:49:57 2006
 *
 * This source code is licensed under the GNU General Public License,
 * Version 2.  See the file COPYING for more details.
 */

#include <asm/asm.h>
#include <asm/asmmacro.h>
#include <asm/regdef.h>
#include <asm/page.h>
#include <asm/mipsregs.h>
#include <asm/stackframe.h>
#include <asm/addrspace.h>

LEAF(relocate_new_kernel)
	PTR_L		s0, kexec_indirection_page
	PTR_L		s1, kexec_start_address

process_entry:
	PTR_L		s2, (s0)
	PTR_ADD		s0, s0, SZREG

	/* destination page */
	and		s3, s2, 0x1
	beq		s3, zero, 1f
	and		s4, s2, ~0x1	/* store destination addr in s4 */
	move		a0, s4
	b		process_entry

1:
	/* indirection page, update s0  */
	and		s3, s2, 0x2
	beq		s3, zero, 1f
	and		s0, s2, ~0x2
	b		process_entry

1:
	/* done page */
	and		s3, s2, 0x4
	beq		s3, zero, 1f
	b		done
1:
	/* source page */
	and		s3, s2, 0x8
	beq		s3, zero, process_entry
	and		s2, s2, ~0x8
	li		s6, (1 << PAGE_SHIFT) / SZREG

copy_word:
	/* copy page word by word */
	REG_L		s5, (s2)
	REG_S		s5, (s4)
	PTR_ADD		s4, s4, SZREG
	PTR_ADD		s2, s2, SZREG
	LONG_SUB	s6, s6, 1
	beq		s6, zero, process_entry
	b		copy_word
	b		process_entry

done:
	/* jump to kexec_start_address */
	j		s1
	END(relocate_new_kernel)

kexec_start_address:
	EXPORT(kexec_start_address)
	PTR		0x0
	.size		kexec_start_address, PTRSIZE

kexec_indirection_page:
	EXPORT(kexec_indirection_page)
	PTR		0
	.size		kexec_indirection_page, PTRSIZE

relocate_new_kernel_end:

relocate_new_kernel_size:
	EXPORT(relocate_new_kernel_size)
	PTR		relocate_new_kernel_end - relocate_new_kernel
	.size		relocate_new_kernel_size, PTRSIZE
