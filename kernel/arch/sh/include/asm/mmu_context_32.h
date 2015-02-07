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

#ifndef __ASM_SH_MMU_CONTEXT_32_H
#define __ASM_SH_MMU_CONTEXT_32_H

/*
 * Destroy context related info for an mm_struct that is about
 * to be put to rest.
 */
static inline void destroy_context(struct mm_struct *mm)
{
	/* Do nothing */
}

static inline void set_asid(unsigned long asid)
{
	unsigned long __dummy;

	__asm__ __volatile__ ("mov.l	%2, %0\n\t"
			      "and	%3, %0\n\t"
			      "or	%1, %0\n\t"
			      "mov.l	%0, %2"
			      : "=&r" (__dummy)
			      : "r" (asid), "m" (__m(MMU_PTEH)),
			        "r" (0xffffff00));
}

static inline unsigned long get_asid(void)
{
	unsigned long asid;

	__asm__ __volatile__ ("mov.l	%1, %0"
			      : "=r" (asid)
			      : "m" (__m(MMU_PTEH)));
	asid &= MMU_CONTEXT_ASID_MASK;
	return asid;
}

/* MMU_TTB is used for optimizing the fault handling. */
static inline void set_TTB(pgd_t *pgd)
{
	ctrl_outl((unsigned long)pgd, MMU_TTB);
}

static inline pgd_t *get_TTB(void)
{
	return (pgd_t *)ctrl_inl(MMU_TTB);
}
#endif /* __ASM_SH_MMU_CONTEXT_32_H */
