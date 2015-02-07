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
 *  drivers/s390/char/keyboard.h
 *    ebcdic keycode functions for s390 console drivers
 *
 *    Copyright (C) 2003 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com),
 */

#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/keyboard.h>

#define NR_FN_HANDLER	20

struct kbd_data;

typedef void (fn_handler_fn)(struct kbd_data *);

/*
 * FIXME: explain key_maps tricks.
 */

struct kbd_data {
	struct tty_struct *tty;
	unsigned short **key_maps;
	char **func_table;
	fn_handler_fn **fn_handler;
	struct kbdiacruc *accent_table;
	unsigned int accent_table_size;
	unsigned int diacr;
	unsigned short sysrq;
};

struct kbd_data *kbd_alloc(void);
void kbd_free(struct kbd_data *);
void kbd_ascebc(struct kbd_data *, unsigned char *);

void kbd_keycode(struct kbd_data *, unsigned int);
int kbd_ioctl(struct kbd_data *, struct file *, unsigned int, unsigned long);

/*
 * Helper Functions.
 */
static inline void
kbd_put_queue(struct tty_struct *tty, int ch)
{
	tty_insert_flip_char(tty, ch, 0);
	tty_schedule_flip(tty);
}

static inline void
kbd_puts_queue(struct tty_struct *tty, char *cp)
{
	while (*cp)
		tty_insert_flip_char(tty, *cp++, 0);
	tty_schedule_flip(tty);
}
