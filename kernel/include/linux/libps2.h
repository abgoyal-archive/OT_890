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

#ifndef _LIBPS2_H
#define _LIBPS2_H

/*
 * Copyright (C) 1999-2002 Vojtech Pavlik
 * Copyright (C) 2004 Dmitry Torokhov
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */


#define PS2_CMD_GETID		0x02f2
#define PS2_CMD_RESET_BAT	0x02ff

#define PS2_RET_BAT		0xaa
#define PS2_RET_ID		0x00
#define PS2_RET_ACK		0xfa
#define PS2_RET_NAK		0xfe
#define PS2_RET_ERR		0xfc

#define PS2_FLAG_ACK		1	/* Waiting for ACK/NAK */
#define PS2_FLAG_CMD		2	/* Waiting for command to finish */
#define PS2_FLAG_CMD1		4	/* Waiting for the first byte of command response */
#define PS2_FLAG_WAITID		8	/* Command execiting is GET ID */
#define PS2_FLAG_NAK		16	/* Last transmission was NAKed */

struct ps2dev {
	struct serio *serio;

	/* Ensures that only one command is executing at a time */
	struct mutex cmd_mutex;

	/* Used to signal completion from interrupt handler */
	wait_queue_head_t wait;

	unsigned long flags;
	unsigned char cmdbuf[6];
	unsigned char cmdcnt;
	unsigned char nak;
};

void ps2_init(struct ps2dev *ps2dev, struct serio *serio);
int ps2_sendbyte(struct ps2dev *ps2dev, unsigned char byte, int timeout);
void ps2_drain(struct ps2dev *ps2dev, int maxbytes, int timeout);
int ps2_command(struct ps2dev *ps2dev, unsigned char *param, int command);
int ps2_handle_ack(struct ps2dev *ps2dev, unsigned char data);
int ps2_handle_response(struct ps2dev *ps2dev, unsigned char data);
void ps2_cmd_aborted(struct ps2dev *ps2dev);
int ps2_is_keyboard_id(char id);

#endif /* _LIBPS2_H */
