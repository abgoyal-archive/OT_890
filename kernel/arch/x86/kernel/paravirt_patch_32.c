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

#include <asm/paravirt.h>

DEF_NATIVE(pv_irq_ops, irq_disable, "cli");
DEF_NATIVE(pv_irq_ops, irq_enable, "sti");
DEF_NATIVE(pv_irq_ops, restore_fl, "push %eax; popf");
DEF_NATIVE(pv_irq_ops, save_fl, "pushf; pop %eax");
DEF_NATIVE(pv_cpu_ops, iret, "iret");
DEF_NATIVE(pv_cpu_ops, irq_enable_sysexit, "sti; sysexit");
DEF_NATIVE(pv_mmu_ops, read_cr2, "mov %cr2, %eax");
DEF_NATIVE(pv_mmu_ops, write_cr3, "mov %eax, %cr3");
DEF_NATIVE(pv_mmu_ops, read_cr3, "mov %cr3, %eax");
DEF_NATIVE(pv_cpu_ops, clts, "clts");
DEF_NATIVE(pv_cpu_ops, read_tsc, "rdtsc");

unsigned native_patch(u8 type, u16 clobbers, void *ibuf,
		      unsigned long addr, unsigned len)
{
	const unsigned char *start, *end;
	unsigned ret;

#define PATCH_SITE(ops, x)					\
		case PARAVIRT_PATCH(ops.x):			\
			start = start_##ops##_##x;		\
			end = end_##ops##_##x;			\
			goto patch_site
	switch (type) {
		PATCH_SITE(pv_irq_ops, irq_disable);
		PATCH_SITE(pv_irq_ops, irq_enable);
		PATCH_SITE(pv_irq_ops, restore_fl);
		PATCH_SITE(pv_irq_ops, save_fl);
		PATCH_SITE(pv_cpu_ops, iret);
		PATCH_SITE(pv_cpu_ops, irq_enable_sysexit);
		PATCH_SITE(pv_mmu_ops, read_cr2);
		PATCH_SITE(pv_mmu_ops, read_cr3);
		PATCH_SITE(pv_mmu_ops, write_cr3);
		PATCH_SITE(pv_cpu_ops, clts);
		PATCH_SITE(pv_cpu_ops, read_tsc);

	patch_site:
		ret = paravirt_patch_insns(ibuf, len, start, end);
		break;

	default:
		ret = paravirt_patch_default(type, clobbers, ibuf, addr, len);
		break;
	}
#undef PATCH_SITE
	return ret;
}
