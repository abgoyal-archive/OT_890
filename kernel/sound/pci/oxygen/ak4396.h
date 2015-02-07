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

#ifndef AK4396_H_INCLUDED
#define AK4396_H_INCLUDED

#define AK4396_WRITE		0x2000

#define AK4396_CONTROL_1	0
#define AK4396_CONTROL_2	1
#define AK4396_CONTROL_3	2
#define AK4396_LCH_ATT		3
#define AK4396_RCH_ATT		4

/* control 1 */
#define AK4396_RSTN		0x01
#define AK4396_DIF_MASK		0x0e
#define AK4396_DIF_16_LSB	0x00
#define AK4396_DIF_20_LSB	0x02
#define AK4396_DIF_24_MSB	0x04
#define AK4396_DIF_24_I2S	0x06
#define AK4396_DIF_24_LSB	0x08
#define AK4396_ACKS		0x80
/* control 2 */
#define AK4396_SMUTE		0x01
#define AK4396_DEM_MASK		0x06
#define AK4396_DEM_441		0x00
#define AK4396_DEM_OFF		0x02
#define AK4396_DEM_48		0x04
#define AK4396_DEM_32		0x06
#define AK4396_DFS_MASK		0x18
#define AK4396_DFS_NORMAL	0x00
#define AK4396_DFS_DOUBLE	0x08
#define AK4396_DFS_QUAD		0x10
#define AK4396_SLOW		0x20
#define AK4396_DZFM		0x40
#define AK4396_DZFE		0x80
/* control 3 */
#define AK4396_DZFB		0x04
#define AK4396_DCKB		0x10
#define AK4396_DCKS		0x20
#define AK4396_DSDM		0x40
#define AK4396_D_P_MASK		0x80
#define AK4396_PCM		0x00
#define AK4396_DSD		0x80

#endif