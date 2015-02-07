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
 * IPWireless 3G PCMCIA Network Driver
 *
 * Original code
 *   by Stephen Blackheath <stephen@blacksapphire.com>,
 *      Ben Martel <benm@symmetric.co.nz>
 *
 * Copyrighted as follows:
 *   Copyright (C) 2004 by Symmetric Systems Ltd (NZ)
 *
 * Various driver changes and rewrites, port to new kernels
 *   Copyright (C) 2006-2007 Jiri Kosina
 *
 * Misc code cleanups and updates
 *   Copyright (C) 2007 David Sterba
 */

#ifndef _IPWIRELESS_CS_SETUP_PROTOCOL_H_
#define _IPWIRELESS_CS_SETUP_PROTOCOL_H_

/* Version of the setup protocol and transport protocols */
#define TL_SETUP_VERSION		1

#define TL_SETUP_VERSION_QRY_TMO	1000
#define TL_SETUP_MAX_VERSION_QRY	30

/* Message numbers 0-9 are obsoleted and must not be reused! */
#define TL_SETUP_SIGNO_GET_VERSION_QRY	10
#define TL_SETUP_SIGNO_GET_VERSION_RSP	11
#define TL_SETUP_SIGNO_CONFIG_MSG	12
#define TL_SETUP_SIGNO_CONFIG_DONE_MSG	13
#define TL_SETUP_SIGNO_OPEN_MSG		14
#define TL_SETUP_SIGNO_CLOSE_MSG	15

#define TL_SETUP_SIGNO_INFO_MSG     20
#define TL_SETUP_SIGNO_INFO_MSG_ACK 21

#define TL_SETUP_SIGNO_REBOOT_MSG      22
#define TL_SETUP_SIGNO_REBOOT_MSG_ACK  23

/* Synchronous start-messages */
struct tl_setup_get_version_qry {
	unsigned char sig_no;		/* TL_SETUP_SIGNO_GET_VERSION_QRY */
} __attribute__ ((__packed__));

struct tl_setup_get_version_rsp {
	unsigned char sig_no;		/* TL_SETUP_SIGNO_GET_VERSION_RSP */
	unsigned char version;		/* TL_SETUP_VERSION */
} __attribute__ ((__packed__));

struct tl_setup_config_msg {
	unsigned char sig_no;		/* TL_SETUP_SIGNO_CONFIG_MSG */
	unsigned char port_no;
	unsigned char prio_data;
	unsigned char prio_ctrl;
} __attribute__ ((__packed__));

struct tl_setup_config_done_msg {
	unsigned char sig_no;		/* TL_SETUP_SIGNO_CONFIG_DONE_MSG */
} __attribute__ ((__packed__));

/* Asyncronous messages */
struct tl_setup_open_msg {
	unsigned char sig_no;		/* TL_SETUP_SIGNO_OPEN_MSG */
	unsigned char port_no;
} __attribute__ ((__packed__));

struct tl_setup_close_msg {
	unsigned char sig_no;		/* TL_SETUP_SIGNO_CLOSE_MSG */
	unsigned char port_no;
} __attribute__ ((__packed__));

/* Driver type  - for use in tl_setup_info_msg.driver_type */
#define COMM_DRIVER     0
#define NDISWAN_DRIVER  1
#define NDISWAN_DRIVER_MAJOR_VERSION  2
#define NDISWAN_DRIVER_MINOR_VERSION  0

/*
 * It should not matter when this message comes over as we just store the
 * results and send the ACK.
 */
struct tl_setup_info_msg {
	unsigned char sig_no;		/* TL_SETUP_SIGNO_INFO_MSG */
	unsigned char driver_type;
	unsigned char major_version;
	unsigned char minor_version;
} __attribute__ ((__packed__));

struct tl_setup_info_msgAck {
	unsigned char sig_no;		/* TL_SETUP_SIGNO_INFO_MSG_ACK */
} __attribute__ ((__packed__));

struct TlSetupRebootMsgAck {
	unsigned char sig_no;		/* TL_SETUP_SIGNO_REBOOT_MSG_ACK */
} __attribute__ ((__packed__));

/* Define a union of all the msgs that the driver can receive from the card.*/
union ipw_setup_rx_msg {
	unsigned char sig_no;
	struct tl_setup_get_version_rsp version_rsp_msg;
	struct tl_setup_open_msg open_msg;
	struct tl_setup_close_msg close_msg;
	struct tl_setup_info_msg InfoMsg;
	struct tl_setup_info_msgAck info_msg_ack;
} __attribute__ ((__packed__));

#endif				/* _IPWIRELESS_CS_SETUP_PROTOCOL_H_ */
