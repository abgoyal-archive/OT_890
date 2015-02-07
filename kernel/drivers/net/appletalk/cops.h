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

/*      cops.h: LocalTalk driver for Linux.
 *
 *      Authors:
 *      - Jay Schulist <jschlst@samba.org>
 */

#ifndef __LINUX_COPSLTALK_H
#define __LINUX_COPSLTALK_H

#ifdef __KERNEL__

/* Max LLAP size we will accept. */
#define MAX_LLAP_SIZE		603

/* Tangent */
#define TANG_CARD_STATUS        1
#define TANG_CLEAR_INT          1
#define TANG_RESET              3

#define TANG_TX_READY           1
#define TANG_RX_READY           2

/* Dayna */
#define DAYNA_CMD_DATA          0
#define DAYNA_CLEAR_INT         1
#define DAYNA_CARD_STATUS       2
#define DAYNA_INT_CARD          3
#define DAYNA_RESET             4

#define DAYNA_RX_READY          0
#define DAYNA_TX_READY          1
#define DAYNA_RX_REQUEST        3

/* Same on both card types */
#define COPS_CLEAR_INT  1

/* LAP response codes received from the cards. */
#define LAP_INIT        1       /* Init cmd */
#define LAP_INIT_RSP    2       /* Init response */
#define LAP_WRITE       3       /* Write cmd */
#define DATA_READ       4       /* Data read */
#define LAP_RESPONSE    4       /* Received ALAP frame response */
#define LAP_GETSTAT     5       /* Get LAP and HW status */
#define LAP_RSPSTAT     6       /* Status response */

#endif

/*
 *	Structure to hold the firmware information.
 */
struct ltfirmware
{
        unsigned int length;
        const unsigned char *data;
};

#define DAYNA 1
#define TANGENT 2

#endif
