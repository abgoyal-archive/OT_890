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

/* $Id$
 *
 * header for Linux ISDN subsystem, functions for synchronous PPP (linklevel).
 *
 * Copyright 1995,96 by Michael Hipp (Michael.Hipp@student.uni-tuebingen.de)
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#include <linux/ppp_defs.h>     /* for PPP_PROTOCOL */
#include <linux/isdn_ppp.h>	/* for isdn_ppp info */

extern int isdn_ppp_read(int, struct file *, char __user *, int);
extern int isdn_ppp_write(int, struct file *, const char __user *, int);
extern int isdn_ppp_open(int, struct file *);
extern int isdn_ppp_init(void);
extern void isdn_ppp_cleanup(void);
extern int isdn_ppp_free(isdn_net_local *);
extern int isdn_ppp_bind(isdn_net_local *);
extern int isdn_ppp_autodial_filter(struct sk_buff *, isdn_net_local *);
extern int isdn_ppp_xmit(struct sk_buff *, struct net_device *);
extern void isdn_ppp_receive(isdn_net_dev *, isdn_net_local *, struct sk_buff *);
extern int isdn_ppp_dev_ioctl(struct net_device *, struct ifreq *, int);
extern unsigned int isdn_ppp_poll(struct file *, struct poll_table_struct *);
extern int isdn_ppp_ioctl(int, struct file *, unsigned int, unsigned long);
extern void isdn_ppp_release(int, struct file *);
extern int isdn_ppp_dial_slave(char *);
extern void isdn_ppp_wakeup_daemon(isdn_net_local *);

extern int isdn_ppp_register_compressor(struct isdn_ppp_compressor *ipc);
extern int isdn_ppp_unregister_compressor(struct isdn_ppp_compressor *ipc);

#define IPPP_OPEN	0x01
#define IPPP_CONNECT	0x02
#define IPPP_CLOSEWAIT	0x04
#define IPPP_NOBLOCK	0x08
#define IPPP_ASSIGNED	0x10

#define IPPP_MAX_HEADER 10


