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

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/syscalls.h>
#include <linux/time.h>
#include <linux/sem.h>
#include <linux/msg.h>
#include <linux/shm.h>
#include <linux/ipc.h>
#include <linux/compat.h>
#include <asm/sys_ia32.h>

asmlinkage long sys32_ipc(u32 call, int first, int second, int third,
			  compat_uptr_t ptr, u32 fifth)
{
	int version;

	version = call >> 16; /* hack for backward compatibility */
	call &= 0xffff;

	switch (call) {
	case SEMOP:
		/* struct sembuf is the same on 32 and 64bit :)) */
		return sys_semtimedop(first, compat_ptr(ptr), second, NULL);
	case SEMTIMEDOP:
		return compat_sys_semtimedop(first, compat_ptr(ptr), second,
						compat_ptr(fifth));
	case SEMGET:
		return sys_semget(first, second, third);
	case SEMCTL:
		return compat_sys_semctl(first, second, third, compat_ptr(ptr));

	case MSGSND:
		return compat_sys_msgsnd(first, second, third, compat_ptr(ptr));
	case MSGRCV:
		return compat_sys_msgrcv(first, second, fifth, third,
					 version, compat_ptr(ptr));
	case MSGGET:
		return sys_msgget((key_t) first, second);
	case MSGCTL:
		return compat_sys_msgctl(first, second, compat_ptr(ptr));

	case SHMAT:
		return compat_sys_shmat(first, second, third, version,
					compat_ptr(ptr));
	case SHMDT:
		return sys_shmdt(compat_ptr(ptr));
	case SHMGET:
		return sys_shmget(first, (unsigned)second, third);
	case SHMCTL:
		return compat_sys_shmctl(first, second, compat_ptr(ptr));
	}
	return -ENOSYS;
}
