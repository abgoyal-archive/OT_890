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
 * slip.h	Define the SLIP device driver interface and constants.
 *
 * NOTE:	THIS FILE WILL BE MOVED TO THE LINUX INCLUDE DIRECTORY
 *		AS SOON AS POSSIBLE!
 *
 * Version:	@(#)slip.h	1.2.0	03/28/93
 *
 * Fixes:
 *		Alan Cox	: 	Added slip mtu field.
 *		Matt Dillon	:	Printable slip (borrowed from net2e)
 *		Alan Cox	:	Added SL_SLIP_LOTS
 *	Dmitry Gorodchanin	:	A lot of changes in the 'struct slip'
 *	Dmitry Gorodchanin	:	Added CSLIP statistics.
 *	Stanislav Voronyi	:	Make line checking as created by
 *					Igor Chechik, RELCOM Corp.
 *	Craig Schlenter		:	Fixed #define bug that caused
 *					CSLIP telnets to hang in 1.3.61-6
 *
 * Author:	Fred N. van Kempen, <waltje@uwalt.nl.mugnet.org>
 */
#ifndef _LINUX_SLIP_H
#define _LINUX_SLIP_H


#if defined(CONFIG_INET) && defined(CONFIG_SLIP_COMPRESSED)
# define SL_INCLUDE_CSLIP
#endif

#ifdef SL_INCLUDE_CSLIP
# define SL_MODE_DEFAULT SL_MODE_ADAPTIVE
#else
# define SL_MODE_DEFAULT SL_MODE_SLIP
#endif

/* SLIP configuration. */
#define SL_NRUNIT	256		/* MAX number of SLIP channels;
					   This can be overridden with
					   insmod -oslip_maxdev=nnn	*/
#define SL_MTU		296		/* 296; I am used to 600- FvK	*/

/* SLIP protocol characters. */
#define END             0300		/* indicates end of frame	*/
#define ESC             0333		/* indicates byte stuffing	*/
#define ESC_END         0334		/* ESC ESC_END means END 'data'	*/
#define ESC_ESC         0335		/* ESC ESC_ESC means ESC 'data'	*/


struct slip {
  int			magic;

  /* Various fields. */
  struct tty_struct	*tty;		/* ptr to TTY structure		*/
  struct net_device	*dev;		/* easy for intr handling	*/
  spinlock_t		lock;

#ifdef SL_INCLUDE_CSLIP
  struct slcompress	*slcomp;	/* for header compression 	*/
  unsigned char		*cbuff;		/* compression buffer		*/
#endif

  /* These are pointers to the malloc()ed frame buffers. */
  unsigned char		*rbuff;		/* receiver buffer		*/
  int                   rcount;         /* received chars counter       */
  unsigned char		*xbuff;		/* transmitter buffer		*/
  unsigned char         *xhead;         /* pointer to next byte to XMIT */
  int                   xleft;          /* bytes left in XMIT queue     */

  /* SLIP interface statistics. */
  unsigned long		rx_packets;	/* inbound frames counter	*/
  unsigned long         tx_packets;     /* outbound frames counter      */
  unsigned long		rx_bytes;	/* inbound byte counte		*/
  unsigned long         tx_bytes;       /* outbound byte counter	*/
  unsigned long         rx_errors;      /* Parity, etc. errors          */
  unsigned long         tx_errors;      /* Planned stuff                */
  unsigned long         rx_dropped;     /* No memory for skb            */
  unsigned long         tx_dropped;     /* When MTU change              */
  unsigned long         rx_over_errors; /* Frame bigger than SLIP buf.  */
#ifdef SL_INCLUDE_CSLIP
  unsigned long		tx_compressed;
  unsigned long		rx_compressed;
  unsigned long		tx_misses;
#endif
  /* Detailed SLIP statistics. */

  int			mtu;		/* Our mtu (to spot changes!)   */
  int                   buffsize;       /* Max buffers sizes            */

#ifdef CONFIG_SLIP_MODE_SLIP6
  int			xdata, xbits;	/* 6 bit slip controls 		*/
#endif

  unsigned long		flags;		/* Flag values/ mode etc	*/
#define SLF_INUSE	0		/* Channel in use               */
#define SLF_ESCAPE	1               /* ESC received                 */
#define SLF_ERROR	2               /* Parity, etc. error           */
#define SLF_KEEPTEST	3		/* Keepalive test flag		*/
#define SLF_OUTWAIT	4		/* is outpacket was flag	*/

  unsigned char		mode;		/* SLIP mode			*/
  unsigned char		leased;
  dev_t			line;
  pid_t			pid;
#define SL_MODE_SLIP	0
#define SL_MODE_CSLIP	1
#define SL_MODE_SLIP6	2		/* Matt Dillon's printable slip */
#define SL_MODE_CSLIP6	(SL_MODE_SLIP6|SL_MODE_CSLIP)
#define SL_MODE_AX25	4
#define SL_MODE_ADAPTIVE 8
#ifdef CONFIG_SLIP_SMART
  unsigned char		outfill;	/* # of sec between outfill packet */
  unsigned char		keepalive;	/* keepalive seconds		*/
  struct timer_list	outfill_timer;
  struct timer_list	keepalive_timer;
#endif
};

#define SLIP_MAGIC 0x5302

#endif	/* _LINUX_SLIP.H */
