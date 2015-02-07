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
 *  SCSI Transport Netlink Interface
 *    Used for the posting of outbound SCSI transport events
 *
 *  Copyright (C) 2006   James Smart, Emulex Corporation
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#ifndef SCSI_NETLINK_H
#define SCSI_NETLINK_H

#include <linux/netlink.h>


/*
 * This file intended to be included by both kernel and user space
 */

/* Single Netlink Message type to send all SCSI Transport messages */
#define SCSI_TRANSPORT_MSG		NLMSG_MIN_TYPE + 1

/* SCSI Transport Broadcast Groups */
	/* leaving groups 0 and 1 unassigned */
#define SCSI_NL_GRP_FC_EVENTS		(1<<2)		/* Group 2 */
#define SCSI_NL_GRP_CNT			3


/* SCSI_TRANSPORT_MSG event message header */
struct scsi_nl_hdr {
	uint8_t version;
	uint8_t transport;
	uint16_t magic;
	uint16_t msgtype;
	uint16_t msglen;
} __attribute__((aligned(sizeof(uint64_t))));

/* scsi_nl_hdr->version value */
#define SCSI_NL_VERSION				1

/* scsi_nl_hdr->magic value */
#define SCSI_NL_MAGIC				0xA1B2

/* scsi_nl_hdr->transport value */
#define SCSI_NL_TRANSPORT			0
#define SCSI_NL_TRANSPORT_FC			1
#define SCSI_NL_MAX_TRANSPORTS			2

/* Transport-based scsi_nl_hdr->msgtype values are defined in each transport */

/*
 * GENERIC SCSI scsi_nl_hdr->msgtype Values
 */
	/* kernel -> user */
#define SCSI_NL_SHOST_VENDOR			0x0001
	/* user -> kernel */
/* SCSI_NL_SHOST_VENDOR msgtype is kernel->user and user->kernel */


/*
 * Message Structures :
 */

/* macro to round up message lengths to 8byte boundary */
#define SCSI_NL_MSGALIGN(len)		(((len) + 7) & ~7)


/*
 * SCSI HOST Vendor Unique messages :
 *   SCSI_NL_SHOST_VENDOR
 *
 * Note: The Vendor Unique message payload will begin directly after
 * 	 this structure, with the length of the payload per vmsg_datalen.
 *
 * Note: When specifying vendor_id, be sure to read the Vendor Type and ID
 *   formatting requirements specified below
 */
struct scsi_nl_host_vendor_msg {
	struct scsi_nl_hdr snlh;		/* must be 1st element ! */
	uint64_t vendor_id;
	uint16_t host_no;
	uint16_t vmsg_datalen;
} __attribute__((aligned(sizeof(uint64_t))));


/*
 * Vendor ID:
 *   If transports post vendor-unique events, they must pass a well-known
 *   32-bit vendor identifier. This identifier consists of 8 bits indicating
 *   the "type" of identifier contained, and 24 bits of id data.
 *
 *   Identifiers for each type:
 *    PCI :  ID data is the 16 bit PCI Registered Vendor ID
 */
#define SCSI_NL_VID_TYPE_SHIFT		56
#define SCSI_NL_VID_TYPE_MASK		((u64)0xFF << SCSI_NL_VID_TYPE_SHIFT)
#define SCSI_NL_VID_TYPE_PCI		((u64)0x01 << SCSI_NL_VID_TYPE_SHIFT)
#define SCSI_NL_VID_ID_MASK		(~ SCSI_NL_VID_TYPE_MASK)


#define INIT_SCSI_NL_HDR(hdr, t, mtype, mlen)			\
	{							\
	(hdr)->version = SCSI_NL_VERSION;			\
	(hdr)->transport = t;					\
	(hdr)->magic = SCSI_NL_MAGIC;				\
	(hdr)->msgtype = mtype;					\
	(hdr)->msglen = mlen;					\
	}


#ifdef __KERNEL__

#include <scsi/scsi_host.h>

/* Exported Kernel Interfaces */
int scsi_nl_add_transport(u8 tport,
	 int (*msg_handler)(struct sk_buff *),
	void (*event_handler)(struct notifier_block *, unsigned long, void *));
void scsi_nl_remove_transport(u8 tport);

int scsi_nl_add_driver(u64 vendor_id, struct scsi_host_template *hostt,
	int (*nlmsg_handler)(struct Scsi_Host *shost, void *payload,
				 u32 len, u32 pid),
	void (*nlevt_handler)(struct notifier_block *nb,
				 unsigned long event, void *notify_ptr));
void scsi_nl_remove_driver(u64 vendor_id);

void scsi_nl_send_transport_msg(u32 pid, struct scsi_nl_hdr *hdr);
int scsi_nl_send_vendor_msg(u32 pid, unsigned short host_no, u64 vendor_id,
			 char *data_buf, u32 data_len);

#endif /* __KERNEL__ */

#endif /* SCSI_NETLINK_H */

