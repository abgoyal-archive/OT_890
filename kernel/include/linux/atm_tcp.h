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

/* atm_tcp.h - Driver-specific declarations of the ATMTCP driver (for use by
	       driver-specific utilities) */

/* Written 1997-2000 by Werner Almesberger, EPFL LRC/ICA */


#ifndef LINUX_ATM_TCP_H
#define LINUX_ATM_TCP_H

#include <linux/atmapi.h>
#include <linux/atm.h>
#include <linux/atmioc.h>
#include <linux/types.h>


/*
 * All values in struct atmtcp_hdr are in network byte order
 */

struct atmtcp_hdr {
	__u16	vpi;
	__u16	vci;
	__u32	length;		/* ... of data part */
};

/*
 * All values in struct atmtcp_command are in host byte order
 */

#define ATMTCP_HDR_MAGIC	(~0)	/* this length indicates a command */
#define ATMTCP_CTRL_OPEN	1	/* request/reply */
#define ATMTCP_CTRL_CLOSE	2	/* request/reply */

struct atmtcp_control {
	struct atmtcp_hdr hdr;	/* must be first */
	int type;		/* message type; both directions */
	atm_kptr_t vcc;		/* both directions */
	struct sockaddr_atmpvc addr; /* suggested value from kernel */
	struct atm_qos	qos;	/* both directions */
	int result;		/* to kernel only */
} __ATM_API_ALIGN;

/*
 * Field usage:
 * Messge type	dir.	hdr.v?i	type	addr	qos	vcc	result
 * -----------  ----	------- ----	----	---	---	------
 * OPEN		K->D	Y	Y	Y	Y	Y	0
 * OPEN		D->K	-	Y	Y	Y	Y	Y
 * CLOSE	K->D	-	-	Y	-	Y	0
 * CLOSE	D->K	-	-	-	-	Y	Y
 */

#define SIOCSIFATMTCP	_IO('a',ATMIOC_ITF)	/* set ATMTCP mode */
#define ATMTCP_CREATE	_IO('a',ATMIOC_ITF+14)	/* create persistent ATMTCP
						   interface */
#define ATMTCP_REMOVE	_IO('a',ATMIOC_ITF+15)	/* destroy persistent ATMTCP
						   interface */


#ifdef __KERNEL__

struct atm_tcp_ops {
	int (*attach)(struct atm_vcc *vcc,int itf);
	int (*create_persistent)(int itf);
	int (*remove_persistent)(int itf);
	struct module *owner;
};

extern struct atm_tcp_ops atm_tcp_ops;

#endif

#endif
