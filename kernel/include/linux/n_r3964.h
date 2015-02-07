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

/* r3964 linediscipline for linux
 *
 * -----------------------------------------------------------
 * Copyright by
 * Philips Automation Projects
 * Kassel (Germany)
 * http://www.pap-philips.de
 * -----------------------------------------------------------
 * This software may be used and distributed according to the terms of
 * the GNU General Public License, incorporated herein by reference.
 *
 * Author:
 * L. Haag
 *
 * $Log: r3964.h,v $
 * Revision 1.4  2005/12/21 19:54:24  Kurt Huwig <kurt huwig de>
 * Fixed HZ usage on 2.6 kernels
 * Removed unnecessary include
 *
 * Revision 1.3  2001/03/18 13:02:24  dwmw2
 * Fix timer usage, use spinlocks properly.
 *
 * Revision 1.2  2001/03/18 12:53:15  dwmw2
 * Merge changes in 2.4.2
 *
 * Revision 1.1.1.1  1998/10/13 16:43:14  dwmw2
 * This'll screw the version control
 *
 * Revision 1.6  1998/09/30 00:40:38  dwmw2
 * Updated to use kernel's N_R3964 if available
 *
 * Revision 1.4  1998/04/02 20:29:44  lhaag
 * select, blocking, ...
 *
 * Revision 1.3  1998/02/12 18:58:43  root
 * fixed some memory leaks
 * calculation of checksum characters
 *
 * Revision 1.2  1998/02/07 13:03:17  root
 * ioctl read_telegram
 *
 * Revision 1.1  1998/02/06 19:19:43  root
 * Initial revision
 *
 *
 */

#ifndef __LINUX_N_R3964_H__
#define __LINUX_N_R3964_H__

/* line disciplines for r3964 protocol */

#ifdef __KERNEL__

#include <linux/param.h>

/*
 * Common ascii handshake characters:
 */

#define STX 0x02
#define ETX 0x03
#define DLE 0x10
#define NAK 0x15

/*
 * Timeouts (from milliseconds to jiffies)
 */

#define R3964_TO_QVZ ((550)*HZ/1000)
#define R3964_TO_ZVZ ((220)*HZ/1000)
#define R3964_TO_NO_BUF ((400)*HZ/1000)
#define R3964_NO_TX_ROOM ((100)*HZ/1000)
#define R3964_TO_RX_PANIC ((4000)*HZ/1000)
#define R3964_MAX_RETRIES 5

#endif

/*
 * Ioctl-commands
 */

#define R3964_ENABLE_SIGNALS      0x5301
#define R3964_SETPRIORITY         0x5302
#define R3964_USE_BCC             0x5303
#define R3964_READ_TELEGRAM       0x5304

/* Options for R3964_SETPRIORITY */
#define R3964_MASTER   0
#define R3964_SLAVE    1

/* Options for R3964_ENABLE_SIGNALS */
#define R3964_SIG_ACK   0x0001
#define R3964_SIG_DATA  0x0002
#define R3964_SIG_ALL   0x000f
#define R3964_SIG_NONE  0x0000
#define R3964_USE_SIGIO 0x1000

/*
 * r3964 operation states:
 */
#ifdef __KERNEL__

enum { R3964_IDLE, 
	   R3964_TX_REQUEST, R3964_TRANSMITTING, 
	   R3964_WAIT_ZVZ_BEFORE_TX_RETRY, R3964_WAIT_FOR_TX_ACK,
	   R3964_WAIT_FOR_RX_BUF,
	   R3964_RECEIVING, R3964_WAIT_FOR_BCC, R3964_WAIT_FOR_RX_REPEAT
	   };

/*
 * All open file-handles are 'clients' and are stored in a linked list:
 */

struct r3964_message;

struct r3964_client_info {
	spinlock_t     lock;
	struct pid    *pid;
	unsigned int   sig_flags;

	struct r3964_client_info *next;

	struct r3964_message *first_msg;
	struct r3964_message *last_msg;
	struct r3964_block_header *next_block_to_read;
	int            msg_count;
};


#endif

/* types for msg_id: */
enum {R3964_MSG_ACK=1, R3964_MSG_DATA };

#define R3964_MAX_MSG_COUNT 32

/* error codes for client messages */
#define R3964_OK 0        /* no error. */
#define R3964_TX_FAIL -1  /* transmission error, block NOT sent */
#define R3964_OVERFLOW -2 /* msg queue overflow */

/* the client gets this struct when calling read(fd,...): */
struct r3964_client_message {
	  int     msg_id;
	  int     arg;
	  int     error_code;
};

#define R3964_MTU      256


#ifdef __KERNEL__

struct r3964_block_header;

/* internal version of client_message: */
struct r3964_message {
	  int     msg_id;
	  int     arg;
	  int     error_code;
	  struct r3964_block_header *block;
	  struct r3964_message *next;
};

/*
 * Header of received block in rx_buf/tx_buf:
 */

struct r3964_block_header 
{
	unsigned int length;             /* length in chars without header */
	unsigned char *data;             /* usually data is located 
                                        immediately behind this struct */
	unsigned int locks;              /* only used in rx_buffer */
	  
    struct r3964_block_header *next;
	struct r3964_client_info *owner;  /* =NULL in rx_buffer */
};

/*
 * If rx_buf hasn't enough space to store R3964_MTU chars,
 * we will reject all incoming STX-requests by sending NAK.
 */

#define RX_BUF_SIZE    4000
#define TX_BUF_SIZE    4000
#define R3964_MAX_BLOCKS_IN_RX_QUEUE 100

#define R3964_PARITY 0x0001
#define R3964_FRAME  0x0002
#define R3964_OVERRUN 0x0004
#define R3964_UNKNOWN 0x0008
#define R3964_BREAK   0x0010
#define R3964_CHECKSUM 0x0020
#define R3964_ERROR  0x003f
#define R3964_BCC   0x4000
#define R3964_DEBUG 0x8000


struct r3964_info {
	spinlock_t     lock;
	struct tty_struct *tty;
	unsigned char priority;
	unsigned char *rx_buf;            /* ring buffer */
	unsigned char *tx_buf;

	wait_queue_head_t read_wait;
	//struct wait_queue *read_wait;

	struct r3964_block_header *rx_first;
	struct r3964_block_header *rx_last;
	struct r3964_block_header *tx_first;
	struct r3964_block_header *tx_last;
	unsigned int tx_position;
        unsigned int rx_position;
	unsigned char last_rx;
	unsigned char bcc;
        unsigned int  blocks_in_rx_queue;
	  
	
	struct r3964_client_info *firstClient;
	unsigned int state;
	unsigned int flags;

	struct timer_list tmr;
	int nRetry;
};

#endif	

#endif
