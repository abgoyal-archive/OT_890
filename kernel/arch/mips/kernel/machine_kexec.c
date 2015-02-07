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
 * machine_kexec.c for kexec
 * Created by <nschichan@corp.free.fr> on Thu Oct 12 15:15:06 2006
 *
 * This source code is licensed under the GNU General Public License,
 * Version 2.  See the file COPYING for more details.
 */

#include <linux/kexec.h>
#include <linux/mm.h>
#include <linux/delay.h>

#include <asm/cacheflush.h>
#include <asm/page.h>

extern const unsigned char relocate_new_kernel[];
extern const size_t relocate_new_kernel_size;

extern unsigned long kexec_start_address;
extern unsigned long kexec_indirection_page;

int
machine_kexec_prepare(struct kimage *kimage)
{
	return 0;
}

void
machine_kexec_cleanup(struct kimage *kimage)
{
}

void
machine_shutdown(void)
{
}

void
machine_crash_shutdown(struct pt_regs *regs)
{
}

typedef void (*noretfun_t)(void) __attribute__((noreturn));

void
machine_kexec(struct kimage *image)
{
	unsigned long reboot_code_buffer;
	unsigned long entry;
	unsigned long *ptr;

	reboot_code_buffer =
	  (unsigned long)page_address(image->control_code_page);

	kexec_start_address = image->start;
	kexec_indirection_page =
		(unsigned long) phys_to_virt(image->head & PAGE_MASK);

	memcpy((void*)reboot_code_buffer, relocate_new_kernel,
	       relocate_new_kernel_size);

	/*
	 * The generic kexec code builds a page list with physical
	 * addresses. they are directly accessible through KSEG0 (or
	 * CKSEG0 or XPHYS if on 64bit system), hence the
	 * pys_to_virt() call.
	 */
	for (ptr = &image->head; (entry = *ptr) && !(entry &IND_DONE);
	     ptr = (entry & IND_INDIRECTION) ?
	       phys_to_virt(entry & PAGE_MASK) : ptr + 1) {
		if (*ptr & IND_SOURCE || *ptr & IND_INDIRECTION ||
		    *ptr & IND_DESTINATION)
			*ptr = (unsigned long) phys_to_virt(*ptr);
	}

	/*
	 * we do not want to be bothered.
	 */
	local_irq_disable();

	printk("Will call new kernel at %08lx\n", image->start);
	printk("Bye ...\n");
	__flush_cache_all();
	((noretfun_t) reboot_code_buffer)();
}
