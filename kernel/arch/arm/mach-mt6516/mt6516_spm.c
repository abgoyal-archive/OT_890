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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/* 
 * Copyright (C) 2008,2009 MediaTek <www.mediatek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111EINVAL307  USA
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <mach/mt6516_ap_config.h>
#include <linux/delay.h>
#include <linux/spm.h>

#define PFX "mt6516_spm: "

#if (CONFIG_DEBUG_SPM==1)
#define dbgmsg(msg...) printk(PFX msg)
#else
#define dbgmsg(...)
#endif

#define warnmsg(msg...) printk(KERN_WARNING PFX msg);
#define errmsg(msg...) printk(KERN_WARNING PFX msg);

extern UINT32 SetARM9Freq(ARM9_FREQ_DIV_ENUM ARM9_div);
extern ssize_t	mt6326_VCORE_1_set_1_05(void);
extern ssize_t	mt6326_VCORE_1_set_1_10(void);
extern ssize_t	mt6326_VCORE_1_set_1_3(void); 

const struct spm_perf perf_tbl[] = {
		/* E.g. 104 => 104 MHz, 101 => 1.05 v */
		{{104, 105}, {0, 0}},
		{{208, 110}, {0, 0}},
		{{416, 130}, {0, 0}},
		SPM_PERF_TBL_END
};

static int mt6516_spm_init(void)
{
	int ret;
	ret = spm_set_curr_perf(&perf_tbl[2]);
	return ret;
}

int mt6516_set_cpu_voltage(unsigned int voltage)
{
	if (voltage == 130) {
		mt6326_VCORE_1_set_1_3();	
	}
	else if (voltage == 110) {
		mt6326_VCORE_1_set_1_10();
	}
	else if (voltage == 105) {
		mt6326_VCORE_1_set_1_05();
	}
	else {
		return 0;
	}
	udelay(1000);
	return 0;
}

int mt6516_set_cpu_freq(unsigned int freq)
{
	if (freq == 416) {
		SetARM9Freq(DIV_1_416);
	}
	else if (freq == 208) {
		SetARM9Freq(DIV_2_208);
	}
	else if (freq == 104) {
		SetARM9Freq(DIV_4_104);
	}
	else {
		return 0;
	}
	return 0;
}

static const struct spm_perf* mt6516_get_perf_tbl(void)
{	
	return perf_tbl;
}

static int mt6516_spm_change_perf(const struct spm_perf *curr, const struct spm_perf *next)
{
	char spm_log[100];
	int ret = 0; 
	
	SPM_ASSERT(curr);
	SPM_ASSERT(next);

	if (curr->cpu.freq > next->cpu.freq) {

		mt6516_set_cpu_freq(next->cpu.freq);
		mt6516_set_cpu_voltage(next->cpu.voltage);
	} 
	else if (curr->cpu.freq < next->cpu.freq) {

		mt6516_set_cpu_voltage(next->cpu.voltage);
		mt6516_set_cpu_freq(next->cpu.freq);
	}
	else {
		return 0;
	}
	

	return 0;
}

static int mt6516_spm_suspend(void)
{
	return 0;
}

static int mt6516_spm_resume(void)
{
	return 0;
}

static struct spm_driver mt6516_spm_drv = {
			.name = "MT6516_SPM_DRV",
			.init = mt6516_spm_init,
			.get_perf_tbl = mt6516_get_perf_tbl,
			.change_perf = mt6516_spm_change_perf,
			.suspend = mt6516_spm_suspend,
			.resume = mt6516_spm_resume

		};

SPM_REGISTER_DRIVER(mt6516_spm_drv);
