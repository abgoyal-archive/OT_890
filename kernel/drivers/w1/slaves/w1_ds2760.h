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
 * 1-Wire implementation for the ds2760 chip
 *
 * Copyright © 2004-2005, Szabolcs Gyurko <szabolcs.gyurko@tlt.hu>
 *
 * Use consistent with the GNU GPL is permitted,
 * provided that this copyright notice is
 * preserved in its entirety in all copies and derived works.
 *
 */

#ifndef __w1_ds2760_h__
#define __w1_ds2760_h__

/* Known commands to the DS2760 chip */
#define W1_DS2760_SWAP			0xAA
#define W1_DS2760_READ_DATA		0x69
#define W1_DS2760_WRITE_DATA		0x6C
#define W1_DS2760_COPY_DATA		0x48
#define W1_DS2760_RECALL_DATA		0xB8
#define W1_DS2760_LOCK			0x6A

/* Number of valid register addresses */
#define DS2760_DATA_SIZE		0x40

#define DS2760_PROTECTION_REG		0x00
#define DS2760_STATUS_REG		0x01
#define DS2760_EEPROM_REG		0x07
#define DS2760_SPECIAL_FEATURE_REG	0x08
#define DS2760_VOLTAGE_MSB		0x0c
#define DS2760_VOLTAGE_LSB		0x0d
#define DS2760_CURRENT_MSB		0x0e
#define DS2760_CURRENT_LSB		0x0f
#define DS2760_CURRENT_ACCUM_MSB	0x10
#define DS2760_CURRENT_ACCUM_LSB	0x11
#define DS2760_TEMP_MSB			0x18
#define DS2760_TEMP_LSB			0x19
#define DS2760_EEPROM_BLOCK0		0x20
#define DS2760_ACTIVE_FULL		0x20
#define DS2760_EEPROM_BLOCK1		0x30
#define DS2760_RATED_CAPACITY		0x32
#define DS2760_CURRENT_OFFSET_BIAS	0x33
#define DS2760_ACTIVE_EMPTY		0x3b

extern int w1_ds2760_read(struct device *dev, char *buf, int addr,
			  size_t count);
extern int w1_ds2760_write(struct device *dev, char *buf, int addr,
			   size_t count);

#endif /* !__w1_ds2760_h__ */
