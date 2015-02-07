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

/******************************************************************************
 * mt6516_sim2.c - MT6516 Android Linux SIM 2 Card Device Driver
 * 
 * Copyright 2009-2010 MediaTek Co.,Ltd.
 * 
 * DESCRIPTION:
 *     1. Decide the usageof the SIM 2 card at kernel booting stage
 *
 * modification history
 * ----------------------------------------
 * ----------------------------------------
 ******************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/workqueue.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>
#include <mach/mt6516_typedefs.h>
#include <mach/mt6516_ap_config.h>

#define VERSION					        "v 0.1"
#define SIM2_DEVICE	        			"mt6516_sim2"

/******************************************************************************
Export symbol
*****************************************************************************/
int req_sim2_ownership(void)
{
	int read_reg=0;

	//#if defined(CONFIG_MT6516_GEMINI_BOARD)
	read_reg = DRV_Reg32(HW_MISC);
	printk("req_sim2_ownership: %x\n", read_reg);

	DRV_SetReg32(HW_MISC, 0x0080);

	read_reg = DRV_Reg32(HW_MISC);
	printk("req_sim2_ownership: %x\n", read_reg);
	
	if ((read_reg >> 7) & 0x1) {
		printk("req_sim2_ownership: PASS!\n");		
	}
	else {
		printk("req_sim2_ownership: Fail!\n");
		return -1;
	}
	//#endif
	
	return 0;
}

/******************************************************************************
Device driver structure
*****************************************************************************/
static int mt6516_sim2_probe(struct device *dev)
{
	//int ret;

	printk("MediaTek MT6516 SIM2 driver mt6516_sim2_probe, version %s\n", VERSION);

	#if 0
	/* For local test */
	ret = req_sim2_ownership();
	if (ret) {
		printk("MediaTek MT6516 SIM2 : req_sim2_ownership() FAIL!\n");
		return ret;
	}	
	#endif
	
	return 0;
}

static int mt6516_sim2_remove(struct device *dev)
{
	printk("MediaTek MT6516 SIM2 driver mt6516_sim2_remove, version %s\n", VERSION);

	return 0;
}

static int mt6516_sim2_suspend(struct device *dev, pm_message_t state)
{
	printk("MediaTek MT6516 SIM2 driver mt6516_sim2_suspend, version %s\n", VERSION);
	return 0;
}

static int mt6516_sim2_resume(struct device *dev)
{
	printk("MediaTek MT6516 SIM2 driver mt6516_sim2_resume, version %s\n", VERSION);
	return 0;
}

static struct device_driver mt6516_sim2_driver = {
	.name		= SIM2_DEVICE,
	.bus		= &platform_bus_type,
	.probe		= mt6516_sim2_probe,
	.remove		= mt6516_sim2_remove,
	#ifdef CONFIG_PM
	.suspend	= mt6516_sim2_suspend,
	.resume		= mt6516_sim2_resume,
	#endif
};

struct platform_device MT6516_sim2_device = {
		.name				= SIM2_DEVICE,
		.id					= 0,
		.dev				= {
		}
};

/******************************************************************************
 * sim2_mod_init
 * 
 * DESCRIPTION:
 *   Register the SIM2 device driver ! 
 * 
 ******************************************************************************/
static s32 __devinit sim2_mod_init(void)
{	
	s32 ret;

	printk("MediaTek MT6516 SIM2 driver register, version %s.\n", VERSION);

	ret = platform_device_register(&MT6516_sim2_device);
	if (ret) {
		printk("****[MT6516_sim2_device] Unable to device register(%d)\n", ret);
		return ret;
	}
	
	ret = driver_register(&mt6516_sim2_driver);
	if (ret) {
		printk("****[mt6516_sim2_driver] Unable to register driver (%d)\n", ret);
		return ret;
	}

	printk("Done\n");
 
    return 0;
}

/******************************************************************************
 * sim2_mod_exit
 * 
 * DESCRIPTION: 
 *   Free the device driver ! 
 * 
 ******************************************************************************/
static void __exit sim2_mod_exit(void)
{
	printk("MediaTek MT6516 SIM2 driver unregister, version %s\n", VERSION);
	printk("Done\n");
}

module_init(sim2_mod_init);
module_exit(sim2_mod_exit);
EXPORT_SYMBOL(req_sim2_ownership);
MODULE_DESCRIPTION("MT6516 SIM2 Driver");
MODULE_LICENSE("Proprietary");
