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
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#define SBIT(state) (1<<state)
#define ALL_STATES  0x03ffffff

#define PROTO_DIS_EURO	0x08

#define L3_DEB_WARN	0x01
#define L3_DEB_PROTERR	0x02
#define L3_DEB_STATE	0x04
#define L3_DEB_CHARGE	0x08
#define L3_DEB_CHECK	0x10
#define L3_DEB_SI	0x20

struct stateentry {
	int state;
	int primitive;
	void (*rout) (struct l3_process *, u8, void *);
};

#define l3_debug(st, fmt, args...) HiSax_putstatus(st->l1.hardware, "l3 ", fmt, ## args)

struct PStack;

void newl3state(struct l3_process *pc, int state);
void L3InitTimer(struct l3_process *pc, struct L3Timer *t);
void L3DelTimer(struct L3Timer *t);
int L3AddTimer(struct L3Timer *t, int millisec, int event);
void StopAllL3Timer(struct l3_process *pc);
struct sk_buff *l3_alloc_skb(int len);
struct l3_process *new_l3_process(struct PStack *st, int cr);
void release_l3_process(struct l3_process *p);
struct l3_process *getl3proc(struct PStack *st, int cr);
void l3_msg(struct PStack *st, int pr, void *arg);
void setstack_dss1(struct PStack *st);
void setstack_ni1(struct PStack *st);
void setstack_1tr6(struct PStack *st);

