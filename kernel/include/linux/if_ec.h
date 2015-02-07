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

/* Definitions for Econet sockets. */

#ifndef __LINUX_IF_EC
#define __LINUX_IF_EC

/* User visible stuff. Glibc provides its own but libc5 folk will use these */

struct ec_addr
{
  unsigned char station;		/* Station number.  */
  unsigned char net;			/* Network number.  */
};

struct sockaddr_ec
{
  unsigned short sec_family;
  unsigned char port;			/* Port number.  */
  unsigned char cb;			/* Control/flag byte.  */
  unsigned char type;			/* Type of message.  */
  struct ec_addr addr;
  unsigned long cookie;
};

#define ECTYPE_PACKET_RECEIVED		0	/* Packet received */
#define ECTYPE_TRANSMIT_STATUS		0x10	/* Transmit completed, 
						   low nibble holds status */

#define ECTYPE_TRANSMIT_OK		1
#define ECTYPE_TRANSMIT_NOT_LISTENING	2
#define ECTYPE_TRANSMIT_NET_ERROR	3
#define ECTYPE_TRANSMIT_NO_CLOCK	4
#define ECTYPE_TRANSMIT_LINE_JAMMED	5
#define ECTYPE_TRANSMIT_NOT_PRESENT	6

#ifdef __KERNEL__

#define EC_HLEN				6

/* This is what an Econet frame looks like on the wire. */
struct ec_framehdr 
{
  unsigned char dst_stn;
  unsigned char dst_net;
  unsigned char src_stn;
  unsigned char src_net;
  unsigned char cb;
  unsigned char port;
};

struct econet_sock {
  /* struct sock has to be the first member of econet_sock */
  struct sock	sk;
  unsigned char cb;
  unsigned char port;
  unsigned char station;
  unsigned char net;
  unsigned short num;
};

static inline struct econet_sock *ec_sk(const struct sock *sk)
{
	return (struct econet_sock *)sk;
}

struct ec_device
{
  unsigned char station, net;		/* Econet protocol address */
};

#endif

#endif
