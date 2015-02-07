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

#ifndef __LINUX_TEXTSEARCH_FSM_H
#define __LINUX_TEXTSEARCH_FSM_H

#include <linux/types.h>

enum {
	TS_FSM_SPECIFIC,	/* specific character */
	TS_FSM_WILDCARD,	/* any character */
	TS_FSM_DIGIT,		/* isdigit() */
	TS_FSM_XDIGIT,		/* isxdigit() */
	TS_FSM_PRINT,		/* isprint() */
	TS_FSM_ALPHA,		/* isalpha() */
	TS_FSM_ALNUM,		/* isalnum() */
	TS_FSM_ASCII,		/* isascii() */
	TS_FSM_CNTRL,		/* iscntrl() */
	TS_FSM_GRAPH,		/* isgraph() */
	TS_FSM_LOWER,		/* islower() */
	TS_FSM_UPPER,		/* isupper() */
	TS_FSM_PUNCT,		/* ispunct() */
	TS_FSM_SPACE,		/* isspace() */
	__TS_FSM_TYPE_MAX,
};
#define TS_FSM_TYPE_MAX (__TS_FSM_TYPE_MAX - 1)

enum {
	TS_FSM_SINGLE,		/* 1 occurrence */
	TS_FSM_PERHAPS,		/* 1 or 0 occurrence */
	TS_FSM_ANY,		/* 0..n occurrences */
	TS_FSM_MULTI,		/* 1..n occurrences */
	TS_FSM_HEAD_IGNORE,	/* 0..n ignored occurrences at head */
	__TS_FSM_RECUR_MAX,
};
#define TS_FSM_RECUR_MAX (__TS_FSM_RECUR_MAX - 1)

/**
 * struct ts_fsm_token - state machine token (state)
 * @type: type of token
 * @recur: number of recurrences
 * @value: character value for TS_FSM_SPECIFIC
 */
struct ts_fsm_token
{
	__u16		type;
	__u8		recur;
	__u8		value;
};

#endif
