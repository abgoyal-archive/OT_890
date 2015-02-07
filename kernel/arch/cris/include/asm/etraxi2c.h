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

/* $Id$ */

#ifndef _LINUX_ETRAXI2C_H
#define _LINUX_ETRAXI2C_H

/* etraxi2c _IOC_TYPE, bits 8 to 15 in ioctl cmd */

#define ETRAXI2C_IOCTYPE 44

/* supported ioctl _IOC_NR's */

/* in write operations, the argument contains both i2c
 * slave, register and value.
 */

#define I2C_WRITEARG(slave, reg, value) (((slave) << 16) | ((reg) << 8) | (value))
#define I2C_READARG(slave, reg) (((slave) << 16) | ((reg) << 8))

#define I2C_ARGSLAVE(arg) ((arg) >> 16)
#define I2C_ARGREG(arg) (((arg) >> 8) & 0xff)
#define I2C_ARGVALUE(arg) ((arg) & 0xff)

#define I2C_WRITEREG 0x1   /* write to an i2c register */
#define I2C_READREG  0x2   /* read from an i2c register */

/*
EXAMPLE usage:

    i2c_arg = I2C_WRITEARG(STA013_WRITE_ADDR, reg, val);
    ioctl(fd, _IO(ETRAXI2C_IOCTYPE, I2C_WRITEREG), i2c_arg);

    i2c_arg = I2C_READARG(STA013_READ_ADDR, reg);
    val = ioctl(fd, _IO(ETRAXI2C_IOCTYPE, I2C_READREG), i2c_arg);

*/
#endif
