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
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#ifndef __UM_NET_USER_H__
#define __UM_NET_USER_H__

#define ETH_ADDR_LEN (6)
#define ETH_HEADER_ETHERTAP (16)
#define ETH_HEADER_OTHER (14)
#define ETH_MAX_PACKET (1500)

#define UML_NET_VERSION (4)

struct net_user_info {
	int (*init)(void *, void *);
	int (*open)(void *);
	void (*close)(int, void *);
	void (*remove)(void *);
	void (*add_address)(unsigned char *, unsigned char *, void *);
	void (*delete_address)(unsigned char *, unsigned char *, void *);
	int max_packet;
	int mtu;
};

extern void ether_user_init(void *data, void *dev);
extern void iter_addresses(void *d, void (*cb)(unsigned char *,
					       unsigned char *, void *),
			   void *arg);

extern void *get_output_buffer(int *len_out);
extern void free_output_buffer(void *buffer);

extern int tap_open_common(void *dev, char *gate_addr);
extern void tap_check_ips(char *gate_addr, unsigned char *eth_addr);

extern void read_output(int fd, char *output_out, int len);

extern int net_read(int fd, void *buf, int len);
extern int net_recvfrom(int fd, void *buf, int len);
extern int net_write(int fd, void *buf, int len);
extern int net_send(int fd, void *buf, int len);
extern int net_sendto(int fd, void *buf, int len, void *to, int sock_len);

extern void open_addr(unsigned char *addr, unsigned char *netmask, void *arg);
extern void close_addr(unsigned char *addr, unsigned char *netmask, void *arg);

extern char *split_if_spec(char *str, ...);

extern int dev_netmask(void *d, void *m);

#endif
