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

#ifndef __BEN_VLAN_802_1Q_INC__
#define __BEN_VLAN_802_1Q_INC__

#include <linux/if_vlan.h>


/**
 *	struct vlan_priority_tci_mapping - vlan egress priority mappings
 *	@priority: skb priority
 *	@vlan_qos: vlan priority: (skb->priority << 13) & 0xE000
 *	@next: pointer to next struct
 */
struct vlan_priority_tci_mapping {
	u32					priority;
	u16					vlan_qos;
	struct vlan_priority_tci_mapping	*next;
};

/**
 *	struct vlan_dev_info - VLAN private device data
 *	@nr_ingress_mappings: number of ingress priority mappings
 *	@ingress_priority_map: ingress priority mappings
 *	@nr_egress_mappings: number of egress priority mappings
 *	@egress_priority_map: hash of egress priority mappings
 *	@vlan_id: VLAN identifier
 *	@flags: device flags
 *	@real_dev: underlying netdevice
 *	@real_dev_addr: address of underlying netdevice
 *	@dent: proc dir entry
 *	@cnt_inc_headroom_on_tx: statistic - number of skb expansions on TX
 *	@cnt_encap_on_xmit: statistic - number of skb encapsulations on TX
 */
struct vlan_dev_info {
	unsigned int				nr_ingress_mappings;
	u32					ingress_priority_map[8];
	unsigned int				nr_egress_mappings;
	struct vlan_priority_tci_mapping	*egress_priority_map[16];

	u16					vlan_id;
	u16					flags;

	struct net_device			*real_dev;
	unsigned char				real_dev_addr[ETH_ALEN];

	struct proc_dir_entry			*dent;
	unsigned long				cnt_inc_headroom_on_tx;
	unsigned long				cnt_encap_on_xmit;
};

static inline struct vlan_dev_info *vlan_dev_info(const struct net_device *dev)
{
	return netdev_priv(dev);
}

#define VLAN_GRP_HASH_SHIFT	5
#define VLAN_GRP_HASH_SIZE	(1 << VLAN_GRP_HASH_SHIFT)
#define VLAN_GRP_HASH_MASK	(VLAN_GRP_HASH_SIZE - 1)

/*  Find a VLAN device by the MAC address of its Ethernet device, and
 *  it's VLAN ID.  The default configuration is to have VLAN's scope
 *  to be box-wide, so the MAC will be ignored.  The mac will only be
 *  looked at if we are configured to have a separate set of VLANs per
 *  each MAC addressable interface.  Note that this latter option does
 *  NOT follow the spec for VLANs, but may be useful for doing very
 *  large quantities of VLAN MUX/DEMUX onto FrameRelay or ATM PVCs.
 *
 *  Must be invoked with rcu_read_lock (ie preempt disabled)
 *  or with RTNL.
 */
struct net_device *__find_vlan_dev(struct net_device *real_dev, u16 vlan_id);

/* found in vlan_dev.c */
int vlan_skb_recv(struct sk_buff *skb, struct net_device *dev,
		  struct packet_type *ptype, struct net_device *orig_dev);
void vlan_dev_set_ingress_priority(const struct net_device *dev,
				   u32 skb_prio, u16 vlan_prio);
int vlan_dev_set_egress_priority(const struct net_device *dev,
				 u32 skb_prio, u16 vlan_prio);
int vlan_dev_change_flags(const struct net_device *dev, u32 flag, u32 mask);
void vlan_dev_get_realdev_name(const struct net_device *dev, char *result);

int vlan_check_real_dev(struct net_device *real_dev, u16 vlan_id);
void vlan_setup(struct net_device *dev);
int register_vlan_dev(struct net_device *dev);
void unregister_vlan_dev(struct net_device *dev);

static inline u32 vlan_get_ingress_priority(struct net_device *dev,
					    u16 vlan_tci)
{
	struct vlan_dev_info *vip = vlan_dev_info(dev);

	return vip->ingress_priority_map[(vlan_tci >> 13) & 0x7];
}

#ifdef CONFIG_VLAN_8021Q_GVRP
extern int vlan_gvrp_request_join(const struct net_device *dev);
extern void vlan_gvrp_request_leave(const struct net_device *dev);
extern int vlan_gvrp_init_applicant(struct net_device *dev);
extern void vlan_gvrp_uninit_applicant(struct net_device *dev);
extern int vlan_gvrp_init(void);
extern void vlan_gvrp_uninit(void);
#else
static inline int vlan_gvrp_request_join(const struct net_device *dev) { return 0; }
static inline void vlan_gvrp_request_leave(const struct net_device *dev) {}
static inline int vlan_gvrp_init_applicant(struct net_device *dev) { return 0; }
static inline void vlan_gvrp_uninit_applicant(struct net_device *dev) {}
static inline int vlan_gvrp_init(void) { return 0; }
static inline void vlan_gvrp_uninit(void) {}
#endif

extern const char vlan_fullname[];
extern const char vlan_version[];
extern int vlan_netlink_init(void);
extern void vlan_netlink_fini(void);

extern struct rtnl_link_ops vlan_link_ops;

static inline int is_vlan_dev(struct net_device *dev)
{
	return dev->priv_flags & IFF_802_1Q_VLAN;
}

extern int vlan_net_id;

struct proc_dir_entry;

struct vlan_net {
	/* /proc/net/vlan */
	struct proc_dir_entry *proc_vlan_dir;
	/* /proc/net/vlan/config */
	struct proc_dir_entry *proc_vlan_conf;
	/* Determines interface naming scheme. */
	unsigned short name_type;
};

#endif /* !(__BEN_VLAN_802_1Q_INC__) */
