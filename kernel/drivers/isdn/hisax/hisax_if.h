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
 * Interface between low level (hardware) drivers and 
 * HiSax protocol stack
 *
 * Author       Kai Germaschewski
 * Copyright    2001 by Kai Germaschewski  <kai.germaschewski@gmx.de>
 * 
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#ifndef __HISAX_IF_H__
#define __HISAX_IF_H__

#include <linux/skbuff.h>

#define REQUEST		0
#define CONFIRM		1
#define INDICATION	2
#define RESPONSE	3

#define PH_ACTIVATE	0x0100
#define PH_DEACTIVATE	0x0110
#define PH_DATA		0x0120
#define PH_PULL		0x0130
#define PH_DATA_E	0x0140

#define L1_MODE_NULL	0
#define L1_MODE_TRANS	1
#define L1_MODE_HDLC	2
#define L1_MODE_EXTRN	3
#define L1_MODE_HDLC_56K 4
#define L1_MODE_MODEM	7
#define L1_MODE_V32	8
#define L1_MODE_FAX	9

struct hisax_if {
	void *priv; // private to driver
	void (*l1l2)(struct hisax_if *, int pr, void *arg);
	void (*l2l1)(struct hisax_if *, int pr, void *arg);
};

struct hisax_b_if {
	struct hisax_if ifc;

	// private to hisax
	struct BCState *bcs;
};

struct hisax_d_if {
	struct hisax_if ifc;

	// private to hisax
	struct module *owner;
	struct IsdnCardState *cs;
	struct hisax_b_if *b_if[2];
	struct sk_buff_head erq;
	unsigned long ph_state;
};

int hisax_register(struct hisax_d_if *hisax_if, struct hisax_b_if *b_if[],
		   char *name, int protocol);
void hisax_unregister(struct hisax_d_if *hisax_if);

#endif
