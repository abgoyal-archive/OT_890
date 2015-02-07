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
 * These are the public elements of the Linux kernel X.25 implementation.
 *
 * 	History
 *	mar/20/00	Daniela Squassoni Disabling/enabling of facilities 
 *					  negotiation.
 *	apr/02/05	Shaun Pereira Selective sub address matching with
 *					call user data
 */

#ifndef	X25_KERNEL_H
#define	X25_KERNEL_H

#include <linux/types.h>

#define	SIOCX25GSUBSCRIP	(SIOCPROTOPRIVATE + 0)
#define	SIOCX25SSUBSCRIP	(SIOCPROTOPRIVATE + 1)
#define	SIOCX25GFACILITIES	(SIOCPROTOPRIVATE + 2)
#define	SIOCX25SFACILITIES	(SIOCPROTOPRIVATE + 3)
#define	SIOCX25GCALLUSERDATA	(SIOCPROTOPRIVATE + 4)
#define	SIOCX25SCALLUSERDATA	(SIOCPROTOPRIVATE + 5)
#define	SIOCX25GCAUSEDIAG	(SIOCPROTOPRIVATE + 6)
#define SIOCX25SCUDMATCHLEN	(SIOCPROTOPRIVATE + 7)
#define SIOCX25CALLACCPTAPPRV   (SIOCPROTOPRIVATE + 8)
#define SIOCX25SENDCALLACCPT    (SIOCPROTOPRIVATE + 9)
#define SIOCX25GDTEFACILITIES (SIOCPROTOPRIVATE + 10)
#define SIOCX25SDTEFACILITIES (SIOCPROTOPRIVATE + 11)

/*
 *	Values for {get,set}sockopt.
 */
#define	X25_QBITINCL		1

/*
 *	X.25 Packet Size values.
 */
#define	X25_PS16		4
#define	X25_PS32		5
#define	X25_PS64		6
#define	X25_PS128		7
#define	X25_PS256		8
#define	X25_PS512		9
#define	X25_PS1024		10
#define	X25_PS2048		11
#define	X25_PS4096		12

/*
 * An X.121 address, it is held as ASCII text, null terminated, up to 15
 * digits and a null terminator.
 */
struct x25_address {
	char x25_addr[16];
};

/*
 *	Linux X.25 Address structure, used for bind, and connect mostly.
 */
struct sockaddr_x25 {
	sa_family_t	   sx25_family;		/* Must be AF_X25 */
	struct x25_address sx25_addr;		/* X.121 Address */
};

/*
 *	DTE/DCE subscription options.
 *
 *      As this is missing lots of options, user should expect major
 *	changes of this structure in 2.5.x which might break compatibilty.
 *      The somewhat ugly dimension 200-sizeof() is needed to maintain
 *	backward compatibility.
 */
struct x25_subscrip_struct {
	char device[200-sizeof(unsigned long)];
	unsigned long	global_facil_mask;	/* 0 to disable negotiation */
	unsigned int	extended;
};

/* values for above global_facil_mask */

#define	X25_MASK_REVERSE	0x01	
#define	X25_MASK_THROUGHPUT	0x02
#define	X25_MASK_PACKET_SIZE	0x04
#define	X25_MASK_WINDOW_SIZE	0x08

#define X25_MASK_CALLING_AE 0x10
#define X25_MASK_CALLED_AE 0x20


/*
 *	Routing table control structure.
 */
struct x25_route_struct {
	struct x25_address address;
	unsigned int	   sigdigits;
	char		   device[200];
};

/*
 *	Facilities structure.
 */
struct x25_facilities {
	unsigned int	winsize_in, winsize_out;
	unsigned int	pacsize_in, pacsize_out;
	unsigned int	throughput;
	unsigned int	reverse;
};

/*
* ITU DTE facilities
* Only the called and calling address
* extension are currently implemented.
* The rest are in place to avoid the struct
* changing size if someone needs them later
*/

struct x25_dte_facilities {
	__u16 delay_cumul;
	__u16 delay_target;
	__u16 delay_max;
	__u8 min_throughput;
	__u8 expedited;
	__u8 calling_len;
	__u8 called_len;
	__u8 calling_ae[20];
	__u8 called_ae[20];
};

/*
 *	Call User Data structure.
 */
struct x25_calluserdata {
	unsigned int	cudlength;
	unsigned char	cuddata[128];
};

/*
 *	Call clearing Cause and Diagnostic structure.
 */
struct x25_causediag {
	unsigned char	cause;
	unsigned char	diagnostic;
};

/*
 *	Further optional call user data match length selection
 */
struct x25_subaddr {
	unsigned int cudmatchlength;
};

#endif
