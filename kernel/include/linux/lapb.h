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
 * These are the public elements of the Linux LAPB module.
 */

#ifndef	LAPB_KERNEL_H
#define	LAPB_KERNEL_H

#define	LAPB_OK			0
#define	LAPB_BADTOKEN		1
#define	LAPB_INVALUE		2
#define	LAPB_CONNECTED		3
#define	LAPB_NOTCONNECTED	4
#define	LAPB_REFUSED		5
#define	LAPB_TIMEDOUT		6
#define	LAPB_NOMEM		7

#define	LAPB_STANDARD		0x00
#define	LAPB_EXTENDED		0x01

#define	LAPB_SLP		0x00
#define	LAPB_MLP		0x02

#define	LAPB_DTE		0x00
#define	LAPB_DCE		0x04

struct lapb_register_struct {
	void (*connect_confirmation)(struct net_device *dev, int reason);
	void (*connect_indication)(struct net_device *dev, int reason);
	void (*disconnect_confirmation)(struct net_device *dev, int reason);
	void (*disconnect_indication)(struct net_device *dev, int reason);
	int  (*data_indication)(struct net_device *dev, struct sk_buff *skb);
	void (*data_transmit)(struct net_device *dev, struct sk_buff *skb);
};

struct lapb_parms_struct {
	unsigned int t1;
	unsigned int t1timer;
	unsigned int t2;
	unsigned int t2timer;
	unsigned int n2;
	unsigned int n2count;
	unsigned int window;
	unsigned int state;
	unsigned int mode;
};

extern int lapb_register(struct net_device *dev, struct lapb_register_struct *callbacks);
extern int lapb_unregister(struct net_device *dev);
extern int lapb_getparms(struct net_device *dev, struct lapb_parms_struct *parms);
extern int lapb_setparms(struct net_device *dev, struct lapb_parms_struct *parms);
extern int lapb_connect_request(struct net_device *dev);
extern int lapb_disconnect_request(struct net_device *dev);
extern int lapb_data_request(struct net_device *dev, struct sk_buff *skb);
extern int lapb_data_received(struct net_device *dev, struct sk_buff *skb);

#endif
