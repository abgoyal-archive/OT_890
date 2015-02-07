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

#ifndef __PPC_FSL_SOC_H
#define __PPC_FSL_SOC_H
#ifdef __KERNEL__

#include <asm/mmu.h>

extern phys_addr_t get_immrbase(void);
#if defined(CONFIG_CPM2) || defined(CONFIG_QUICC_ENGINE) || defined(CONFIG_8xx)
extern u32 get_brgfreq(void);
extern u32 get_baudrate(void);
#else
static inline u32 get_brgfreq(void) { return -1; }
static inline u32 get_baudrate(void) { return -1; }
#endif
extern u32 fsl_get_sys_freq(void);

struct spi_board_info;
struct device_node;

extern int fsl_spi_init(struct spi_board_info *board_infos,
			unsigned int num_board_infos,
			void (*activate_cs)(u8 cs, u8 polarity),
			void (*deactivate_cs)(u8 cs, u8 polarity));

extern void fsl_rstcr_restart(char *cmd);

#if defined(CONFIG_FB_FSL_DIU) || defined(CONFIG_FB_FSL_DIU_MODULE)
struct platform_diu_data_ops {
	unsigned int (*get_pixel_format) (unsigned int bits_per_pixel,
		int monitor_port);
	void (*set_gamma_table) (int monitor_port, char *gamma_table_base);
	void (*set_monitor_port) (int monitor_port);
	void (*set_pixel_clock) (unsigned int pixclock);
	ssize_t (*show_monitor_port) (int monitor_port, char *buf);
	int (*set_sysfs_monitor_port) (int val);
};

extern struct platform_diu_data_ops diu_ops;
#endif

#endif
#endif
