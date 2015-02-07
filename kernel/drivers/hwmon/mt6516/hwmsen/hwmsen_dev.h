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

/* alps/ALPS_SW/TRUNK/MAIN/alps/kernel/drivers/hwmon/mt6516/hwmsen_dev.h
 *
 * (C) Copyright 2009 
 * MediaTek <www.MediaTek.com>
 *
 * Sensor devices
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef __HWMSEN_DEV_H__ 
#define __HWMSEN_DEV_H__

#include <linux/types.h> 
#include <linux/hwmsensor.h>
/******************************************************************************
 * structure / enumeration / macro / definition
 *****************************************************************************/
struct hwmsen_prop {
    /*the order follows hwmsensor.h*/
    int   evt_max[C_MAX_HWMSEN_EVENT_NUM];  /*the maximum of reported event value*/
    int   evt_min[C_MAX_HWMSEN_EVENT_NUM];  /*the minimum of reported event value*/
    int   num;
};
/*----------------------------------------------------------------------------*/
struct hwmsen_object {
    void *self;
    int (*activate)(void* self, u8 enable); 
    int (*get_data)(void* self, struct hwmsen_data *data);  /*get sensor output value*/
    int (*set_conf)(void* self, struct hwmsen_conf *conf);  /*set configuration*/
    int (*get_prop)(void* self, struct hwmsen_prop *prop);  /*get property*/
    int (*get_cali)(void* self, struct hwmsen_data *data);
    int (*set_cali)(void* self, struct hwmsen_data *data);
};
/*----------------------------------------------------------------------------*/
extern int hwmsen_attach(HWM_SENSOR sensor, struct hwmsen_object *obj);
extern int hwmsen_detach(HWM_SENSOR sensor);
extern int hwmsen_get_conf(HWM_SENSOR sensor, struct hwmsen_conf *conf);
/*----------------------------------------------------------------------------*/
#endif 
