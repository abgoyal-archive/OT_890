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
 * see notice in l1oip.c
 */

/* debugging */
#define DEBUG_L1OIP_INIT	0x00010000
#define DEBUG_L1OIP_SOCKET	0x00020000
#define DEBUG_L1OIP_MGR		0x00040000
#define DEBUG_L1OIP_MSG		0x00080000

/* enable to disorder received bchannels by sequence 2143658798... */
/*
#define REORDER_DEBUG
*/

/* frames */
#define L1OIP_MAX_LEN		2048		/* max packet size form l2 */
#define L1OIP_MAX_PERFRAME	1400		/* max data size in one frame */


/* timers */
#define L1OIP_KEEPALIVE		15
#define L1OIP_TIMEOUT		65


/* socket */
#define L1OIP_DEFAULTPORT	931


/* channel structure */
struct l1oip_chan {
	struct dchannel       	*dch;
	struct bchannel       	*bch;
	u32			tx_counter;	/* counts xmit bytes/packets */
	u32			rx_counter;	/* counts recv bytes/packets */
	u32			codecstate;	/* used by codec to save data */
#ifdef REORDER_DEBUG
	int			disorder_flag;
	struct sk_buff		*disorder_skb;
	u32			disorder_cnt;
#endif
};


/* card structure */
struct l1oip {
	struct list_head        list;

	/* card */
	int			registered;	/* if registered with mISDN */
	char			name[MISDN_MAX_IDLEN];
	int			idx;		/* card index */
	int			pri;		/* 1=pri, 0=bri */
	int			d_idx;		/* current dchannel number */
	int			b_num;		/* number of bchannels */
	u32			id;		/* id of connection */
	int			ondemand;	/* if transmis. is on demand */
	int			bundle;		/* bundle channels in one frm */
	int			codec;		/* codec to use for transmis. */
	int			limit;		/* limit number of bchannels */

	/* timer */
	struct timer_list 	keep_tl;
	struct timer_list 	timeout_tl;
	int			timeout_on;
	struct work_struct	workq;

	/* socket */
	struct socket 		*socket;	/* if set, socket is created */
	struct completion 	socket_complete;/* completion of sock thread */
	struct task_struct	*socket_thread;
	spinlock_t 		socket_lock;	/* access sock outside thread */
	u32			remoteip;	/* if all set, ip is assigned */
	u16	 		localport;	/* must always be set */
	u16	 		remoteport;	/* must always be set */
	struct sockaddr_in	sin_local;	/* local socket name */
	struct sockaddr_in	sin_remote;	/* remote socket name */
	struct msghdr		sendmsg;	/* ip message to send */
	struct iovec		sendiov;	/* iov for message */

	/* frame */
	struct l1oip_chan	chan[128];	/* channel instances */
};

extern int l1oip_law_to_4bit(u8 *data, int len, u8 *result, u32 *state);
extern int l1oip_4bit_to_law(u8 *data, int len, u8 *result);
extern int l1oip_alaw_to_ulaw(u8 *data, int len, u8 *result);
extern int l1oip_ulaw_to_alaw(u8 *data, int len, u8 *result);
extern void l1oip_4bit_free(void);
extern int l1oip_4bit_alloc(int ulaw);

