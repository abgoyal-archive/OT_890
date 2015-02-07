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
 * selection.h
 *
 * Interface between console.c, tty_io.c, vt.c, vc_screen.c and selection.c
 */

#ifndef _LINUX_SELECTION_H_
#define _LINUX_SELECTION_H_

#include <linux/tiocl.h>
#include <linux/vt_buffer.h>

struct tty_struct;

extern struct vc_data *sel_cons;
struct tty_struct;

extern void clear_selection(void);
extern int set_selection(const struct tiocl_selection __user *sel, struct tty_struct *tty);
extern int paste_selection(struct tty_struct *tty);
extern int sel_loadlut(char __user *p);
extern int mouse_reporting(void);
extern void mouse_report(struct tty_struct * tty, int butt, int mrx, int mry);

extern int console_blanked;

extern unsigned char color_table[];
extern int default_red[];
extern int default_grn[];
extern int default_blu[];

extern unsigned short *screen_pos(struct vc_data *vc, int w_offset, int viewed);
extern u16 screen_glyph(struct vc_data *vc, int offset);
extern void complement_pos(struct vc_data *vc, int offset);
extern void invert_screen(struct vc_data *vc, int offset, int count, int shift);

extern void getconsxy(struct vc_data *vc, unsigned char *p);
extern void putconsxy(struct vc_data *vc, unsigned char *p);

extern u16 vcs_scr_readw(struct vc_data *vc, const u16 *org);
extern void vcs_scr_writew(struct vc_data *vc, u16 val, u16 *org);

#endif
