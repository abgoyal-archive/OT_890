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

/* compat.c: 32-bit compatibility syscall for 64-bit systems
 *
 * Copyright (C) 2004-5 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/syscalls.h>
#include <linux/keyctl.h>
#include <linux/compat.h>
#include "internal.h"

/*****************************************************************************/
/*
 * the key control system call, 32-bit compatibility version for 64-bit archs
 * - this should only be called if the 64-bit arch uses weird pointers in
 *   32-bit mode or doesn't guarantee that the top 32-bits of the argument
 *   registers on taking a 32-bit syscall are zero
 * - if you can, you should call sys_keyctl directly
 */
asmlinkage long compat_sys_keyctl(u32 option,
				  u32 arg2, u32 arg3, u32 arg4, u32 arg5)
{
	switch (option) {
	case KEYCTL_GET_KEYRING_ID:
		return keyctl_get_keyring_ID(arg2, arg3);

	case KEYCTL_JOIN_SESSION_KEYRING:
		return keyctl_join_session_keyring(compat_ptr(arg2));

	case KEYCTL_UPDATE:
		return keyctl_update_key(arg2, compat_ptr(arg3), arg4);

	case KEYCTL_REVOKE:
		return keyctl_revoke_key(arg2);

	case KEYCTL_DESCRIBE:
		return keyctl_describe_key(arg2, compat_ptr(arg3), arg4);

	case KEYCTL_CLEAR:
		return keyctl_keyring_clear(arg2);

	case KEYCTL_LINK:
		return keyctl_keyring_link(arg2, arg3);

	case KEYCTL_UNLINK:
		return keyctl_keyring_unlink(arg2, arg3);

	case KEYCTL_SEARCH:
		return keyctl_keyring_search(arg2, compat_ptr(arg3),
					     compat_ptr(arg4), arg5);

	case KEYCTL_READ:
		return keyctl_read_key(arg2, compat_ptr(arg3), arg4);

	case KEYCTL_CHOWN:
		return keyctl_chown_key(arg2, arg3, arg4);

	case KEYCTL_SETPERM:
		return keyctl_setperm_key(arg2, arg3);

	case KEYCTL_INSTANTIATE:
		return keyctl_instantiate_key(arg2, compat_ptr(arg3), arg4,
					      arg5);

	case KEYCTL_NEGATE:
		return keyctl_negate_key(arg2, arg3, arg4);

	case KEYCTL_SET_REQKEY_KEYRING:
		return keyctl_set_reqkey_keyring(arg2);

	case KEYCTL_SET_TIMEOUT:
		return keyctl_set_timeout(arg2, arg3);

	case KEYCTL_ASSUME_AUTHORITY:
		return keyctl_assume_authority(arg2);

	case KEYCTL_GET_SECURITY:
		return keyctl_get_security(arg2, compat_ptr(arg3), arg4);

	default:
		return -EOPNOTSUPP;
	}

} /* end compat_sys_keyctl() */
