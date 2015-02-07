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

#ifndef NET_COMPAT_H
#define NET_COMPAT_H


struct sock;

#if defined(CONFIG_COMPAT)

#include <linux/compat.h>

struct compat_msghdr {
	compat_uptr_t	msg_name;	/* void * */
	compat_int_t	msg_namelen;
	compat_uptr_t	msg_iov;	/* struct compat_iovec * */
	compat_size_t	msg_iovlen;
	compat_uptr_t	msg_control;	/* void * */
	compat_size_t	msg_controllen;
	compat_uint_t	msg_flags;
};

struct compat_cmsghdr {
	compat_size_t	cmsg_len;
	compat_int_t	cmsg_level;
	compat_int_t	cmsg_type;
};

extern int compat_sock_get_timestamp(struct sock *, struct timeval __user *);
extern int compat_sock_get_timestampns(struct sock *, struct timespec __user *);

#else /* defined(CONFIG_COMPAT) */
#define compat_msghdr	msghdr		/* to avoid compiler warnings */
#endif /* defined(CONFIG_COMPAT) */

extern int get_compat_msghdr(struct msghdr *, struct compat_msghdr __user *);
extern int verify_compat_iovec(struct msghdr *, struct iovec *, struct sockaddr *, int);
extern asmlinkage long compat_sys_sendmsg(int,struct compat_msghdr __user *,unsigned);
extern asmlinkage long compat_sys_recvmsg(int,struct compat_msghdr __user *,unsigned);
extern asmlinkage long compat_sys_getsockopt(int, int, int, char __user *, int __user *);
extern int put_cmsg_compat(struct msghdr*, int, int, int, void *);

extern int cmsghdr_from_user_compat_to_kern(struct msghdr *, struct sock *, unsigned char *, int);

extern int compat_mc_setsockopt(struct sock *, int, int, char __user *, int,
	int (*)(struct sock *, int, int, char __user *, int));
extern int compat_mc_getsockopt(struct sock *, int, int, char __user *,
	int __user *, int (*)(struct sock *, int, int, char __user *,
				int __user *));

#endif /* NET_COMPAT_H */
