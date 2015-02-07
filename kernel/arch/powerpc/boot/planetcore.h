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

#ifndef _PPC_BOOT_PLANETCORE_H_
#define _PPC_BOOT_PLANETCORE_H_

#include "types.h"

#define PLANETCORE_KEY_BOARD_TYPE   "BO"
#define PLANETCORE_KEY_BOARD_REV    "BR"
#define PLANETCORE_KEY_MB_RAM       "D1"
#define PLANETCORE_KEY_MAC_ADDR     "EA"
#define PLANETCORE_KEY_FLASH_SPEED  "FS"
#define PLANETCORE_KEY_IP_ADDR      "IP"
#define PLANETCORE_KEY_KB_NVRAM     "NV"
#define PLANETCORE_KEY_PROCESSOR    "PR"
#define PLANETCORE_KEY_PROC_VARIANT "PV"
#define PLANETCORE_KEY_SERIAL_BAUD  "SB"
#define PLANETCORE_KEY_SERIAL_PORT  "SP"
#define PLANETCORE_KEY_SWITCH       "SW"
#define PLANETCORE_KEY_TEMP_OFFSET  "TC"
#define PLANETCORE_KEY_TARGET_IP    "TIP"
#define PLANETCORE_KEY_CRYSTAL_HZ   "XT"

/* Prepare the table for processing, by turning all newlines
 * into NULL bytes.
 */
void planetcore_prepare_table(char *table);

/* Return the value associated with a given key in text,
 * decimal, or hex format.
 *
 * Returns zero/NULL on failure, non-zero on success.
 */
const char *planetcore_get_key(const char *table, const char *key);
int planetcore_get_decimal(const char *table, const char *key, u64 *val);
int planetcore_get_hex(const char *table, const char *key, u64 *val);

/* Updates the device tree local-mac-address properties based
 * on the EA tag.
 */
void planetcore_set_mac_addrs(const char *table);

/* Sets the linux,stdout-path in the /chosen node.  This requires the
 * linux,planetcore-label property in each serial node.
 */
void planetcore_set_stdout_path(const char *table);

/* Sets the current-speed property in the serial node. */
void planetcore_set_serial_speed(const char *table);

#endif
