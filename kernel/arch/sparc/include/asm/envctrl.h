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
 *
 * envctrl.h: Definitions for access to the i2c environment
 *            monitoring on Ultrasparc systems.
 *
 * Copyright (C) 1998  Eddie C. Dost  (ecd@skynet.be)
 * Copyright (C) 2000  Vinh Truong  (vinh.truong@eng.sun.com)
 * VT - Add all ioctl commands and environment status definitions
 * VT - Add application note
 */
#ifndef _SPARC64_ENVCTRL_H
#define _SPARC64_ENVCTRL_H 1

#include <linux/ioctl.h>

/* Application note:
 *
 * The driver supports 4 operations: open(), close(), ioctl(), read()
 * The device name is /dev/envctrl.
 * Below is sample usage:
 *
 *	fd = open("/dev/envtrl", O_RDONLY);
 *	if (ioctl(fd, ENVCTRL_READ_SHUTDOWN_TEMPERATURE, 0) < 0)
 *		printf("error\n");
 *	ret = read(fd, buf, 10);
 *	close(fd);
 *
 * Notice in the case of cpu voltage and temperature, the default is
 * cpu0.  If we need to know the info of cpu1, cpu2, cpu3, we need to
 * pass in cpu number in ioctl() last parameter.  For example, to
 * get the voltage of cpu2:
 *
 *	ioctlbuf[0] = 2;
 *	if (ioctl(fd, ENVCTRL_READ_CPU_VOLTAGE, ioctlbuf) < 0)
 *		printf("error\n");
 *	ret = read(fd, buf, 10);
 *
 * All the return values are in ascii.  So check read return value
 * and do appropriate conversions in your application.
 */

/* IOCTL commands */

/* Note: these commands reflect possible monitor features.
 * Some boards choose to support some of the features only.
 */
#define ENVCTRL_RD_CPU_TEMPERATURE	_IOR('p', 0x40, int)
#define ENVCTRL_RD_CPU_VOLTAGE		_IOR('p', 0x41, int)
#define ENVCTRL_RD_FAN_STATUS		_IOR('p', 0x42, int)
#define ENVCTRL_RD_WARNING_TEMPERATURE	_IOR('p', 0x43, int)
#define ENVCTRL_RD_SHUTDOWN_TEMPERATURE	_IOR('p', 0x44, int)
#define ENVCTRL_RD_VOLTAGE_STATUS	_IOR('p', 0x45, int)
#define ENVCTRL_RD_SCSI_TEMPERATURE	_IOR('p', 0x46, int)
#define ENVCTRL_RD_ETHERNET_TEMPERATURE	_IOR('p', 0x47, int)
#define ENVCTRL_RD_MTHRBD_TEMPERATURE	_IOR('p', 0x48, int)

#define ENVCTRL_RD_GLOBALADDRESS	_IOR('p', 0x49, int)

/* Read return values for a voltage status request. */
#define ENVCTRL_VOLTAGE_POWERSUPPLY_GOOD	0x01
#define ENVCTRL_VOLTAGE_BAD			0x02
#define ENVCTRL_POWERSUPPLY_BAD			0x03
#define ENVCTRL_VOLTAGE_POWERSUPPLY_BAD		0x04

/* Read return values for a fan status request.
 * A failure match means either the fan fails or
 * the fan is not connected.  Some boards have optional
 * connectors to connect extra fans.
 *
 * There are maximum 8 monitor fans.  Some are cpu fans
 * some are system fans.  The mask below only indicates
 * fan by order number.
 * Below is a sample application:
 *
 *	if (ioctl(fd, ENVCTRL_READ_FAN_STATUS, 0) < 0) {
 *		printf("ioctl fan failed\n");
 *	}
 *	if (read(fd, rslt, 1) <= 0) {
 *		printf("error or fan not monitored\n");
 *	} else {
 *		if (rslt[0] == ENVCTRL_ALL_FANS_GOOD) {
 *			printf("all fans good\n");
 *	} else if (rslt[0] == ENVCTRL_ALL_FANS_BAD) {
 *		printf("all fans bad\n");
 *	} else {
 *		if (rslt[0] & ENVCTRL_FAN0_FAILURE_MASK) {
 *			printf("fan 0 failed or not connected\n");
 *	}
 *	......
 */

#define ENVCTRL_ALL_FANS_GOOD			0x00
#define ENVCTRL_FAN0_FAILURE_MASK		0x01
#define ENVCTRL_FAN1_FAILURE_MASK		0x02
#define ENVCTRL_FAN2_FAILURE_MASK		0x04
#define ENVCTRL_FAN3_FAILURE_MASK		0x08
#define ENVCTRL_FAN4_FAILURE_MASK		0x10
#define ENVCTRL_FAN5_FAILURE_MASK		0x20
#define ENVCTRL_FAN6_FAILURE_MASK		0x40
#define ENVCTRL_FAN7_FAILURE_MASK		0x80
#define ENVCTRL_ALL_FANS_BAD 			0xFF

#endif /* !(_SPARC64_ENVCTRL_H) */
