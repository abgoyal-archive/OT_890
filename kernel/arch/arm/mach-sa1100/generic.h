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
 * linux/arch/arm/mach-sa1100/generic.h
 *
 * Author: Nicolas Pitre
 */

struct sys_timer;

extern struct sys_timer sa1100_timer;
extern void __init sa1100_map_io(void);
extern void __init sa1100_init_irq(void);
extern void __init sa1100_init_gpio(void);

#define SET_BANK(__nr,__start,__size) \
	mi->bank[__nr].start = (__start), \
	mi->bank[__nr].size = (__size), \
	mi->bank[__nr].node = (((unsigned)(__start) - PHYS_OFFSET) >> 27)

extern void (*sa1100fb_backlight_power)(int on);
extern void (*sa1100fb_lcd_power)(int on);

extern void sa1110_mb_enable(void);
extern void sa1110_mb_disable(void);

struct cpufreq_policy;

extern unsigned int sa11x0_freq_to_ppcr(unsigned int khz);
extern int sa11x0_verify_speed(struct cpufreq_policy *policy);
extern unsigned int sa11x0_getspeed(unsigned int cpu);
extern unsigned int sa11x0_ppcr_to_freq(unsigned int idx);

struct flash_platform_data;
struct resource;

extern void sa11x0_set_flash_data(struct flash_platform_data *flash,
				  struct resource *res, int nr);

struct sa11x0_ssp_plat_ops;
extern void sa11x0_set_ssp_data(struct sa11x0_ssp_plat_ops *ops);

struct irda_platform_data;
void sa11x0_set_irda_data(struct irda_platform_data *irda);

struct mcp_plat_data;
void sa11x0_set_mcp_data(struct mcp_plat_data *data);
