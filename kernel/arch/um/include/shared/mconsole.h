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
 * Copyright (C) 2001 Lennert Buytenhek (buytenh@gnu.org)
 * Copyright (C) 2001 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#ifndef __MCONSOLE_H__
#define __MCONSOLE_H__

#ifndef __KERNEL__
#include <stdint.h>
#define u32 uint32_t
#endif

#include "sysdep/ptrace.h"

#define MCONSOLE_MAGIC (0xcafebabe)
#define MCONSOLE_MAX_DATA (512)
#define MCONSOLE_VERSION 2

struct mconsole_request {
	u32 magic;
	u32 version;
	u32 len;
	char data[MCONSOLE_MAX_DATA];
};

struct mconsole_reply {
	u32 err;
	u32 more;
	u32 len;
	char data[MCONSOLE_MAX_DATA];
};

struct mconsole_notify {
	u32 magic;
	u32 version;
	enum { MCONSOLE_SOCKET, MCONSOLE_PANIC, MCONSOLE_HANG,
	       MCONSOLE_USER_NOTIFY } type;
	u32 len;
	char data[MCONSOLE_MAX_DATA];
};

struct mc_request;

enum mc_context { MCONSOLE_INTR, MCONSOLE_PROC };

struct mconsole_command
{
	char *command;
	void (*handler)(struct mc_request *req);
	enum mc_context context;
};

struct mc_request
{
	int len;
	int as_interrupt;

	int originating_fd;
	unsigned int originlen;
	unsigned char origin[128];			/* sockaddr_un */

	struct mconsole_request request;
	struct mconsole_command *cmd;
	struct uml_pt_regs regs;
};

extern char mconsole_socket_name[];

extern int mconsole_unlink_socket(void);
extern int mconsole_reply_len(struct mc_request *req, const char *reply,
			      int len, int err, int more);
extern int mconsole_reply(struct mc_request *req, const char *str, int err,
			  int more);

extern void mconsole_version(struct mc_request *req);
extern void mconsole_help(struct mc_request *req);
extern void mconsole_halt(struct mc_request *req);
extern void mconsole_reboot(struct mc_request *req);
extern void mconsole_config(struct mc_request *req);
extern void mconsole_remove(struct mc_request *req);
extern void mconsole_sysrq(struct mc_request *req);
extern void mconsole_cad(struct mc_request *req);
extern void mconsole_stop(struct mc_request *req);
extern void mconsole_go(struct mc_request *req);
extern void mconsole_log(struct mc_request *req);
extern void mconsole_proc(struct mc_request *req);
extern void mconsole_stack(struct mc_request *req);

extern int mconsole_get_request(int fd, struct mc_request *req);
extern int mconsole_notify(char *sock_name, int type, const void *data,
			   int len);
extern char *mconsole_notify_socket(void);
extern void lock_notify(void);
extern void unlock_notify(void);

#endif
