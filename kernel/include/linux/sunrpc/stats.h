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

/*
 * linux/include/linux/sunrpc/stats.h
 *
 * Client statistics collection for SUN RPC
 *
 * Copyright (C) 1996 Olaf Kirch <okir@monad.swb.de>
 */

#ifndef _LINUX_SUNRPC_STATS_H
#define _LINUX_SUNRPC_STATS_H

#include <linux/proc_fs.h>

struct rpc_stat {
	struct rpc_program *	program;

	unsigned int		netcnt,
				netudpcnt,
				nettcpcnt,
				nettcpconn,
				netreconn;
	unsigned int		rpccnt,
				rpcretrans,
				rpcauthrefresh,
				rpcgarbage;
};

struct svc_stat {
	struct svc_program *	program;

	unsigned int		netcnt,
				netudpcnt,
				nettcpcnt,
				nettcpconn;
	unsigned int		rpccnt,
				rpcbadfmt,
				rpcbadauth,
				rpcbadclnt;
};

void			rpc_proc_init(void);
void			rpc_proc_exit(void);
#ifdef MODULE
void			rpc_modcount(struct inode *, int);
#endif

#ifdef CONFIG_PROC_FS
struct proc_dir_entry *	rpc_proc_register(struct rpc_stat *);
void			rpc_proc_unregister(const char *);
void			rpc_proc_zero(struct rpc_program *);
struct proc_dir_entry *	svc_proc_register(struct svc_stat *,
					  const struct file_operations *);
void			svc_proc_unregister(const char *);

void			svc_seq_show(struct seq_file *,
				     const struct svc_stat *);

extern struct proc_dir_entry	*proc_net_rpc;

#else

static inline struct proc_dir_entry *rpc_proc_register(struct rpc_stat *s) { return NULL; }
static inline void rpc_proc_unregister(const char *p) {}
static inline void rpc_proc_zero(struct rpc_program *p) {}

static inline struct proc_dir_entry *svc_proc_register(struct svc_stat *s,
						       const struct file_operations *f) { return NULL; }
static inline void svc_proc_unregister(const char *p) {}

static inline void svc_seq_show(struct seq_file *seq,
				const struct svc_stat *st) {}

#define proc_net_rpc NULL

#endif

#endif /* _LINUX_SUNRPC_STATS_H */
