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

/*********************************************************************
 *                
 * Filename:      irlap_frame.h
 * Version:       0.9
 * Description:   IrLAP frame declarations
 * Status:        Experimental.
 * Author:        Dag Brattli <dagb@cs.uit.no>
 * Created at:    Tue Aug 19 10:27:26 1997
 * Modified at:   Sat Dec 25 21:07:26 1999
 * Modified by:   Dag Brattli <dagb@cs.uit.no>
 * 
 *     Copyright (c) 1997-1999 Dag Brattli <dagb@cs.uit.no>,
 *     All Rights Reserved.
 *     Copyright (c) 2000-2002 Jean Tourrilhes <jt@hpl.hp.com>
 *     
 *     This program is free software; you can redistribute it and/or 
 *     modify it under the terms of the GNU General Public License as 
 *     published by the Free Software Foundation; either version 2 of 
 *     the License, or (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License 
 *     along with this program; if not, write to the Free Software 
 *     Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
 *     MA 02111-1307 USA
 *     
 ********************************************************************/

#ifndef IRLAP_FRAME_H
#define IRLAP_FRAME_H

#include <linux/skbuff.h>

#include <net/irda/irda.h>

/* A few forward declarations (to make compiler happy) */
struct irlap_cb;
struct discovery_t;

/* Frame types and templates */
#define INVALID   0xff

/* Unnumbered (U) commands */
#define SNRM_CMD  0x83 /* Set Normal Response Mode */
#define DISC_CMD  0x43 /* Disconnect */
#define XID_CMD   0x2f /* Exchange Station Identification */
#define TEST_CMD  0xe3 /* Test */

/* Unnumbered responses */
#define RNRM_RSP  0x83 /* Request Normal Response Mode */
#define UA_RSP    0x63 /* Unnumbered Acknowledgement */
#define FRMR_RSP  0x87 /* Frame Reject */
#define DM_RSP    0x0f /* Disconnect Mode */
#define RD_RSP    0x43 /* Request Disconnection */
#define XID_RSP   0xaf /* Exchange Station Identification */
#define TEST_RSP  0xe3 /* Test frame */

/* Supervisory (S) */
#define RR        0x01 /* Receive Ready */
#define REJ       0x09 /* Reject */
#define RNR       0x05 /* Receive Not Ready */
#define SREJ      0x0d /* Selective Reject */

/* Information (I) */
#define I_FRAME   0x00 /* Information Format */
#define UI_FRAME  0x03 /* Unnumbered Information */

#define CMD_FRAME 0x01
#define RSP_FRAME 0x00

#define PF_BIT    0x10 /* Poll/final bit */

/* Some IrLAP field lengths */
/*
 * Only baud rate triplet is 4 bytes (PV can be 2 bytes).
 * All others params (7) are 3 bytes, so that's 7*3 + 1*4 bytes.
 */
#define IRLAP_NEGOCIATION_PARAMS_LEN 25
#define IRLAP_DISCOVERY_INFO_LEN     32

struct disc_frame {
	__u8 caddr;          /* Connection address */
	__u8 control;
} IRDA_PACK;

struct xid_frame {
	__u8  caddr; /* Connection address */
	__u8  control;
	__u8  ident; /* Should always be XID_FORMAT */ 
	__le32 saddr; /* Source device address */
	__le32 daddr; /* Destination device address */
	__u8  flags; /* Discovery flags */
	__u8  slotnr;
	__u8  version;
} IRDA_PACK;

struct test_frame {
	__u8 caddr;          /* Connection address */
	__u8 control;
	__le32 saddr;         /* Source device address */
	__le32 daddr;         /* Destination device address */
} IRDA_PACK;

struct ua_frame {
	__u8 caddr;
	__u8 control;
	__le32 saddr; /* Source device address */
	__le32 daddr; /* Dest device address */
} IRDA_PACK;

struct dm_frame {
	__u8 caddr;          /* Connection address */
	__u8 control;
} IRDA_PACK;

struct rd_frame {
	__u8 caddr;          /* Connection address */
	__u8 control;
} IRDA_PACK;

struct rr_frame {
	__u8 caddr;          /* Connection address */
	__u8 control;
} IRDA_PACK;

struct i_frame {
	__u8 caddr;
	__u8 control;
} IRDA_PACK;

struct snrm_frame {
	__u8  caddr;
	__u8  control;
	__le32 saddr;
	__le32 daddr;
	__u8  ncaddr;
} IRDA_PACK;

void irlap_queue_xmit(struct irlap_cb *self, struct sk_buff *skb);
void irlap_send_discovery_xid_frame(struct irlap_cb *, int S, __u8 s, 
				    __u8 command,
				    struct discovery_t *discovery);
void irlap_send_snrm_frame(struct irlap_cb *, struct qos_info *);
void irlap_send_test_frame(struct irlap_cb *self, __u8 caddr, __u32 daddr, 
			   struct sk_buff *cmd);
void irlap_send_ua_response_frame(struct irlap_cb *, struct qos_info *);
void irlap_send_dm_frame(struct irlap_cb *self);
void irlap_send_rd_frame(struct irlap_cb *self);
void irlap_send_disc_frame(struct irlap_cb *self);
void irlap_send_rr_frame(struct irlap_cb *self, int command);

void irlap_send_data_primary(struct irlap_cb *, struct sk_buff *);
void irlap_send_data_primary_poll(struct irlap_cb *, struct sk_buff *);
void irlap_send_data_secondary(struct irlap_cb *, struct sk_buff *);
void irlap_send_data_secondary_final(struct irlap_cb *, struct sk_buff *);
void irlap_resend_rejected_frames(struct irlap_cb *, int command);
void irlap_resend_rejected_frame(struct irlap_cb *self, int command);

void irlap_send_ui_frame(struct irlap_cb *self, struct sk_buff *skb,
			 __u8 caddr, int command);

extern int irlap_insert_qos_negotiation_params(struct irlap_cb *self, 
					       struct sk_buff *skb);

#endif
