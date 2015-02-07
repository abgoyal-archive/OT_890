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

#ifndef _M68K_MACHDEP_H
#define _M68K_MACHDEP_H

#include <linux/seq_file.h>
#include <linux/interrupt.h>

struct pt_regs;
struct mktime;
struct rtc_time;
struct rtc_pll_info;
struct buffer_head;

extern void (*mach_sched_init) (irq_handler_t handler);
/* machine dependent irq functions */
extern void (*mach_init_IRQ) (void);
extern void (*mach_get_model) (char *model);
extern void (*mach_get_hardware_list) (struct seq_file *m);
/* machine dependent timer functions */
extern unsigned long (*mach_gettimeoffset)(void);
extern int (*mach_hwclk)(int, struct rtc_time*);
extern unsigned int (*mach_get_ss)(void);
extern int (*mach_get_rtc_pll)(struct rtc_pll_info *);
extern int (*mach_set_rtc_pll)(struct rtc_pll_info *);
extern int (*mach_set_clock_mmss)(unsigned long);
extern void (*mach_reset)( void );
extern void (*mach_halt)( void );
extern void (*mach_power_off)( void );
extern unsigned long (*mach_hd_init) (unsigned long, unsigned long);
extern void (*mach_hd_setup)(char *, int *);
extern long mach_max_dma_address;
extern void (*mach_heartbeat) (int);
extern void (*mach_l2_flush) (int);
extern void (*mach_beep) (unsigned int, unsigned int);

#endif /* _M68K_MACHDEP_H */
