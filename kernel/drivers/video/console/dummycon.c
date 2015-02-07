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
 *  linux/drivers/video/dummycon.c -- A dummy console driver
 *
 *  To be used if there's no other console driver (e.g. for plain VGA text)
 *  available, usually until fbcon takes console over.
 */

#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/console.h>
#include <linux/vt_kern.h>
#include <linux/screen_info.h>
#include <linux/init.h>
#include <linux/module.h>

/*
 *  Dummy console driver
 */

#if defined(__arm__)
#define DUMMY_COLUMNS	screen_info.orig_video_cols
#define DUMMY_ROWS	screen_info.orig_video_lines
#elif defined(__hppa__)
/* set by Kconfig. Use 80x25 for 640x480 and 160x64 for 1280x1024 */
#define DUMMY_COLUMNS	CONFIG_DUMMY_CONSOLE_COLUMNS
#define DUMMY_ROWS	CONFIG_DUMMY_CONSOLE_ROWS
#else
#define DUMMY_COLUMNS	80
#define DUMMY_ROWS	25
#endif

static const char *dummycon_startup(void)
{
    return "dummy device";
}

static void dummycon_init(struct vc_data *vc, int init)
{
    vc->vc_can_do_color = 1;
    if (init) {
	vc->vc_cols = DUMMY_COLUMNS;
	vc->vc_rows = DUMMY_ROWS;
    } else
	vc_resize(vc, DUMMY_COLUMNS, DUMMY_ROWS);
}

static int dummycon_dummy(void)
{
    return 0;
}

#define DUMMY	(void *)dummycon_dummy

/*
 *  The console `switch' structure for the dummy console
 *
 *  Most of the operations are dummies.
 */

const struct consw dummy_con = {
    .owner =		THIS_MODULE,
    .con_startup =	dummycon_startup,
    .con_init =		dummycon_init,
    .con_deinit =	DUMMY,
    .con_clear =	DUMMY,
    .con_putc =		DUMMY,
    .con_putcs =	DUMMY,
    .con_cursor =	DUMMY,
    .con_scroll =	DUMMY,
    .con_bmove =	DUMMY,
    .con_switch =	DUMMY,
    .con_blank =	DUMMY,
    .con_font_set =	DUMMY,
    .con_font_get =	DUMMY,
    .con_font_default =	DUMMY,
    .con_font_copy =	DUMMY,
    .con_set_palette =	DUMMY,
    .con_scrolldelta =	DUMMY,
};
