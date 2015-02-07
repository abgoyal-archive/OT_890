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
 * linux/can/bcm.h
 *
 * Definitions for CAN Broadcast Manager (BCM)
 *
 * Author: Oliver Hartkopp <oliver.hartkopp@volkswagen.de>
 * Copyright (c) 2002-2007 Volkswagen Group Electronic Research
 * All rights reserved.
 *
 * Send feedback to <socketcan-users@lists.berlios.de>
 *
 */

#ifndef CAN_BCM_H
#define CAN_BCM_H

#include <linux/types.h>

/**
 * struct bcm_msg_head - head of messages to/from the broadcast manager
 * @opcode:    opcode, see enum below.
 * @flags:     special flags, see below.
 * @count:     number of frames to send before changing interval.
 * @ival1:     interval for the first @count frames.
 * @ival2:     interval for the following frames.
 * @can_id:    CAN ID of frames to be sent or received.
 * @nframes:   number of frames appended to the message head.
 * @frames:    array of CAN frames.
 */
struct bcm_msg_head {
	__u32 opcode;
	__u32 flags;
	__u32 count;
	struct timeval ival1, ival2;
	canid_t can_id;
	__u32 nframes;
	struct can_frame frames[0];
};

enum {
	TX_SETUP = 1,	/* create (cyclic) transmission task */
	TX_DELETE,	/* remove (cyclic) transmission task */
	TX_READ,	/* read properties of (cyclic) transmission task */
	TX_SEND,	/* send one CAN frame */
	RX_SETUP,	/* create RX content filter subscription */
	RX_DELETE,	/* remove RX content filter subscription */
	RX_READ,	/* read properties of RX content filter subscription */
	TX_STATUS,	/* reply to TX_READ request */
	TX_EXPIRED,	/* notification on performed transmissions (count=0) */
	RX_STATUS,	/* reply to RX_READ request */
	RX_TIMEOUT,	/* cyclic message is absent */
	RX_CHANGED	/* updated CAN frame (detected content change) */
};

#define SETTIMER            0x0001
#define STARTTIMER          0x0002
#define TX_COUNTEVT         0x0004
#define TX_ANNOUNCE         0x0008
#define TX_CP_CAN_ID        0x0010
#define RX_FILTER_ID        0x0020
#define RX_CHECK_DLC        0x0040
#define RX_NO_AUTOTIMER     0x0080
#define RX_ANNOUNCE_RESUME  0x0100
#define TX_RESET_MULTI_IDX  0x0200
#define RX_RTR_FRAME        0x0400

#endif /* CAN_BCM_H */
