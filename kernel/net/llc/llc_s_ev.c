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
 * llc_s_ev.c - Defines SAP component events
 *
 * The followed event functions are SAP component events which are described
 * in 802.2 LLC protocol standard document.
 *
 * Copyright (c) 1997 by Procom Technology, Inc.
 *		 2001-2003 by Arnaldo Carvalho de Melo <acme@conectiva.com.br>
 *
 * This program can be redistributed or modified under the terms of the
 * GNU General Public License as published by the Free Software Foundation.
 * This program is distributed without any warranty or implied warranty
 * of merchantability or fitness for a particular purpose.
 *
 * See the GNU General Public License for more details.
 */
#include <linux/socket.h>
#include <net/sock.h>
#include <net/llc_if.h>
#include <net/llc_s_ev.h>
#include <net/llc_pdu.h>

int llc_sap_ev_activation_req(struct llc_sap *sap, struct sk_buff *skb)
{
	struct llc_sap_state_ev *ev = llc_sap_ev(skb);

	return ev->type == LLC_SAP_EV_TYPE_SIMPLE &&
	       ev->prim_type == LLC_SAP_EV_ACTIVATION_REQ ? 0 : 1;
}

int llc_sap_ev_rx_ui(struct llc_sap *sap, struct sk_buff *skb)
{
	struct llc_sap_state_ev *ev = llc_sap_ev(skb);
	struct llc_pdu_un *pdu = llc_pdu_un_hdr(skb);

	return ev->type == LLC_SAP_EV_TYPE_PDU && LLC_PDU_IS_CMD(pdu) &&
	       LLC_PDU_TYPE_IS_U(pdu) &&
	       LLC_U_PDU_CMD(pdu) == LLC_1_PDU_CMD_UI ? 0 : 1;
}

int llc_sap_ev_unitdata_req(struct llc_sap *sap, struct sk_buff *skb)
{
	struct llc_sap_state_ev *ev = llc_sap_ev(skb);

	return ev->type == LLC_SAP_EV_TYPE_PRIM &&
	       ev->prim == LLC_DATAUNIT_PRIM &&
	       ev->prim_type == LLC_PRIM_TYPE_REQ ? 0 : 1;

}

int llc_sap_ev_xid_req(struct llc_sap *sap, struct sk_buff *skb)
{
	struct llc_sap_state_ev *ev = llc_sap_ev(skb);

	return ev->type == LLC_SAP_EV_TYPE_PRIM &&
	       ev->prim == LLC_XID_PRIM &&
	       ev->prim_type == LLC_PRIM_TYPE_REQ ? 0 : 1;
}

int llc_sap_ev_rx_xid_c(struct llc_sap *sap, struct sk_buff *skb)
{
	struct llc_sap_state_ev *ev = llc_sap_ev(skb);
	struct llc_pdu_un *pdu = llc_pdu_un_hdr(skb);

	return ev->type == LLC_SAP_EV_TYPE_PDU && LLC_PDU_IS_CMD(pdu) &&
	       LLC_PDU_TYPE_IS_U(pdu) &&
	       LLC_U_PDU_CMD(pdu) == LLC_1_PDU_CMD_XID ? 0 : 1;
}

int llc_sap_ev_rx_xid_r(struct llc_sap *sap, struct sk_buff *skb)
{
	struct llc_sap_state_ev *ev = llc_sap_ev(skb);
	struct llc_pdu_un *pdu = llc_pdu_un_hdr(skb);

	return ev->type == LLC_SAP_EV_TYPE_PDU && LLC_PDU_IS_RSP(pdu) &&
	       LLC_PDU_TYPE_IS_U(pdu) &&
	       LLC_U_PDU_RSP(pdu) == LLC_1_PDU_CMD_XID ? 0 : 1;
}

int llc_sap_ev_test_req(struct llc_sap *sap, struct sk_buff *skb)
{
	struct llc_sap_state_ev *ev = llc_sap_ev(skb);

	return ev->type == LLC_SAP_EV_TYPE_PRIM &&
	       ev->prim == LLC_TEST_PRIM &&
	       ev->prim_type == LLC_PRIM_TYPE_REQ ? 0 : 1;
}

int llc_sap_ev_rx_test_c(struct llc_sap *sap, struct sk_buff *skb)
{
	struct llc_sap_state_ev *ev = llc_sap_ev(skb);
	struct llc_pdu_un *pdu = llc_pdu_un_hdr(skb);

	return ev->type == LLC_SAP_EV_TYPE_PDU && LLC_PDU_IS_CMD(pdu) &&
	       LLC_PDU_TYPE_IS_U(pdu) &&
	       LLC_U_PDU_CMD(pdu) == LLC_1_PDU_CMD_TEST ? 0 : 1;
}

int llc_sap_ev_rx_test_r(struct llc_sap *sap, struct sk_buff *skb)
{
	struct llc_sap_state_ev *ev = llc_sap_ev(skb);
	struct llc_pdu_un *pdu = llc_pdu_un_hdr(skb);

	return ev->type == LLC_SAP_EV_TYPE_PDU && LLC_PDU_IS_RSP(pdu) &&
	       LLC_PDU_TYPE_IS_U(pdu) &&
	       LLC_U_PDU_RSP(pdu) == LLC_1_PDU_CMD_TEST ? 0 : 1;
}

int llc_sap_ev_deactivation_req(struct llc_sap *sap, struct sk_buff *skb)
{
	struct llc_sap_state_ev *ev = llc_sap_ev(skb);

	return ev->type == LLC_SAP_EV_TYPE_SIMPLE &&
	       ev->prim_type == LLC_SAP_EV_DEACTIVATION_REQ ? 0 : 1;
}
