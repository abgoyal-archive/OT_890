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
 *******               H O S T   M E M O R Y  M A P
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
6/4/1991   jonb		     Made changes to accommodate Mips R3230 bus
 ***************************************************************************/

#ifndef _parmap_h
#define _parmap_h

typedef struct PARM_MAP PARM_MAP;

struct PARM_MAP {
	u16 phb_ptr;	/* Pointer to the PHB array */
	u16 phb_num_ptr;	/* Ptr to Number of PHB's */
	u16 free_list;	/* Free List pointer */
	u16 free_list_end;	/* Free List End pointer */
	u16 q_free_list_ptr;	/* Ptr to Q_BUF variable */
	u16 unit_id_ptr;	/* Unit Id */
	u16 link_str_ptr;	/* Link Structure Array */
	u16 bootloader_1;	/* 1st Stage Boot Loader */
	u16 bootloader_2;	/* 2nd Stage Boot Loader */
	u16 port_route_map_ptr;	/* Port Route Map */
	u16 route_ptr;		/* Unit Route Map */
	u16 map_present;	/* Route Map present */
	s16 pkt_num;		/* Total number of packets */
	s16 q_num;		/* Total number of Q packets */
	u16 buffers_per_port;	/* Number of buffers per port */
	u16 heap_size;		/* Initial size of heap */
	u16 heap_left;		/* Current Heap left */
	u16 error;		/* Error code */
	u16 tx_max;		/* Max number of tx pkts per phb */
	u16 rx_max;		/* Max number of rx pkts per phb */
	u16 rx_limit;		/* For high / low watermarks */
	s16 links;		/* Links to use */
	s16 timer;		/* Interrupts per second */
	u16 rups;		/* Pointer to the RUPs */
	u16 max_phb;		/* Mostly for debugging */
	u16 living;		/* Just increments!! */
	u16 init_done;		/* Initialisation over */
	u16 booting_link;
	u16 idle_count;		/* Idle time counter */
	u16 busy_count;		/* Busy counter */
	u16 idle_control;	/* Control Idle Process */
	u16 tx_intr;		/* TX interrupt pending */
	u16 rx_intr;		/* RX interrupt pending */
	u16 rup_intr;		/* RUP interrupt pending */
};

#endif

/*********** end of file ***********/
