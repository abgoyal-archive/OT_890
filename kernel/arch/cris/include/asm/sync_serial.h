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
 * ioctl defines for synchronous serial port driver
 *
 * Copyright (c) 2001-2003 Axis Communications AB
 *
 * Author: Mikael Starvik
 *
 */

#ifndef SYNC_SERIAL_H
#define SYNC_SERIAL_H

#include <linux/ioctl.h>

#define SSP_SPEED      _IOR('S', 0, unsigned int)
#define SSP_MODE       _IOR('S', 1, unsigned int)
#define SSP_FRAME_SYNC _IOR('S', 2, unsigned int)
#define SSP_IPOLARITY  _IOR('S', 3, unsigned int)
#define SSP_OPOLARITY  _IOR('S', 4, unsigned int)
#define SSP_SPI        _IOR('S', 5, unsigned int)
#define SSP_INBUFCHUNK _IOR('S', 6, unsigned int)

/* Values for SSP_SPEED */
#define SSP150        0
#define SSP300        1
#define SSP600        2
#define SSP1200       3
#define SSP2400       4
#define SSP4800       5
#define SSP9600       6
#define SSP19200      7
#define SSP28800      8
#define SSP57600      9
#define SSP115200    10
#define SSP230400    11
#define SSP460800    12
#define SSP921600    13
#define SSP3125000   14
#define CODEC        15

#define FREQ_4MHz   0
#define FREQ_2MHz   1
#define FREQ_1MHz   2
#define FREQ_512kHz 3
#define FREQ_256kHz 4
#define FREQ_128kHz 5
#define FREQ_64kHz  6
#define FREQ_32kHz  7

/* Used by application to set CODEC divider, word rate and frame rate */
#define CODEC_VAL(freq, clk_per_sync, sync_per_frame) (CODEC | (freq << 8) | (clk_per_sync << 16) | (sync_per_frame << 28))

/* Used by driver to extract speed */
#define GET_SPEED(x) (x & 0xff)
#define GET_FREQ(x) ((x & 0xff00) >> 8)
#define GET_WORD_RATE(x) (((x & 0x0fff0000) >> 16) - 1)
#define GET_FRAME_RATE(x) (((x & 0xf0000000) >> 28) - 1)

/* Values for SSP_MODE */
#define MASTER_OUTPUT 0
#define SLAVE_OUTPUT  1
#define MASTER_INPUT  2
#define SLAVE_INPUT   3
#define MASTER_BIDIR  4
#define SLAVE_BIDIR   5

/* Values for SSP_FRAME_SYNC */
#define NORMAL_SYNC                1
#define EARLY_SYNC                 2
#define SECOND_WORD_SYNC     0x40000

#define BIT_SYNC                   4
#define WORD_SYNC                  8
#define EXTENDED_SYNC           0x10

#define SYNC_OFF                0x20
#define SYNC_ON                 0x40
#define WORD_SIZE_8             0x80
#define WORD_SIZE_12           0x100
#define WORD_SIZE_16           0x200
#define WORD_SIZE_24           0x400
#define WORD_SIZE_32           0x800
#define BIT_ORDER_LSB         0x1000
#define BIT_ORDER_MSB         0x2000
#define FLOW_CONTROL_ENABLE   0x4000
#define FLOW_CONTROL_DISABLE  0x8000
#define CLOCK_GATED          0x10000
#define CLOCK_NOT_GATED      0x20000

/* Values for SSP_IPOLARITY and SSP_OPOLARITY */
#define CLOCK_NORMAL         1
#define CLOCK_INVERT         2
#define CLOCK_INEGEDGE       CLOCK_NORMAL
#define CLOCK_IPOSEDGE       CLOCK_INVERT
#define FRAME_NORMAL         4
#define FRAME_INVERT         8
#define STATUS_NORMAL      0x10
#define STATUS_INVERT      0x20

/* Values for SSP_SPI */
#define SPI_MASTER           0
#define SPI_SLAVE            1

/* Values for SSP_INBUFCHUNK */
/* plain integer with the size of DMA chunks */

#endif
