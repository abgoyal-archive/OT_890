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

#ifndef _H8300_GPIO_H
#define _H8300_GPIO_H

#define H8300_GPIO_P1 0
#define H8300_GPIO_P2 1
#define H8300_GPIO_P3 2
#define H8300_GPIO_P4 3
#define H8300_GPIO_P5 4
#define H8300_GPIO_P6 5
#define H8300_GPIO_P7 6
#define H8300_GPIO_P8 7
#define H8300_GPIO_P9 8
#define H8300_GPIO_PA 9
#define H8300_GPIO_PB 10
#define H8300_GPIO_PC 11
#define H8300_GPIO_PD 12
#define H8300_GPIO_PE 13
#define H8300_GPIO_PF 14
#define H8300_GPIO_PG 15
#define H8300_GPIO_PH 16

#define H8300_GPIO_B7 0x80
#define H8300_GPIO_B6 0x40
#define H8300_GPIO_B5 0x20
#define H8300_GPIO_B4 0x10
#define H8300_GPIO_B3 0x08
#define H8300_GPIO_B2 0x04
#define H8300_GPIO_B1 0x02
#define H8300_GPIO_B0 0x01

#define H8300_GPIO_INPUT 0
#define H8300_GPIO_OUTPUT 1

#define H8300_GPIO_RESERVE(port, bits) \
        h8300_reserved_gpio(port, bits)

#define H8300_GPIO_FREE(port, bits) \
        h8300_free_gpio(port, bits)

#define H8300_GPIO_DDR(port, bit, dir) \
        h8300_set_gpio_dir(((port) << 8) | (bit), dir)

#define H8300_GPIO_GETDIR(port, bit) \
        h8300_get_gpio_dir(((port) << 8) | (bit))

extern int h8300_reserved_gpio(int port, int bits);
extern int h8300_free_gpio(int port, int bits);
extern int h8300_set_gpio_dir(int port_bit, int dir);
extern int h8300_get_gpio_dir(int port_bit);
extern int h8300_init_gpio(void);

#endif
