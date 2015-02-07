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

/****************************************************************************
 *******                                                              *******
 *******                      L I N K
 *******                                                              *******
 ****************************************************************************

 Author  : Ian Nandhra / Jeremy Rolls
 Date    :

 *
 *  (C) 1990 - 2000 Specialix International Ltd., Byfleet, Surrey, UK.
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

 Version : 0.01


                            Mods
 ----------------------------------------------------------------------------
  Date     By                Description
 ----------------------------------------------------------------------------

 ***************************************************************************/

#ifndef _link_h
#define _link_h 1

/*************************************************
 * Define the Link Status stuff
 ************************************************/
/* Boot request stuff */
#define BOOT_REQUEST       ((ushort) 0)	/* Request for a boot */
#define BOOT_ABORT         ((ushort) 1)	/* Abort a boot */
#define BOOT_SEQUENCE      ((ushort) 2)	/* Packet with the number of packets
					   and load address */
#define BOOT_COMPLETED     ((ushort) 3)	/* Boot completed */


struct LPB {
	u16 link_number;	/* Link Number */
	u16 in_ch;	/* Link In Channel */
	u16 out_ch;	/* Link Out Channel */
	u8 attached_serial[4];  /* Attached serial number */
	u8 attached_host_serial[4];
	/* Serial number of Host who
	   booted the other end */
	u16 descheduled;	/* Currently Descheduled */
	u16 state;		/* Current state */
	u16 send_poll;		/* Send a Poll Packet */
	u16 ltt_p;	/* Process Descriptor */
	u16 lrt_p;	/* Process Descriptor */
	u16 lrt_status;		/* Current lrt status */
	u16 ltt_status;		/* Current ltt status */
	u16 timeout;		/* Timeout value */
	u16 topology;		/* Topology bits */
	u16 mon_ltt;
	u16 mon_lrt;
	u16 WaitNoBoot;	/* Secs to hold off booting */
	u16 add_packet_list;	/* Add packets to here */
	u16 remove_packet_list;	/* Send packets from here */

	u16 lrt_fail_chan;	/* Lrt's failure channel */
	u16 ltt_fail_chan;	/* Ltt's failure channel */

	/* RUP structure for HOST to driver communications */
	struct RUP rup;
	struct RUP link_rup;	/* RUP for the link (POLL,
				   topology etc.) */
	u16 attached_link;	/* Number of attached link */
	u16 csum_errors;	/* csum errors */
	u16 num_disconnects;	/* number of disconnects */
	u16 num_sync_rcvd;	/* # sync's received */
	u16 num_sync_rqst;	/* # sync requests */
	u16 num_tx;		/* Num pkts sent */
	u16 num_rx;		/* Num pkts received */
	u16 module_attached;	/* Module tpyes of attached */
	u16 led_timeout;	/* LED timeout */
	u16 first_port;		/* First port to service */
	u16 last_port;		/* Last port to service */
};

#endif

/*********** end of file ***********/
