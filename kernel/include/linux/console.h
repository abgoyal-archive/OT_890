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
 *  linux/include/linux/console.h
 *
 *  Copyright (C) 1993        Hamish Macdonald
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 *
 * Changed:
 * 10-Mar-94: Arno Griffioen: Conversion for vt100 emulator port from PC LINUX
 */

#ifndef _LINUX_CONSOLE_H_
#define _LINUX_CONSOLE_H_ 1

#include <linux/types.h>

struct vc_data;
struct console_font_op;
struct console_font;
struct module;
struct tty_struct;

/*
 * this is what the terminal answers to a ESC-Z or csi0c query.
 */
#define VT100ID "\033[?1;2c"
#define VT102ID "\033[?6c"

struct consw {
	struct module *owner;
	const char *(*con_startup)(void);
	void	(*con_init)(struct vc_data *, int);
	void	(*con_deinit)(struct vc_data *);
	void	(*con_clear)(struct vc_data *, int, int, int, int);
	void	(*con_putc)(struct vc_data *, int, int, int);
	void	(*con_putcs)(struct vc_data *, const unsigned short *, int, int, int);
	void	(*con_cursor)(struct vc_data *, int);
	int	(*con_scroll)(struct vc_data *, int, int, int, int);
	void	(*con_bmove)(struct vc_data *, int, int, int, int, int, int);
	int	(*con_switch)(struct vc_data *);
	int	(*con_blank)(struct vc_data *, int, int);
	int	(*con_font_set)(struct vc_data *, struct console_font *, unsigned);
	int	(*con_font_get)(struct vc_data *, struct console_font *);
	int	(*con_font_default)(struct vc_data *, struct console_font *, char *);
	int	(*con_font_copy)(struct vc_data *, int);
	int     (*con_resize)(struct vc_data *, unsigned int, unsigned int,
			       unsigned int);
	int	(*con_set_palette)(struct vc_data *, unsigned char *);
	int	(*con_scrolldelta)(struct vc_data *, int);
	int	(*con_set_origin)(struct vc_data *);
	void	(*con_save_screen)(struct vc_data *);
	u8	(*con_build_attr)(struct vc_data *, u8, u8, u8, u8, u8, u8);
	void	(*con_invert_region)(struct vc_data *, u16 *, int);
	u16    *(*con_screen_pos)(struct vc_data *, int);
	unsigned long (*con_getxy)(struct vc_data *, unsigned long, int *, int *);
};

extern const struct consw *conswitchp;

extern const struct consw dummy_con;	/* dummy console buffer */
extern const struct consw vga_con;	/* VGA text console */
extern const struct consw newport_con;	/* SGI Newport console  */
extern const struct consw prom_con;	/* SPARC PROM console */

int con_is_bound(const struct consw *csw);
int register_con_driver(const struct consw *csw, int first, int last);
int unregister_con_driver(const struct consw *csw);
int take_over_console(const struct consw *sw, int first, int last, int deflt);
void give_up_console(const struct consw *sw);
/* scroll */
#define SM_UP       (1)
#define SM_DOWN     (2)

/* cursor */
#define CM_DRAW     (1)
#define CM_ERASE    (2)
#define CM_MOVE     (3)

/*
 * The interface for a console, or any other device that wants to capture
 * console messages (printer driver?)
 *
 * If a console driver is marked CON_BOOT then it will be auto-unregistered
 * when the first real console is registered.  This is for early-printk drivers.
 */

#define CON_PRINTBUFFER	(1)
#define CON_CONSDEV	(2) /* Last on the command line */
#define CON_ENABLED	(4)
#define CON_BOOT	(8)
#define CON_ANYTIME	(16) /* Safe to call when cpu is offline */
#define CON_BRL		(32) /* Used for a braille device */

struct console {
	char	name[16];
	void	(*write)(struct console *, const char *, unsigned);
	int	(*read)(struct console *, char *, unsigned);
	struct tty_driver *(*device)(struct console *, int *);
	void	(*unblank)(void);
	int	(*setup)(struct console *, char *);
	int	(*early_setup)(void);
	short	flags;
	short	index;
	int	cflag;
	void	*data;
	struct	 console *next;
};

extern int console_set_on_cmdline;

extern int add_preferred_console(char *name, int idx, char *options);
extern int update_console_cmdline(char *name, int idx, char *name_new, int idx_new, char *options);
extern void register_console(struct console *);
extern int unregister_console(struct console *);
extern struct console *console_drivers;
extern void acquire_console_sem(void);
extern int try_acquire_console_sem(void);
extern void release_console_sem(void);
extern void console_conditional_schedule(void);
extern void console_unblank(void);
extern struct tty_driver *console_device(int *);
extern void console_stop(struct console *);
extern void console_start(struct console *);
extern int is_console_locked(void);
extern int braille_register_console(struct console *, int index,
		char *console_options, char *braille_options);
extern int braille_unregister_console(struct console *);

extern int console_suspend_enabled;

/* Suspend and resume console messages over PM events */
extern void suspend_console(void);
extern void resume_console(void);

int mda_console_init(void);
void prom_con_init(void);

void vcs_make_sysfs(struct tty_struct *tty);
void vcs_remove_sysfs(struct tty_struct *tty);

/* Some debug stub to catch some of the obvious races in the VT code */
#if 1
#define WARN_CONSOLE_UNLOCKED()	WARN_ON(!is_console_locked() && !oops_in_progress)
#else
#define WARN_CONSOLE_UNLOCKED()
#endif

/* VESA Blanking Levels */
#define VESA_NO_BLANKING        0
#define VESA_VSYNC_SUSPEND      1
#define VESA_HSYNC_SUSPEND      2
#define VESA_POWERDOWN          3

#ifdef CONFIG_VGA_CONSOLE
extern bool vgacon_text_force(void);
#endif

#endif /* _LINUX_CONSOLE_H */
