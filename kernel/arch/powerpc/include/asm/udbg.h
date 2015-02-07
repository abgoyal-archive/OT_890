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
 * (c) 2001, 2006 IBM Corporation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#ifndef _ASM_POWERPC_UDBG_H
#define _ASM_POWERPC_UDBG_H
#ifdef __KERNEL__

#include <linux/compiler.h>
#include <linux/init.h>

extern void (*udbg_putc)(char c);
extern int (*udbg_getc)(void);
extern int (*udbg_getc_poll)(void);

extern void udbg_puts(const char *s);
extern int udbg_write(const char *s, int n);
extern int udbg_read(char *buf, int buflen);

extern void register_early_udbg_console(void);
extern void udbg_printf(const char *fmt, ...)
	__attribute__ ((format (printf, 1, 2)));
extern void udbg_progress(char *s, unsigned short hex);

extern void udbg_init_uart(void __iomem *comport, unsigned int speed,
			   unsigned int clock);
extern unsigned int udbg_probe_uart_speed(void __iomem *comport,
					  unsigned int clock);

struct device_node;
extern void udbg_scc_init(int force_scc);
extern int udbg_adb_init(int force_btext);
extern void udbg_adb_init_early(void);

extern void __init udbg_early_init(void);
extern void __init udbg_init_debug_lpar(void);
extern void __init udbg_init_pmac_realmode(void);
extern void __init udbg_init_maple_realmode(void);
extern void __init udbg_init_pas_realmode(void);
extern void __init udbg_init_iseries(void);
extern void __init udbg_init_rtas_panel(void);
extern void __init udbg_init_rtas_console(void);
extern void __init udbg_init_debug_beat(void);
extern void __init udbg_init_btext(void);
extern void __init udbg_init_44x_as1(void);
extern void __init udbg_init_40x_realmode(void);
extern void __init udbg_init_cpm(void);

#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_UDBG_H */
