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
 * CAPI encode/decode prototypes and defines
 *
 * Copyright (C) 1996 Universidade de Lisboa
 * 
 * Written by Pedro Roque Marques (roque@di.fc.ul.pt)
 *
 * This software may be used and distributed according to the terms of 
 * the GNU General Public License, incorporated herein by reference.
 */

#ifndef CAPI_H
#define CAPI_H


#define REQ_CAUSE         0x01
#define REQ_DISPLAY       0x04
#define REQ_USER_TO_USER  0x08 

#define AppInfoMask  REQ_CAUSE|REQ_DISPLAY|REQ_USER_TO_USER 

/* Connection Setup */
extern int capi_conn_req(const char * calledPN, struct sk_buff **buf,
			 int proto);
extern int capi_decode_conn_conf(struct pcbit_chan * chan, struct sk_buff *skb,
				 int *complete); 

extern int capi_decode_conn_ind(struct pcbit_chan * chan, struct sk_buff *skb,
				struct callb_data *info);
extern int capi_conn_resp(struct pcbit_chan* chan, struct sk_buff **skb);

extern int capi_conn_active_req(struct pcbit_chan* chan, struct sk_buff **skb);
extern int capi_decode_conn_actv_conf(struct pcbit_chan * chan, 
				      struct sk_buff *skb);

extern int capi_decode_conn_actv_ind(struct pcbit_chan * chan, 
				     struct sk_buff *skb);
extern int capi_conn_active_resp(struct pcbit_chan* chan, 
				 struct sk_buff **skb);

/* Data */
extern int capi_select_proto_req(struct pcbit_chan *chan, struct sk_buff **skb,
				 int outgoing);
extern int capi_decode_sel_proto_conf(struct pcbit_chan *chan, 
				      struct sk_buff *skb);

extern int capi_activate_transp_req(struct pcbit_chan *chan, 
				    struct sk_buff **skb);
extern int capi_decode_actv_trans_conf(struct pcbit_chan *chan, 
				       struct sk_buff *skb);

extern int capi_tdata_req(struct pcbit_chan* chan, struct sk_buff *skb);
extern int capi_tdata_resp(struct pcbit_chan *chan, struct sk_buff ** skb);

/* Connection Termination */
extern int capi_disc_req(ushort callref, struct sk_buff **skb, u_char cause);

extern int capi_decode_disc_ind(struct pcbit_chan *chan, struct sk_buff *skb);
extern int capi_disc_resp(struct pcbit_chan *chan, struct sk_buff **skb);

#ifdef DEBUG
extern int capi_decode_debug_188(u_char *hdr, ushort hdrlen);
#endif

static inline struct pcbit_chan * 
capi_channel(struct pcbit_dev *dev, struct sk_buff *skb)
{
	ushort callref;

	callref = *((ushort*) skb->data);
	skb_pull(skb, 2);

	if (dev->b1->callref == callref)
		return dev->b1;
	else if (dev->b2->callref == callref)
		return dev->b2;

	return NULL;
}

#endif






