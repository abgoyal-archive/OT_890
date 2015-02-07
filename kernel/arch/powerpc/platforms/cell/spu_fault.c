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
 * SPU mm fault handler
 *
 * (C) Copyright IBM Deutschland Entwicklung GmbH 2007
 *
 * Author: Arnd Bergmann <arndb@de.ibm.com>
 * Author: Jeremy Kerr <jk@ozlabs.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/module.h>

#include <asm/spu.h>
#include <asm/spu_csa.h>

/*
 * This ought to be kept in sync with the powerpc specific do_page_fault
 * function. Currently, there are a few corner cases that we haven't had
 * to handle fortunately.
 */
int spu_handle_mm_fault(struct mm_struct *mm, unsigned long ea,
		unsigned long dsisr, unsigned *flt)
{
	struct vm_area_struct *vma;
	unsigned long is_write;
	int ret;

#if 0
	if (!IS_VALID_EA(ea)) {
		return -EFAULT;
	}
#endif /* XXX */
	if (mm == NULL) {
		return -EFAULT;
	}
	if (mm->pgd == NULL) {
		return -EFAULT;
	}

	down_read(&mm->mmap_sem);
	vma = find_vma(mm, ea);
	if (!vma)
		goto bad_area;
	if (vma->vm_start <= ea)
		goto good_area;
	if (!(vma->vm_flags & VM_GROWSDOWN))
		goto bad_area;
	if (expand_stack(vma, ea))
		goto bad_area;
good_area:
	is_write = dsisr & MFC_DSISR_ACCESS_PUT;
	if (is_write) {
		if (!(vma->vm_flags & VM_WRITE))
			goto bad_area;
	} else {
		if (dsisr & MFC_DSISR_ACCESS_DENIED)
			goto bad_area;
		if (!(vma->vm_flags & (VM_READ | VM_EXEC)))
			goto bad_area;
	}
	ret = 0;
	*flt = handle_mm_fault(mm, vma, ea, is_write);
	if (unlikely(*flt & VM_FAULT_ERROR)) {
		if (*flt & VM_FAULT_OOM) {
			ret = -ENOMEM;
			goto bad_area;
		} else if (*flt & VM_FAULT_SIGBUS) {
			ret = -EFAULT;
			goto bad_area;
		}
		BUG();
	}
	if (*flt & VM_FAULT_MAJOR)
		current->maj_flt++;
	else
		current->min_flt++;
	up_read(&mm->mmap_sem);
	return ret;

bad_area:
	up_read(&mm->mmap_sem);
	return -EFAULT;
}
EXPORT_SYMBOL_GPL(spu_handle_mm_fault);
