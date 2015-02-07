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

/* ----------------------------------------------------------------------- *
 *
 *   Copyright (C) 1991, 1992 Linus Torvalds
 *   Copyright 2007 rPath, Inc. - All Rights Reserved
 *
 *   This file is part of the Linux kernel, and is made available under
 *   the terms of the GNU General Public License version 2.
 *
 * ----------------------------------------------------------------------- */

/*
 * Memory copy routines
 */

	.code16gcc
	.text

	.globl	memcpy
	.type	memcpy, @function
memcpy:
	pushw	%si
	pushw	%di
	movw	%ax, %di
	movw	%dx, %si
	pushw	%cx
	shrw	$2, %cx
	rep; movsl
	popw	%cx
	andw	$3, %cx
	rep; movsb
	popw	%di
	popw	%si
	ret
	.size	memcpy, .-memcpy

	.globl	memset
	.type	memset, @function
memset:
	pushw	%di
	movw	%ax, %di
	movzbl	%dl, %eax
	imull	$0x01010101,%eax
	pushw	%cx
	shrw	$2, %cx
	rep; stosl
	popw	%cx
	andw	$3, %cx
	rep; stosb
	popw	%di
	ret
	.size	memset, .-memset

	.globl	copy_from_fs
	.type	copy_from_fs, @function
copy_from_fs:
	pushw	%ds
	pushw	%fs
	popw	%ds
	call	memcpy
	popw	%ds
	ret
	.size	copy_from_fs, .-copy_from_fs

	.globl	copy_to_fs
	.type	copy_to_fs, @function
copy_to_fs:
	pushw	%es
	pushw	%fs
	popw	%es
	call	memcpy
	popw	%es
	ret
	.size	copy_to_fs, .-copy_to_fs

#if 0 /* Not currently used, but can be enabled as needed */

	.globl	copy_from_gs
	.type	copy_from_gs, @function
copy_from_gs:
	pushw	%ds
	pushw	%gs
	popw	%ds
	call	memcpy
	popw	%ds
	ret
	.size	copy_from_gs, .-copy_from_gs
	.globl	copy_to_gs

	.type	copy_to_gs, @function
copy_to_gs:
	pushw	%es
	pushw	%gs
	popw	%es
	call	memcpy
	popw	%es
	ret
	.size	copy_to_gs, .-copy_to_gs

#endif
