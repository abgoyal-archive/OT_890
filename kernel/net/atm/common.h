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

/* net/atm/common.h - ATM sockets (common part for PVC and SVC) */

/* Written 1995-2000 by Werner Almesberger, EPFL LRC/ICA */


#ifndef NET_ATM_COMMON_H
#define NET_ATM_COMMON_H

#include <linux/net.h>
#include <linux/poll.h> /* for poll_table */


int vcc_create(struct net *net, struct socket *sock, int protocol, int family);
int vcc_release(struct socket *sock);
int vcc_connect(struct socket *sock, int itf, short vpi, int vci);
int vcc_recvmsg(struct kiocb *iocb, struct socket *sock, struct msghdr *msg,
		size_t size, int flags);
int vcc_sendmsg(struct kiocb *iocb, struct socket *sock, struct msghdr *m,
		size_t total_len);
unsigned int vcc_poll(struct file *file, struct socket *sock, poll_table *wait);
int vcc_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg);
int vcc_compat_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg);
int vcc_setsockopt(struct socket *sock, int level, int optname,
		   char __user *optval, int optlen);
int vcc_getsockopt(struct socket *sock, int level, int optname,
		   char __user *optval, int __user *optlen);

int atmpvc_init(void);
void atmpvc_exit(void);
int atmsvc_init(void);
void atmsvc_exit(void);
int atm_sysfs_init(void);
void atm_sysfs_exit(void);

#ifdef CONFIG_PROC_FS
int atm_proc_init(void);
void atm_proc_exit(void);
#else
static inline int atm_proc_init(void)
{
	return 0;
}

static inline void atm_proc_exit(void)
{
	/* nothing */
}
#endif /* CONFIG_PROC_FS */

/* SVC */
int svc_change_qos(struct atm_vcc *vcc,struct atm_qos *qos);

void atm_dev_release_vccs(struct atm_dev *dev);

#endif
