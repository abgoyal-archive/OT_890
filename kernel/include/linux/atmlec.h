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
 * ATM Lan Emulation Daemon driver interface
 *
 * Marko Kiiskila <mkiiskila@yahoo.com>
 */

#ifndef _ATMLEC_H_
#define _ATMLEC_H_

#include <linux/atmapi.h>
#include <linux/atmioc.h>
#include <linux/atm.h>
#include <linux/if_ether.h>

/* ATM lec daemon control socket */
#define ATMLEC_CTRL	_IO('a', ATMIOC_LANE)
#define ATMLEC_DATA	_IO('a', ATMIOC_LANE+1)
#define ATMLEC_MCAST	_IO('a', ATMIOC_LANE+2)

/* Maximum number of LEC interfaces (tweakable) */
#define MAX_LEC_ITF 48

/*
 * From the total of MAX_LEC_ITF, last NUM_TR_DEVS are reserved for Token Ring.
 * E.g. if MAX_LEC_ITF = 48 and NUM_TR_DEVS = 8, then lec0-lec39 are for
 * Ethernet ELANs and lec40-lec47 are for Token Ring ELANS.
 */
#define NUM_TR_DEVS 8

typedef enum {
	l_set_mac_addr,
	l_del_mac_addr,
	l_svc_setup,
	l_addr_delete,
	l_topology_change,
	l_flush_complete,
	l_arp_update,
	l_narp_req,		/* LANE2 mandates the use of this */
	l_config,
	l_flush_tran_id,
	l_set_lecid,
	l_arp_xmt,
	l_rdesc_arp_xmt,
	l_associate_req,
	l_should_bridge		/* should we bridge this MAC? */
} atmlec_msg_type;

#define ATMLEC_MSG_TYPE_MAX l_should_bridge

struct atmlec_config_msg {
	unsigned int maximum_unknown_frame_count;
	unsigned int max_unknown_frame_time;
	unsigned short max_retry_count;
	unsigned int aging_time;
	unsigned int forward_delay_time;
	unsigned int arp_response_time;
	unsigned int flush_timeout;
	unsigned int path_switching_delay;
	unsigned int lane_version;	/* LANE2: 1 for LANEv1, 2 for LANEv2 */
	int mtu;
	int is_proxy;
};

struct atmlec_msg {
	atmlec_msg_type type;
	int sizeoftlvs;		/* LANE2: if != 0, tlvs follow */
	union {
		struct {
			unsigned char mac_addr[ETH_ALEN];
			unsigned char atm_addr[ATM_ESA_LEN];
			unsigned int flag;	/*
						 * Topology_change flag,
						 * remoteflag, permanent flag,
						 * lecid, transaction id
						 */
			unsigned int targetless_le_arp;	/* LANE2 */
			unsigned int no_source_le_narp;	/* LANE2 */
		} normal;
		struct atmlec_config_msg config;
		struct {
			uint16_t lec_id;			/* requestor lec_id  */
			uint32_t tran_id;			/* transaction id    */
			unsigned char mac_addr[ETH_ALEN];	/* dst mac addr      */
			unsigned char atm_addr[ATM_ESA_LEN];	/* reqestor ATM addr */
		} proxy;	/*
				 * For mapping LE_ARP requests to responses. Filled by
				 * zeppelin, returned by kernel. Used only when proxying
				 */
	} content;
} __ATM_API_ALIGN;

struct atmlec_ioc {
	int dev_num;
	unsigned char atm_addr[ATM_ESA_LEN];
	unsigned char receive;	/* 1= receive vcc, 0 = send vcc */
};
#endif /* _ATMLEC_H_ */
