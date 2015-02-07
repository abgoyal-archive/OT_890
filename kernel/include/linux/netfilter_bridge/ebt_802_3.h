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

#ifndef __LINUX_BRIDGE_EBT_802_3_H
#define __LINUX_BRIDGE_EBT_802_3_H

#define EBT_802_3_SAP 0x01
#define EBT_802_3_TYPE 0x02

#define EBT_802_3_MATCH "802_3"

/*
 * If frame has DSAP/SSAP value 0xaa you must check the SNAP type
 * to discover what kind of packet we're carrying. 
 */
#define CHECK_TYPE 0xaa

/*
 * Control field may be one or two bytes.  If the first byte has
 * the value 0x03 then the entire length is one byte, otherwise it is two.
 * One byte controls are used in Unnumbered Information frames.
 * Two byte controls are used in Numbered Information frames.
 */
#define IS_UI 0x03

#define EBT_802_3_MASK (EBT_802_3_SAP | EBT_802_3_TYPE | EBT_802_3)

/* ui has one byte ctrl, ni has two */
struct hdr_ui {
	uint8_t dsap;
	uint8_t ssap;
	uint8_t ctrl;
	uint8_t orig[3];
	__be16 type;
};

struct hdr_ni {
	uint8_t dsap;
	uint8_t ssap;
	__be16 ctrl;
	uint8_t  orig[3];
	__be16 type;
};

struct ebt_802_3_hdr {
	uint8_t  daddr[6];
	uint8_t  saddr[6];
	__be16 len;
	union {
		struct hdr_ui ui;
		struct hdr_ni ni;
	} llc;
};

#ifdef __KERNEL__
#include <linux/skbuff.h>

static inline struct ebt_802_3_hdr *ebt_802_3_hdr(const struct sk_buff *skb)
{
	return (struct ebt_802_3_hdr *)skb_mac_header(skb);
}
#endif

struct ebt_802_3_info 
{
	uint8_t  sap;
	__be16 type;
	uint8_t  bitmask;
	uint8_t  invflags;
};

#endif
