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

#ifndef __PPC64_VDSO_H__
#define __PPC64_VDSO_H__

#ifdef __KERNEL__

/* Default link addresses for the vDSOs */
#define VDSO32_LBASE	0x100000
#define VDSO64_LBASE	0x100000

/* Default map addresses */
#define VDSO32_MBASE	VDSO32_LBASE
#define VDSO64_MBASE	VDSO64_LBASE

#define VDSO_VERSION_STRING	LINUX_2.6.15

/* Define if 64 bits VDSO has procedure descriptors */
#undef VDS64_HAS_DESCRIPTORS

#ifndef __ASSEMBLY__

/* Offsets relative to thread->vdso_base */
extern unsigned long vdso64_rt_sigtramp;
extern unsigned long vdso32_sigtramp;
extern unsigned long vdso32_rt_sigtramp;

#else /* __ASSEMBLY__ */

#ifdef __VDSO64__
#ifdef VDS64_HAS_DESCRIPTORS
#define V_FUNCTION_BEGIN(name)		\
	.globl name;			\
        .section ".opd","a";		\
        .align 3;			\
	name:				\
	.quad .name,.TOC.@tocbase,0;	\
	.previous;			\
	.globl .name;			\
	.type .name,@function; 		\
	.name:				\

#define V_FUNCTION_END(name)		\
	.size .name,.-.name;

#define V_LOCAL_FUNC(name) (.name)

#else /* VDS64_HAS_DESCRIPTORS */

#define V_FUNCTION_BEGIN(name)		\
	.globl name;			\
	name:				\

#define V_FUNCTION_END(name)		\
	.size name,.-name;

#define V_LOCAL_FUNC(name) (name)

#endif /* VDS64_HAS_DESCRIPTORS */
#endif /* __VDSO64__ */

#ifdef __VDSO32__

#define V_FUNCTION_BEGIN(name)		\
	.globl name;			\
	.type name,@function; 		\
	name:				\

#define V_FUNCTION_END(name)		\
	.size name,.-name;

#define V_LOCAL_FUNC(name) (name)

#endif /* __VDSO32__ */

#endif /* __ASSEMBLY__ */

#endif /* __KERNEL__ */

#endif /* __PPC64_VDSO_H__ */
