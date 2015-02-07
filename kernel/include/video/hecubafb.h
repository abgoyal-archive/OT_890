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
 * hecubafb.h - definitions for the hecuba framebuffer driver
 *
 * Copyright (C) 2008 by Jaya Kumar
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file COPYING in the main directory of this archive for
 * more details.
 *
 */

#ifndef _LINUX_HECUBAFB_H_
#define _LINUX_HECUBAFB_H_

/* Apollo controller specific defines */
#define APOLLO_START_NEW_IMG	0xA0
#define APOLLO_STOP_IMG_DATA	0xA1
#define APOLLO_DISPLAY_IMG	0xA2
#define APOLLO_ERASE_DISPLAY	0xA3
#define APOLLO_INIT_DISPLAY	0xA4

/* Hecuba interface specific defines */
#define HCB_WUP_BIT	0x01
#define HCB_DS_BIT 	0x02
#define HCB_RW_BIT 	0x04
#define HCB_CD_BIT 	0x08
#define HCB_ACK_BIT 	0x80

/* struct used by hecuba. board specific stuff comes from *board */
struct hecubafb_par {
	struct fb_info *info;
	struct hecuba_board *board;
	void (*send_command)(struct hecubafb_par *, unsigned char);
	void (*send_data)(struct hecubafb_par *, unsigned char);
};

/* board specific routines
board drivers can implement wait_for_ack with interrupts if desired. if
wait_for_ack is called with clear=0, then go to sleep and return when ack
goes hi or if wait_for_ack with clear=1, then return when ack goes lo */
struct hecuba_board {
	struct module *owner;
	void (*remove)(struct hecubafb_par *);
	void (*set_ctl)(struct hecubafb_par *, unsigned char, unsigned char);
	void (*set_data)(struct hecubafb_par *, unsigned char);
	void (*wait_for_ack)(struct hecubafb_par *, int);
	int (*init)(struct hecubafb_par *);
};


#endif
