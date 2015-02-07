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

#ifndef _LINUX_TIOCL_H
#define _LINUX_TIOCL_H

#define TIOCL_SETSEL	2	/* set a selection */
#define 	TIOCL_SELCHAR	0	/* select characters */
#define 	TIOCL_SELWORD	1	/* select whole words */
#define 	TIOCL_SELLINE	2	/* select whole lines */
#define 	TIOCL_SELPOINTER	3	/* show the pointer */
#define 	TIOCL_SELCLEAR	4	/* clear visibility of selection */
#define 	TIOCL_SELMOUSEREPORT	16	/* report beginning of selection */
#define 	TIOCL_SELBUTTONMASK	15	/* button mask for report */
/* selection extent */
struct tiocl_selection {
	unsigned short xs;	/* X start */
	unsigned short ys;	/* Y start */
	unsigned short xe;	/* X end */
	unsigned short ye;	/* Y end */
	unsigned short sel_mode;	/* selection mode */
};

#define TIOCL_PASTESEL	3	/* paste previous selection */
#define TIOCL_UNBLANKSCREEN	4	/* unblank screen */

#define TIOCL_SELLOADLUT	5
	/* set characters to be considered alphabetic when selecting */
	/* u32[8] bit array, 4 bytes-aligned with type */

/* these two don't return a value: they write it back in the type */
#define TIOCL_GETSHIFTSTATE	6	/* write shift state */
#define TIOCL_GETMOUSEREPORTING	7	/* write whether mouse event are reported */
#define TIOCL_SETVESABLANK	10	/* set vesa blanking mode */
#define TIOCL_SETKMSGREDIRECT	11	/* restrict kernel messages to a vt */
#define TIOCL_GETFGCONSOLE	12	/* get foreground vt */
#define TIOCL_SCROLLCONSOLE	13	/* scroll console */
#define TIOCL_BLANKSCREEN	14	/* keep screen blank even if a key is pressed */
#define TIOCL_BLANKEDSCREEN	15	/* return which vt was blanked */
#define TIOCL_GETKMSGREDIRECT	17	/* get the vt the kernel messages are restricted to */

#endif /* _LINUX_TIOCL_H */
