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

#include <linux/i2c.h>
#include <linux/input-polldev.h>
#include <linux/kthread.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/of_device.h>

enum ams_irq {
	AMS_IRQ_FREEFALL = 0x01,
	AMS_IRQ_SHOCK = 0x02,
	AMS_IRQ_GLOBAL = 0x04,
	AMS_IRQ_ALL =
		AMS_IRQ_FREEFALL |
		AMS_IRQ_SHOCK |
		AMS_IRQ_GLOBAL,
};

struct ams {
	/* Locks */
	spinlock_t irq_lock;
	struct mutex lock;

	/* General properties */
	struct device_node *of_node;
	struct of_device *of_dev;
	char has_device;
	char vflag;
	u32 orient1;
	u32 orient2;

	/* Interrupt worker */
	struct work_struct worker;
	u8 worker_irqs;

	/* Implementation
	 *
	 * Only call these functions with the main lock held.
	 */
	void (*exit)(void);

	void (*get_xyz)(s8 *x, s8 *y, s8 *z);
	u8 (*get_vendor)(void);

	void (*clear_irq)(enum ams_irq reg);

#ifdef CONFIG_SENSORS_AMS_I2C
	/* I2C properties */
	struct i2c_client *i2c_client;
#endif

	/* Joystick emulation */
	struct input_polled_dev *idev;
	__u16 bustype;

	/* calibrated null values */
	int xcalib, ycalib, zcalib;
};

extern struct ams ams_info;

extern void ams_sensors(s8 *x, s8 *y, s8 *z);
extern int ams_sensor_attach(void);

extern int ams_pmu_init(struct device_node *np);
extern int ams_i2c_init(struct device_node *np);

extern int ams_input_init(void);
extern void ams_input_exit(void);
