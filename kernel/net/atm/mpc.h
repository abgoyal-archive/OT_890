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

#ifndef _MPC_H_
#define _MPC_H_

#include <linux/types.h>
#include <linux/atm.h>
#include <linux/atmmpc.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include "mpoa_caches.h"

/* kernel -> mpc-daemon */
int msg_to_mpoad(struct k_message *msg, struct mpoa_client *mpc);

struct mpoa_client {
	struct mpoa_client *next;
	struct net_device *dev;      /* lec in question                     */
	int dev_num;                 /* e.g. 2 for lec2                     */
	int (*old_hard_start_xmit)(struct sk_buff *skb, struct net_device *dev);
	struct atm_vcc *mpoad_vcc;   /* control channel to mpoad            */
	uint8_t mps_ctrl_addr[ATM_ESA_LEN];  /* MPS control ATM address     */
	uint8_t our_ctrl_addr[ATM_ESA_LEN];  /* MPC's control ATM address   */

	rwlock_t ingress_lock;
	struct in_cache_ops *in_ops; /* ingress cache operations            */
	in_cache_entry *in_cache;    /* the ingress cache of this MPC       */

	rwlock_t egress_lock;
	struct eg_cache_ops *eg_ops; /* egress cache operations             */
	eg_cache_entry *eg_cache;    /* the egress  cache of this MPC       */

	uint8_t *mps_macs;           /* array of MPS MAC addresses, >=1     */
	int number_of_mps_macs;      /* number of the above MAC addresses   */
	struct mpc_parameters parameters;  /* parameters for this client    */
};


struct atm_mpoa_qos {
	struct atm_mpoa_qos *next;
	__be32 ipaddr;
	struct atm_qos qos;
};


/* MPOA QoS operations */
struct atm_mpoa_qos *atm_mpoa_add_qos(__be32 dst_ip, struct atm_qos *qos);
struct atm_mpoa_qos *atm_mpoa_search_qos(__be32 dst_ip);
int atm_mpoa_delete_qos(struct atm_mpoa_qos *qos);

/* Display QoS entries. This is for the procfs */
struct seq_file;
void atm_mpoa_disp_qos(struct seq_file *m);

#ifdef CONFIG_PROC_FS
int mpc_proc_init(void);
void mpc_proc_clean(void);
#else
#define mpc_proc_init() (0)
#define mpc_proc_clean() do { } while(0)
#endif

#endif /* _MPC_H_ */
