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

#ifndef _LINUX_X25_ASY_H
#define _LINUX_X25_ASY_H

/* X.25 asy configuration. */
#define SL_NRUNIT	256		/* MAX number of X.25 channels;
					   This can be overridden with
					   insmod -ox25_asy_maxdev=nnn	*/
#define SL_MTU		256	

/* X25 async protocol characters. */
#define X25_END         0x7E		/* indicates end of frame	*/
#define X25_ESC         0x7D		/* indicates byte stuffing	*/
#define X25_ESCAPE(x)	((x)^0x20)
#define X25_UNESCAPE(x)	((x)^0x20)


struct x25_asy {
  int			magic;

  /* Various fields. */
  spinlock_t		lock;
  struct tty_struct	*tty;		/* ptr to TTY structure		*/
  struct net_device	*dev;		/* easy for intr handling	*/

  /* These are pointers to the malloc()ed frame buffers. */
  unsigned char		*rbuff;		/* receiver buffer		*/
  int                   rcount;         /* received chars counter       */
  unsigned char		*xbuff;		/* transmitter buffer		*/
  unsigned char         *xhead;         /* pointer to next byte to XMIT */
  int                   xleft;          /* bytes left in XMIT queue     */

  /* X.25 interface statistics. */
  struct net_device_stats stats;

  int                   buffsize;       /* Max buffers sizes            */

  unsigned long		flags;		/* Flag values/ mode etc	*/
#define SLF_INUSE	0		/* Channel in use               */
#define SLF_ESCAPE	1               /* ESC received                 */
#define SLF_ERROR	2               /* Parity, etc. error           */
#define SLF_OUTWAIT	4		/* Waiting for output		*/
};



#define X25_ASY_MAGIC 0x5303

extern int x25_asy_init(struct net_device *dev);

#endif	/* _LINUX_X25_ASY.H */
