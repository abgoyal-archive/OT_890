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

/* atmarp.h - ATM ARP protocol and kernel-demon interface definitions */
 
/* Written 1995-1999 by Werner Almesberger, EPFL LRC/ICA */
 

#ifndef _LINUX_ATMARP_H
#define _LINUX_ATMARP_H

#include <linux/types.h>
#include <linux/atmapi.h>
#include <linux/atmioc.h>


#define ATMARP_RETRY_DELAY 30		/* request next resolution or forget
					   NAK after 30 sec - should go into
					   atmclip.h */
#define ATMARP_MAX_UNRES_PACKETS 5	/* queue that many packets while
					   waiting for the resolver */


#define ATMARPD_CTRL	_IO('a',ATMIOC_CLIP+1)	/* become atmarpd ctrl sock */
#define ATMARP_MKIP	_IO('a',ATMIOC_CLIP+2)	/* attach socket to IP */
#define ATMARP_SETENTRY	_IO('a',ATMIOC_CLIP+3)	/* fill or hide ARP entry */
#define ATMARP_ENCAP	_IO('a',ATMIOC_CLIP+5)	/* change encapsulation */


enum atmarp_ctrl_type {
	act_invalid,		/* catch uninitialized structures */
	act_need,		/* need address resolution */
	act_up,			/* interface is coming up */
	act_down,		/* interface is going down */
	act_change		/* interface configuration has changed */
};

struct atmarp_ctrl {
	enum atmarp_ctrl_type	type;	/* message type */
	int			itf_num;/* interface number (if present) */
	__be32			ip;	/* IP address (act_need only) */
};

#endif
