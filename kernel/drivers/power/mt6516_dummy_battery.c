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

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/******************************************************************************
 * INCLUDE
 *****************************************************************************/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <mach/hardware.h>

#include <linux/delay.h>
#include <linux/wait.h>
#include <linux/interrupt.h>

#include <linux/power_supply.h>

/******************************************************************************
 * EXTERN DECLARATION
 *****************************************************************************/

/******************************************************************************
 * FUNCTION DECLARATIONS
 ******************************************************************************/

/*****************************************************************************
 * LOCAL CONST DEFINATION
 ****************************************************************************/

/******************************************************************************
 * GLOBAL VARIABLE DEFINITIONS
 *****************************************************************************/

/*****************************************************************************
 * FOR ANDROID BATTERY SERVICE
 ****************************************************************************/
struct mt6516_ac_data {
    struct power_supply psy;
};

struct mt6516_battery_data {
    struct power_supply psy;
};

static enum power_supply_property mt6516_ac_props[] = {
	POWER_SUPPLY_PROP_ONLINE,
};

static enum power_supply_property mt6516_battery_props[] = {
	POWER_SUPPLY_PROP_STATUS,
	POWER_SUPPLY_PROP_HEALTH,
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_TECHNOLOGY,
	POWER_SUPPLY_PROP_CAPACITY,
};

static int mt6516_ac_get_property(struct power_supply *psy,
			enum power_supply_property psp,
			union power_supply_propval *val)
{
    int ret = 0;

	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:		
		val->intval = 1;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	return ret;
}

static int mt6516_battery_get_property(struct power_supply *psy,
				 enum power_supply_property psp,
				 union power_supply_propval *val)
{
    int ret = 0; 	
	
	switch (psp) {
	case POWER_SUPPLY_PROP_STATUS:
		val->intval = POWER_SUPPLY_STATUS_FULL;
		break;
	case POWER_SUPPLY_PROP_HEALTH:
		val->intval = POWER_SUPPLY_HEALTH_GOOD;
		break;
	case POWER_SUPPLY_PROP_PRESENT:
		val->intval = 1;
		break;
	case POWER_SUPPLY_PROP_TECHNOLOGY:
		val->intval = POWER_SUPPLY_TECHNOLOGY_LION;
		break;
	case POWER_SUPPLY_PROP_CAPACITY:
		val->intval = 100;
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

// mt6516_ac_data initialization
static struct mt6516_ac_data mt6516_ac_main = {
    .psy = {
        .name = "ac",
        .type = POWER_SUPPLY_TYPE_MAINS,
        .properties = mt6516_ac_props,
        .num_properties = ARRAY_SIZE(mt6516_ac_props),
        .get_property = mt6516_ac_get_property,                
    },
};

// mt6516_battery_data initialization
static struct mt6516_battery_data mt6516_battery_main = {
    .psy = {
        .name = "battery",
        .type = POWER_SUPPLY_TYPE_BATTERY,
        .properties = mt6516_battery_props,
        .num_properties = ARRAY_SIZE(mt6516_battery_props),
        .get_property = mt6516_battery_get_property,                
    },
};


static int __init BAT_mod_init(void)
{
    int ret;

    printk("********* mt6516 BAT_probe *********\n");

    ret = power_supply_register(NULL, &mt6516_ac_main.psy);
    if (ret)
        return ret;
    
    ret = power_supply_register(NULL, &mt6516_battery_main.psy);
    
    return ret;
}

static void __exit BAT_mod_exit(void)
{
    power_supply_unregister(&mt6516_ac_main.psy);
    power_supply_unregister(&mt6516_battery_main.psy);
}

module_init(BAT_mod_init);
module_exit(BAT_mod_exit);

MODULE_AUTHOR("James Lo <James.lo@mediatek.com>");
MODULE_DESCRIPTION("mt6516 Battery Driver");
MODULE_LICENSE("GPL");

