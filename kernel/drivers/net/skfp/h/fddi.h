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

/******************************************************************************
 *
 *	(C)Copyright 1998,1999 SysKonnect,
 *	a business unit of Schneider & Koch & Co. Datensysteme GmbH.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	The information in this file is provided "AS IS" without warranty.
 *
 ******************************************************************************/

#ifndef	_FDDI_
#define _FDDI_

struct fddi_addr {
	u_char	a[6] ;
} ;

#define GROUP_ADDR	0x80		/* MSB in a[0] */

struct fddi_mac {
	struct fddi_addr	mac_dest ;
	struct fddi_addr	mac_source ;
	u_char			mac_info[4478] ;
} ;

#define FDDI_MAC_SIZE	(12)
#define FDDI_RAW_MTU	(4500-5)	/* exl. Pr,SD, ED/FS */
#define FDDI_RAW	(4500)

/*
 * FC values
 */
#define FC_VOID		0x40		/* void frame */
#define FC_TOKEN	0x80		/* token */
#define FC_RES_TOKEN	0xc0		/* restricted token */
#define FC_SMT_INFO	0x41		/* SMT Info frame */
/*
 * FC_SMT_LAN_LOC && FC_SMT_LOC are SK specific !
 */
#define FC_SMT_LAN_LOC	0x42		/* local SMT Info frame */
#define FC_SMT_LOC	0x43		/* local SMT Info frame */
#define FC_SMT_NSA	0x4f		/* SMT NSA frame */
#define FC_MAC		0xc0		/* MAC frame */
#define FC_BEACON	0xc2		/* MAC beacon frame */
#define FC_CLAIM	0xc3		/* MAC claim frame */
#define FC_SYNC_LLC	0xd0		/* sync. LLC frame */
#define FC_ASYNC_LLC	0x50		/* async. LLC frame */
#define FC_SYNC_BIT	0x80		/* sync. bit in FC */

#define FC_LLC_PRIOR	0x07		/* priority bits */

#define BEACON_INFO	0		/* beacon type */
#define DBEACON_INFO	1		/* beacon type DIRECTED */


/*
 * indicator bits
 */
#define C_INDICATOR	(1<<0)
#define A_INDICATOR	(1<<1)
#define E_INDICATOR	(1<<2)
#define I_INDICATOR	(1<<6)		/* SK specific */ 
#define L_INDICATOR	(1<<7)		/* SK specific */

#endif	/* _FDDI_ */
