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
 * Finite state machine
 *
 * Author       Karsten Keil
 * Copyright    by Karsten Keil      <keil@isdn4linux.de>
 *              by Kai Germaschewski <kai.germaschewski@gmx.de>
 * 
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#ifndef __FSM_H__
#define __FSM_H__

#include <linux/timer.h>

struct FsmInst;

typedef void (* FSMFNPTR)(struct FsmInst *, int, void *);

struct Fsm {
	FSMFNPTR *jumpmatrix;
	int state_count, event_count;
	char **strEvent, **strState;
};

struct FsmInst {
	struct Fsm *fsm;
	int state;
	int debug;
	void *userdata;
	int userint;
	void (*printdebug) (struct FsmInst *, char *, ...);
};

struct FsmNode {
	int state, event;
	void (*routine) (struct FsmInst *, int, void *);
};

struct FsmTimer {
	struct FsmInst *fi;
	struct timer_list tl;
	int event;
	void *arg;
};

int FsmNew(struct Fsm *fsm, struct FsmNode *fnlist, int fncount);
void FsmFree(struct Fsm *fsm);
int FsmEvent(struct FsmInst *fi, int event, void *arg);
void FsmChangeState(struct FsmInst *fi, int newstate);
void FsmInitTimer(struct FsmInst *fi, struct FsmTimer *ft);
int FsmAddTimer(struct FsmTimer *ft, int millisec, int event,
		void *arg, int where);
void FsmRestartTimer(struct FsmTimer *ft, int millisec, int event,
		     void *arg, int where);
void FsmDelTimer(struct FsmTimer *ft, int where);

#endif
