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

#ifndef _PARISC_SHMBUF_H
#define _PARISC_SHMBUF_H

/* 
 * The shmid64_ds structure for parisc architecture.
 * Note extra padding because this structure is passed back and forth
 * between kernel and user space.
 *
 * Pad space is left for:
 * - 64-bit time_t to solve y2038 problem
 * - 2 miscellaneous 32-bit values
 */

struct shmid64_ds {
	struct ipc64_perm	shm_perm;	/* operation perms */
#ifndef CONFIG_64BIT
	unsigned int		__pad1;
#endif
	__kernel_time_t		shm_atime;	/* last attach time */
#ifndef CONFIG_64BIT
	unsigned int		__pad2;
#endif
	__kernel_time_t		shm_dtime;	/* last detach time */
#ifndef CONFIG_64BIT
	unsigned int		__pad3;
#endif
	__kernel_time_t		shm_ctime;	/* last change time */
#ifndef CONFIG_64BIT
	unsigned int		__pad4;
#endif
	size_t			shm_segsz;	/* size of segment (bytes) */
	__kernel_pid_t		shm_cpid;	/* pid of creator */
	__kernel_pid_t		shm_lpid;	/* pid of last operator */
	unsigned int		shm_nattch;	/* no. of current attaches */
	unsigned int		__unused1;
	unsigned int		__unused2;
};

#ifdef CONFIG_64BIT
/* The 'unsigned int' (formerly 'unsigned long') data types below will
 * ensure that a 32-bit app calling shmctl(*,IPC_INFO,*) will work on
 * a wide kernel, but if some of these values are meant to contain pointers
 * they may need to be 'long long' instead. -PB XXX FIXME
 */
#endif
struct shminfo64 {
	unsigned int	shmmax;
	unsigned int	shmmin;
	unsigned int	shmmni;
	unsigned int	shmseg;
	unsigned int	shmall;
	unsigned int	__unused1;
	unsigned int	__unused2;
	unsigned int	__unused3;
	unsigned int	__unused4;
};

#endif /* _PARISC_SHMBUF_H */
