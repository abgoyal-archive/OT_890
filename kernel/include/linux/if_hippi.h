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
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Global definitions for the HIPPI interface.
 *
 * Version:	@(#)if_hippi.h	1.0.0	05/26/97
 *
 * Author:	Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
 *		Donald Becker, <becker@super.org>
 *		Alan Cox, <alan@lxorguk.ukuu.org.uk>
 *		Steve Whitehouse, <gw7rrm@eeshack3.swan.ac.uk>
 *		Jes Sorensen, <Jes.Sorensen@cern.ch>
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */
 
#ifndef _LINUX_IF_HIPPI_H
#define _LINUX_IF_HIPPI_H

#include <linux/types.h>
#include <asm/byteorder.h>

/*
 *	HIPPI magic constants.
 */

#define HIPPI_ALEN	6		/* Bytes in one HIPPI hw-addr	   */
#define HIPPI_HLEN	sizeof(struct hippi_hdr)
#define HIPPI_ZLEN	0		/* Min. bytes in frame without FCS */
#define HIPPI_DATA_LEN	65280		/* Max. bytes in payload	   */
#define HIPPI_FRAME_LEN	(HIPPI_DATA_LEN + HIPPI_HLEN)
					/* Max. bytes in frame without FCS */

/*
 * Define LLC and SNAP constants.
 */
#define HIPPI_EXTENDED_SAP	0xAA
#define HIPPI_UI_CMD		0x03


/*
 *	Do we need to list some sort of ID's here?
 */

/*
 *	HIPPI statistics collection data. 
 */
 
struct hipnet_statistics
{
	int	rx_packets;		/* total packets received	*/
	int	tx_packets;		/* total packets transmitted	*/
	int	rx_errors;		/* bad packets received		*/
	int	tx_errors;		/* packet transmit problems	*/
	int	rx_dropped;		/* no space in linux buffers	*/
	int	tx_dropped;		/* no space available in linux	*/

	/* detailed rx_errors: */
	int	rx_length_errors;
	int	rx_over_errors;		/* receiver ring buff overflow	*/
	int	rx_crc_errors;		/* recved pkt with crc error	*/
	int	rx_frame_errors;	/* recv'd frame alignment error */
	int	rx_fifo_errors;		/* recv'r fifo overrun		*/
	int	rx_missed_errors;	/* receiver missed packet	*/

	/* detailed tx_errors */
	int	tx_aborted_errors;
	int	tx_carrier_errors;
	int	tx_fifo_errors;
	int	tx_heartbeat_errors;
	int	tx_window_errors;
};


struct hippi_fp_hdr
{
#if 0
	__u8		ulp;				/* must contain 4 */
#if defined (__BIG_ENDIAN_BITFIELD)
	__u8		d1_data_present:1;		/* must be 1 */
	__u8		start_d2_burst_boundary:1;	/* must be zero */
	__u8		reserved:6;			/* must be zero */
#if 0
	__u16		reserved1:5;
	__u16		d1_area_size:8;			/* must be 3 */
	__u16		d2_offset:3;			/* must be zero */
#endif
#elif defined(__LITTLE_ENDIAN_BITFIELD)
	__u8		reserved:6;			/* must be zero */
	__u8	 	start_d2_burst_boundary:1;	/* must be zero */
	__u8		d1_data_present:1;		/* must be 1 */
#if 0
	__u16		d2_offset:3;			/* must be zero */
	__u16		d1_area_size:8;			/* must be 3 */
	__u16		reserved1:5;			/* must be zero */
#endif
#else
#error	"Please fix <asm/byteorder.h>"
#endif
#else
	__be32		fixed;
#endif
	__be32		d2_size;
} __attribute__ ((packed));

struct hippi_le_hdr
{
#if defined (__BIG_ENDIAN_BITFIELD)
	__u8		fc:3;
	__u8		double_wide:1;
	__u8		message_type:4;
#elif defined(__LITTLE_ENDIAN_BITFIELD)
	__u8		message_type:4;
	__u8		double_wide:1;
	__u8		fc:3;
#endif
	__u8		dest_switch_addr[3];
#if defined (__BIG_ENDIAN_BITFIELD)
	__u8		dest_addr_type:4,
			src_addr_type:4;
#elif defined(__LITTLE_ENDIAN_BITFIELD)
	__u8		src_addr_type:4,
			dest_addr_type:4;
#endif
	__u8		src_switch_addr[3];
	__u16		reserved;
	__u8		daddr[HIPPI_ALEN];
	__u16		locally_administered;
	__u8		saddr[HIPPI_ALEN];
} __attribute__ ((packed));

#define HIPPI_OUI_LEN	3
/*
 * Looks like the dsap and ssap fields have been swapped by mistake in
 * RFC 2067 "IP over HIPPI".
 */
struct hippi_snap_hdr
{
	__u8	dsap;			/* always 0xAA */
	__u8	ssap;			/* always 0xAA */
	__u8	ctrl;			/* always 0x03 */
	__u8	oui[HIPPI_OUI_LEN];	/* organizational universal id (zero)*/
	__be16	ethertype;		/* packet type ID field */
} __attribute__ ((packed));

struct hippi_hdr
{
	struct hippi_fp_hdr	fp;
	struct hippi_le_hdr	le;
	struct hippi_snap_hdr	snap;
} __attribute__ ((packed));

#endif	/* _LINUX_IF_HIPPI_H */
