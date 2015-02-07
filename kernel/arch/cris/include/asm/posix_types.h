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

/* $Id$ */

/* We cheat a bit and use our C-coded bitops functions from asm/bitops.h */
/* I guess we should write these in assembler because they are used often. */

#ifndef __ARCH_CRIS_POSIX_TYPES_H
#define __ARCH_CRIS_POSIX_TYPES_H

/*
 * This file is generally used by user-level software, so you need to
 * be a little careful about namespace pollution etc.  Also, we cannot
 * assume GCC is being used.
 */

typedef unsigned long	__kernel_ino_t;
typedef unsigned short	__kernel_mode_t;
typedef unsigned short	__kernel_nlink_t;
typedef long		__kernel_off_t;
typedef int		__kernel_pid_t;
typedef unsigned short  __kernel_ipc_pid_t;
typedef unsigned short	__kernel_uid_t;
typedef unsigned short	__kernel_gid_t;
typedef __SIZE_TYPE__	__kernel_size_t;
typedef long		__kernel_ssize_t;
typedef int		__kernel_ptrdiff_t;
typedef long		__kernel_time_t;
typedef long            __kernel_suseconds_t;
typedef long		__kernel_clock_t;
typedef int		__kernel_timer_t;
typedef int		__kernel_clockid_t;
typedef int		__kernel_daddr_t;
typedef char *		__kernel_caddr_t;
typedef unsigned short  __kernel_uid16_t;
typedef unsigned short  __kernel_gid16_t;
typedef unsigned int    __kernel_uid32_t;
typedef unsigned int    __kernel_gid32_t;

typedef unsigned short  __kernel_old_uid_t;
typedef unsigned short  __kernel_old_gid_t;
typedef unsigned short	__kernel_old_dev_t;

#ifdef __GNUC__
typedef long long	__kernel_loff_t;
#endif

typedef struct {
	int	val[2];
} __kernel_fsid_t;

#ifdef __KERNEL__

#undef	__FD_SET
#define __FD_SET(fd,fdsetp) set_bit(fd, (void *)(fdsetp))

#undef	__FD_CLR
#define __FD_CLR(fd,fdsetp) clear_bit(fd, (void *)(fdsetp))

#undef	__FD_ISSET
#define __FD_ISSET(fd,fdsetp) test_bit(fd, (void *)(fdsetp))

#undef	__FD_ZERO
#define __FD_ZERO(fdsetp) memset((void *)(fdsetp), 0, __FDSET_LONGS << 2)

#endif /* __KERNEL__ */

#endif /* __ARCH_CRIS_POSIX_TYPES_H */
