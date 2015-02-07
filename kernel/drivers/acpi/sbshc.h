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

struct acpi_smb_hc;
enum acpi_smb_protocol {
	SMBUS_WRITE_QUICK = 2,
	SMBUS_READ_QUICK = 3,
	SMBUS_SEND_BYTE = 4,
	SMBUS_RECEIVE_BYTE = 5,
	SMBUS_WRITE_BYTE = 6,
	SMBUS_READ_BYTE = 7,
	SMBUS_WRITE_WORD  = 8,
	SMBUS_READ_WORD  = 9,
	SMBUS_WRITE_BLOCK = 0xa,
	SMBUS_READ_BLOCK = 0xb,
	SMBUS_PROCESS_CALL = 0xc,
	SMBUS_BLOCK_PROCESS_CALL = 0xd,
};

static const u8 SMBUS_PEC = 0x80;

enum acpi_sbs_device_addr {
	ACPI_SBS_CHARGER = 0x9,
	ACPI_SBS_MANAGER = 0xa,
	ACPI_SBS_BATTERY = 0xb,
};

typedef void (*smbus_alarm_callback)(void *context);

extern int acpi_smbus_read(struct acpi_smb_hc *hc, u8 protocol, u8 address,
	       u8 command, u8 * data);
extern int acpi_smbus_write(struct acpi_smb_hc *hc, u8 protocol, u8 slave_address,
		u8 command, u8 * data, u8 length);
extern int acpi_smbus_register_callback(struct acpi_smb_hc *hc,
			         smbus_alarm_callback callback, void *context);
extern int acpi_smbus_unregister_callback(struct acpi_smb_hc *hc);
