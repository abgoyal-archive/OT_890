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

#ifndef LLC_S_AC_H
#define LLC_S_AC_H
/*
 * Copyright (c) 1997 by Procom Technology,Inc.
 * 		 2001 by Arnaldo Carvalho de Melo <acme@conectiva.com.br>
 *
 * This program can be redistributed or modified under the terms of the
 * GNU General Public License as published by the Free Software Foundation.
 * This program is distributed without any warranty or implied warranty
 * of merchantability or fitness for a particular purpose.
 *
 * See the GNU General Public License for more details.
 */
/* SAP component actions */
#define SAP_ACT_UNITDATA_IND	1
#define SAP_ACT_SEND_UI		2
#define SAP_ACT_SEND_XID_C	3
#define SAP_ACT_SEND_XID_R	4
#define SAP_ACT_SEND_TEST_C	5
#define SAP_ACT_SEND_TEST_R	6
#define SAP_ACT_REPORT_STATUS	7
#define SAP_ACT_XID_IND		8
#define SAP_ACT_TEST_IND	9

/* All action functions must look like this */
typedef int (*llc_sap_action_t)(struct llc_sap *sap, struct sk_buff *skb);

extern int llc_sap_action_unitdata_ind(struct llc_sap *sap,
				       struct sk_buff *skb);
extern int llc_sap_action_send_ui(struct llc_sap *sap, struct sk_buff *skb);
extern int llc_sap_action_send_xid_c(struct llc_sap *sap, struct sk_buff *skb);
extern int llc_sap_action_send_xid_r(struct llc_sap *sap, struct sk_buff *skb);
extern int llc_sap_action_send_test_c(struct llc_sap *sap, struct sk_buff *skb);
extern int llc_sap_action_send_test_r(struct llc_sap *sap, struct sk_buff *skb);
extern int llc_sap_action_report_status(struct llc_sap *sap,
					struct sk_buff *skb);
extern int llc_sap_action_xid_ind(struct llc_sap *sap, struct sk_buff *skb);
extern int llc_sap_action_test_ind(struct llc_sap *sap, struct sk_buff *skb);
#endif /* LLC_S_AC_H */
