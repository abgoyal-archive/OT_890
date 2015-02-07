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

#ifndef _M68K_SEGMENT_H
#define _M68K_SEGMENT_H

/* define constants */
/* Address spaces (FC0-FC2) */
#define USER_DATA     (1)
#ifndef __USER_DS
#define __USER_DS     (USER_DATA)
#endif
#define USER_PROGRAM  (2)
#define SUPER_DATA    (5)
#ifndef __KERNEL_DS
#define __KERNEL_DS   (SUPER_DATA)
#endif
#define SUPER_PROGRAM (6)
#define CPU_SPACE     (7)

#ifndef __ASSEMBLY__

typedef struct {
	unsigned long seg;
} mm_segment_t;

#define MAKE_MM_SEG(s)	((mm_segment_t) { (s) })
#define USER_DS		MAKE_MM_SEG(__USER_DS)
#define KERNEL_DS	MAKE_MM_SEG(__KERNEL_DS)

/*
 * Get/set the SFC/DFC registers for MOVES instructions
 */

static inline mm_segment_t get_fs(void)
{
	mm_segment_t _v;
	__asm__ ("movec %/dfc,%0":"=r" (_v.seg):);

	return _v;
}

static inline mm_segment_t get_ds(void)
{
    /* return the supervisor data space code */
    return KERNEL_DS;
}

static inline void set_fs(mm_segment_t val)
{
	__asm__ __volatile__ ("movec %0,%/sfc\n\t"
			      "movec %0,%/dfc\n\t"
			      : /* no outputs */ : "r" (val.seg) : "memory");
}

#define segment_eq(a,b)	((a).seg == (b).seg)

#endif /* __ASSEMBLY__ */

#endif /* _M68K_SEGMENT_H */
