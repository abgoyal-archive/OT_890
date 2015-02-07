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

#ifndef __INCLUDE_MT3351_BATTERY_H
#define __INCLUDE_MT3351_BATTERY_H


#include <linux/ioctl.h>
#include <mach/mt3351_typedefs.h>
#include <mach/mt3351_auxadc_hw.h>
#include <mach/mt3351_auxadc_sw.h>
#include <mach/mt3351_reg_base.h>
#include <mach/mt3351_pdn_sw.h>
#include <mach/mt3351_pmu_hw.h>
#include <mach/mt3351_pmu_sw.h>

/*****************************************************************************
 * LOCAL CONST DEFINATION
 ****************************************************************************/

#define BATTERY_DEVNAME				"battery"
#define BATTERY_MAJOR				241
#define PARAM_COUNT 71
#define ADC_SAMPLE_TIMES 5

/*****************************************************************************
 * Battery IOCTL
 ****************************************************************************/
typedef struct {
	UINT16 u16BatteryVoltage;		/* battery voltage */
	UINT16 u16ChargeCurrent;		/* charge current */
	UINT8 u8ChargeStatus;
	UINT8 u8VoltageSource;
} BATTERY_STATUS_IOCTL;

#define CHARGE_STATE_ACPWR_OFF		0		/* AC power is off   */
#define CHARGE_STATE_FAULT		1		/* Charge fault      */
#define CHARGE_STATE_COMPLETE		2		/* Charging complete */
#define CHARGE_STATE_FAST_CHARGING	3		/* Fast charging     */
#define CHARGE_STATE_TOPPINGUP		4		/* Topping up        */

#define VOLTAGE_SOURCE_BATTERY		0		/* Standard battery  */
#define VOLTAGE_SOURCE_BATTERY_PACK	1		/* AA batter pack    */

#define BATTERY_DRIVER_MAGIC	'B' /* Battery driver magic number */
#define IOR_BATTERY_STATUS	_IOR(BATTERY_DRIVER_MAGIC, 3, BATTERY_STATUS)
#define IO_ENABLE_CHARGING	_IO(BATTERY_DRIVER_MAGIC, 4)
#define IO_DISABLE_CHARGING	_IO(BATTERY_DRIVER_MAGIC, 5)
#define IO_DUMP_POWER	_IO(BATTERY_DRIVER_MAGIC, 6)

/*****************************************************************************
 *  TYPEDEF
 ****************************************************************************/
typedef unsigned int       WORD;

/*****************************************************************************
 *  Customer define
 ****************************************************************************/
//#define BATTERY_NTC_TYPE_A         1          //TSM0A103F34D1RZ
#define BATTERY_NTC_TYPE_B         1            //ERTJ1VR103J

/*****************************************************************************
 *  ADC CALIRATION
 ****************************************************************************/
#define ADC_DEFAULT_SLOPE       1
#define ADC_DEFAULT_OFFSET      0
#define ADC_CALI_RANGE          30
//#define ADC_CALI_DIVISOR        10000
#define ADC_CALI_DIVISOR        1000000


/*****************************************************************************
 *  BATTERY PROTECT STATUS
 ****************************************************************************/
#define	BAT_STATUS_OK		0
#define	BAT_STATUS_FAIL		1
#define RTC_DEBOUNCE_OK         0x80
#define ADC_SAMPLE_TIMES        5
#define MAX_CHARGE_TEMPERATURE  50
#define MIN_CHARGE_TEMPERATURE  0
#define ERR_CHARGE_TEMPERATURE  0xFF


/*****************************************************************************
 *  BATTERY VOLTAGE
 ****************************************************************************/
#define PRE_CHARGE_VOLTAGE                  3200
#define SYSTEM_OFF_VOLTAGE                  3450  
#define CONSTANT_CURRENT_CHARGE_VOLTAGE     4100  
#define CONSTANT_VOLTAGE_CHARGE_VOLTAGE     4200  
#define RECHARGING_VOLTAGE                  4000  
#define CV_DROPDOWN_VOLTAGE                 4000
#define CHARGER_THRESH_HOLD                 4300
#define BATTERY_UVLO_VOLTAGE                2700

/*****************************************************************************
 *  BATTERY TIMER
 ****************************************************************************/
#define MAX_CHARGING_TIME                    8*60*60 // 8hr
#define MAX_PreCC_CHARGING_TIME         1*30*60  // 0.5hr
#define MAX_CV_CHARGING_TIME              3*60*60 // 3hr
#define MUTEX_TIMEOUT                            5000
#define BAT_TASK_PERIOD                         10 // 10sec
#define MAX_CV_CHARGING_CURRENT       50 //50mA

/*****************************************************************************
 *  BATTERY Percentage
 ****************************************************************************/
#define BatteryLevelCount       6
#define BatteryLevel_0_Percent 15
#define BatteryLevel_1_Percent 30
#define BatteryLevel_2_Percent 50
#define BatteryLevel_3_Percent 60
#define BatteryLevel_4_Percent 80
#define BatteryLevel_5_Percent 95
#define Battery_Percent_100    100

#define BatteryLevel_0_Percent_VOLTAGE 3550 
#define BatteryLevel_1_Percent_VOLTAGE 3670
#define BatteryLevel_2_Percent_VOLTAGE 3700
#define BatteryLevel_3_Percent_VOLTAGE 3750
#define BatteryLevel_4_Percent_VOLTAGE 3850
#define BatteryLevel_5_Percent_VOLTAGE 3990


#define charger_OVER_VOL	    1
#define BATTERY_UNDER_VOL		2
#define BATTERY_OVER_TEMP		3


typedef enum {
    CHARGER_UNKNOWN = 0,
    STANDARD_HOST,          // 500mA
    CHARGING_HOST,
    NONSTANDARD_CHARGER,    // 500mA or 1A not sure
    STANDARD_CHARGER,       // 1A
} CHARGER_TYPE;


/* Register definition */

#define EFUSEC_CON              ((volatile P_U32)(EFUSE_BASE+0x0000))
#define EFUSEC_WDATA0       ((volatile P_U32)(EFUSE_BASE+0x0010))
#define EFUSEC_WDATA1       ((volatile P_U32)(EFUSE_BASE+0x0014))
#define EFUSEC_WDATA2       ((volatile P_U32)(EFUSE_BASE+0x0018))
#define EFUSEC_WDATA3       ((volatile P_U32)(EFUSE_BASE+0x001C))
#define EFUSEC_D0                ((volatile P_U32)(EFUSE_BASE+0x0020))
#define EFUSEC_D1                ((volatile P_U32)(EFUSE_BASE+0x0024))
#define EFUSEC_D2                ((volatile P_U32)(EFUSE_BASE+0x0028))
#define EFUSEC_D3                ((volatile P_U32)(EFUSE_BASE+0x002C))
#define EFUSEC_PGMCTR       ((volatile P_U32)(EFUSE_BASE+0x0030))

/*******************************************************************************
 * EFUSE register definition
 *******************************************************************************/
/* EFUSEC_CON */
#define EFUSEC_CON_VLD                      (0x0001)
#define EFUSEC_CON_BUSY                    (0x0002)
#define EFUSEC_CON_READ                    (0x0004)
#define EFUSEC_CON_PGM                      (0x0008)
#define EFUSEC_CON_F52M_EN              (0x0010)
#define EFUSEC_CON_PGM_UPDATAE     (0x0020)

void BAT_thread(UINT16);

#endif /* __INCLUDE_MT3351_BATTERY_H */


