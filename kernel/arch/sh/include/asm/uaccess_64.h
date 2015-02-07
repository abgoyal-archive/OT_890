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

#ifndef __ASM_SH_UACCESS_64_H
#define __ASM_SH_UACCESS_64_H

/*
 * include/asm-sh/uaccess_64.h
 *
 * Copyright (C) 2000, 2001  Paolo Alberelli
 * Copyright (C) 2003, 2004  Paul Mundt
 *
 * User space memory access functions
 *
 * Copyright (C) 1999  Niibe Yutaka
 *
 *  Based on:
 *     MIPS implementation version 1.15 by
 *              Copyright (C) 1996, 1997, 1998 by Ralf Baechle
 *     and i386 version.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#define __get_user_size(x,ptr,size,retval)			\
do {								\
	retval = 0;						\
	switch (size) {						\
	case 1:							\
		retval = __get_user_asm_b((void *)&x,		\
					  (long)ptr);		\
		break;						\
	case 2:							\
		retval = __get_user_asm_w((void *)&x,		\
					  (long)ptr);		\
		break;						\
	case 4:							\
		retval = __get_user_asm_l((void *)&x,		\
					  (long)ptr);		\
		break;						\
	case 8:							\
		retval = __get_user_asm_q((void *)&x,		\
					  (long)ptr);		\
		break;						\
	default:						\
		__get_user_unknown();				\
		break;						\
	}							\
} while (0)

extern long __get_user_asm_b(void *, long);
extern long __get_user_asm_w(void *, long);
extern long __get_user_asm_l(void *, long);
extern long __get_user_asm_q(void *, long);
extern void __get_user_unknown(void);

#define __put_user_size(x,ptr,size,retval)			\
do {								\
	retval = 0;						\
	switch (size) {						\
	case 1:							\
		retval = __put_user_asm_b((void *)&x,		\
					  (long)ptr);		\
		break;						\
	case 2:							\
		retval = __put_user_asm_w((void *)&x,		\
					  (long)ptr);		\
		break;						\
	case 4:							\
		retval = __put_user_asm_l((void *)&x,		\
					  (long)ptr);		\
		break;						\
	case 8:							\
		retval = __put_user_asm_q((void *)&x,		\
					  (long)ptr);		\
		break;						\
	default:						\
		__put_user_unknown();				\
	}							\
} while (0)

extern long __put_user_asm_b(void *, long);
extern long __put_user_asm_w(void *, long);
extern long __put_user_asm_l(void *, long);
extern long __put_user_asm_q(void *, long);
extern void __put_user_unknown(void);

extern long __strnlen_user(const char *__s, long __n);
extern int __strncpy_from_user(unsigned long __dest,
	       unsigned long __user __src, int __count);

#endif /* __ASM_SH_UACCESS_64_H */
