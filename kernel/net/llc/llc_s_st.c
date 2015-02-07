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
 * llc_s_st.c - Defines SAP component state machine transitions.
 *
 * The followed transitions are SAP component state machine transitions
 * which are described in 802.2 LLC protocol standard document.
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
#include <linux/types.h>
#include <net/llc_if.h>
#include <net/llc_s_ev.h>
#include <net/llc_s_ac.h>
#include <net/llc_s_st.h>

/* dummy last-transition indicator; common to all state transition groups
 * last entry for this state
 * all members are zeros, .bss zeroes it
 */
static struct llc_sap_state_trans llc_sap_state_trans_end;

/* state LLC_SAP_STATE_INACTIVE transition for
 * LLC_SAP_EV_ACTIVATION_REQ event
 */
static llc_sap_action_t llc_sap_inactive_state_actions_1[] = {
	[0] = llc_sap_action_report_status,
	[1] = NULL,
};

static struct llc_sap_state_trans llc_sap_inactive_state_trans_1 = {
	.ev =		llc_sap_ev_activation_req,
	.next_state =	LLC_SAP_STATE_ACTIVE,
	.ev_actions =	llc_sap_inactive_state_actions_1,
};

/* array of pointers; one to each transition */
static struct llc_sap_state_trans *llc_sap_inactive_state_transitions[] = {
	[0] = &llc_sap_inactive_state_trans_1,
	[1] = &llc_sap_state_trans_end,
};

/* state LLC_SAP_STATE_ACTIVE transition for LLC_SAP_EV_RX_UI event */
static llc_sap_action_t llc_sap_active_state_actions_1[] = {
	[0] = llc_sap_action_unitdata_ind,
	[1] = NULL,
};

static struct llc_sap_state_trans llc_sap_active_state_trans_1 = {
	.ev =		llc_sap_ev_rx_ui,
	.next_state =	LLC_SAP_STATE_ACTIVE,
	.ev_actions =	llc_sap_active_state_actions_1,
};

/* state LLC_SAP_STATE_ACTIVE transition for LLC_SAP_EV_UNITDATA_REQ event */
static llc_sap_action_t llc_sap_active_state_actions_2[] = {
	[0] = llc_sap_action_send_ui,
	[1] = NULL,
};

static struct llc_sap_state_trans llc_sap_active_state_trans_2 = {
	.ev =		llc_sap_ev_unitdata_req,
	.next_state =	LLC_SAP_STATE_ACTIVE,
	.ev_actions =	llc_sap_active_state_actions_2,
};

/* state LLC_SAP_STATE_ACTIVE transition for LLC_SAP_EV_XID_REQ event */
static llc_sap_action_t llc_sap_active_state_actions_3[] = {
	[0] = llc_sap_action_send_xid_c,
	[1] = NULL,
};

static struct llc_sap_state_trans llc_sap_active_state_trans_3 = {
	.ev =		llc_sap_ev_xid_req,
	.next_state =	LLC_SAP_STATE_ACTIVE,
	.ev_actions =	llc_sap_active_state_actions_3,
};

/* state LLC_SAP_STATE_ACTIVE transition for LLC_SAP_EV_RX_XID_C event */
static llc_sap_action_t llc_sap_active_state_actions_4[] = {
	[0] = llc_sap_action_send_xid_r,
	[1] = NULL,
};

static struct llc_sap_state_trans llc_sap_active_state_trans_4 = {
	.ev =		llc_sap_ev_rx_xid_c,
	.next_state =	LLC_SAP_STATE_ACTIVE,
	.ev_actions =	llc_sap_active_state_actions_4,
};

/* state LLC_SAP_STATE_ACTIVE transition for LLC_SAP_EV_RX_XID_R event */
static llc_sap_action_t llc_sap_active_state_actions_5[] = {
	[0] = llc_sap_action_xid_ind,
	[1] = NULL,
};

static struct llc_sap_state_trans llc_sap_active_state_trans_5 = {
	.ev =		llc_sap_ev_rx_xid_r,
	.next_state =	LLC_SAP_STATE_ACTIVE,
	.ev_actions =	llc_sap_active_state_actions_5,
};

/* state LLC_SAP_STATE_ACTIVE transition for LLC_SAP_EV_TEST_REQ event */
static llc_sap_action_t llc_sap_active_state_actions_6[] = {
	[0] = llc_sap_action_send_test_c,
	[1] = NULL,
};

static struct llc_sap_state_trans llc_sap_active_state_trans_6 = {
	.ev =		llc_sap_ev_test_req,
	.next_state =	LLC_SAP_STATE_ACTIVE,
	.ev_actions =	llc_sap_active_state_actions_6,
};

/* state LLC_SAP_STATE_ACTIVE transition for LLC_SAP_EV_RX_TEST_C event */
static llc_sap_action_t llc_sap_active_state_actions_7[] = {
	[0] = llc_sap_action_send_test_r,
	[1] = NULL,
};

static struct llc_sap_state_trans llc_sap_active_state_trans_7 = {
	.ev =		llc_sap_ev_rx_test_c,
	.next_state =	LLC_SAP_STATE_ACTIVE,
	.ev_actions =	llc_sap_active_state_actions_7
};

/* state LLC_SAP_STATE_ACTIVE transition for LLC_SAP_EV_RX_TEST_R event */
static llc_sap_action_t llc_sap_active_state_actions_8[] = {
	[0] = llc_sap_action_test_ind,
	[1] = NULL,
};

static struct llc_sap_state_trans llc_sap_active_state_trans_8 = {
	.ev =		llc_sap_ev_rx_test_r,
	.next_state =	LLC_SAP_STATE_ACTIVE,
	.ev_actions =	llc_sap_active_state_actions_8,
};

/* state LLC_SAP_STATE_ACTIVE transition for
 * LLC_SAP_EV_DEACTIVATION_REQ event
 */
static llc_sap_action_t llc_sap_active_state_actions_9[] = {
	[0] = llc_sap_action_report_status,
	[1] = NULL,
};

static struct llc_sap_state_trans llc_sap_active_state_trans_9 = {
	.ev =		llc_sap_ev_deactivation_req,
	.next_state =	LLC_SAP_STATE_INACTIVE,
	.ev_actions =	llc_sap_active_state_actions_9
};

/* array of pointers; one to each transition */
static struct llc_sap_state_trans *llc_sap_active_state_transitions[] = {
	[0] = &llc_sap_active_state_trans_2,
	[1] = &llc_sap_active_state_trans_1,
	[2] = &llc_sap_active_state_trans_3,
	[3] = &llc_sap_active_state_trans_4,
	[4] = &llc_sap_active_state_trans_5,
	[5] = &llc_sap_active_state_trans_6,
	[6] = &llc_sap_active_state_trans_7,
	[7] = &llc_sap_active_state_trans_8,
	[8] = &llc_sap_active_state_trans_9,
	[9] = &llc_sap_state_trans_end,
};

/* SAP state transition table */
struct llc_sap_state llc_sap_state_table[LLC_NR_SAP_STATES] = {
	[LLC_SAP_STATE_INACTIVE - 1] = {
		.curr_state	= LLC_SAP_STATE_INACTIVE,
		.transitions	= llc_sap_inactive_state_transitions,
	},
	[LLC_SAP_STATE_ACTIVE - 1] = {
		.curr_state	= LLC_SAP_STATE_ACTIVE,
		.transitions	= llc_sap_active_state_transitions,
	},
};
