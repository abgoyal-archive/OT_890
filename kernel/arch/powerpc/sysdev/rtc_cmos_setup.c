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
 * Setup code for PC-style Real-Time Clock.
 *
 * Author: Wade Farnsworth <wfarnsworth@mvista.com>
 *
 * 2007 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */

#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/mc146818rtc.h>

#include <asm/prom.h>

static int  __init add_rtc(void)
{
	struct device_node *np;
	struct platform_device *pd;
	struct resource res[2];
	unsigned int num_res = 1;
	int ret;

	memset(&res, 0, sizeof(res));

	np = of_find_compatible_node(NULL, NULL, "pnpPNP,b00");
	if (!np)
		return -ENODEV;

	ret = of_address_to_resource(np, 0, &res[0]);
	of_node_put(np);
	if (ret)
		return ret;

	/*
	 * RTC_PORT(x) is hardcoded in asm/mc146818rtc.h.  Verify that the
	 * address provided by the device node matches.
	 */
	if (res[0].start != RTC_PORT(0))
		return -EINVAL;

	np = of_find_compatible_node(NULL, NULL, "chrp,iic");
	if (!np)
		np = of_find_compatible_node(NULL, NULL, "pnpPNP,000");
	if (np) {
		of_node_put(np);
		/*
		 * Use a fixed interrupt value of 8 since on PPC if we are
		 * using this its off an i8259 which we ensure has interrupt
		 * numbers 0..15.
		 */
		res[1].start = 8;
		res[1].end = 8;
		res[1].flags = IORESOURCE_IRQ;
		num_res++;
	}

	pd = platform_device_register_simple("rtc_cmos", -1,
					     &res[0], num_res);

	if (IS_ERR(pd))
		return PTR_ERR(pd);

	return 0;
}
fs_initcall(add_rtc);

MODULE_LICENSE("GPL");
