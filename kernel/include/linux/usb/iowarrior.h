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

#ifndef __LINUX_USB_IOWARRIOR_H
#define __LINUX_USB_IOWARRIOR_H

#define CODEMERCS_MAGIC_NUMBER	0xC0	/* like COde Mercenaries */

/* Define the ioctl commands for reading and writing data */
#define IOW_WRITE	_IOW(CODEMERCS_MAGIC_NUMBER, 1, __u8 *)
#define IOW_READ	_IOW(CODEMERCS_MAGIC_NUMBER, 2, __u8 *)

/*
   A struct for available device info which is read
   with the ioctl IOW_GETINFO.
   To be compatible with 2.4 userspace which didn't have an easy way to get
   this information.
*/
struct iowarrior_info {
	/* vendor id : supposed to be USB_VENDOR_ID_CODEMERCS in all cases */
	__u32 vendor;
	/* product id : depends on type of chip (USB_DEVICE_ID_CODEMERCS_X) */
	__u32 product;
	/* the serial number of our chip (if a serial-number is not available
	 * this is empty string) */
	__u8 serial[9];
	/* revision number of the chip */
	__u32 revision;
	/* USB-speed of the device (0=UNKNOWN, 1=LOW, 2=FULL 3=HIGH) */
	__u32 speed;
	/* power consumption of the device in mA */
	__u32 power;
	/* the number of the endpoint */
	__u32 if_num;
	/* size of the data-packets on this interface */
	__u32 report_size;
};

/*
  Get some device-information (product-id , serial-number etc.)
  in order to identify a chip.
*/
#define IOW_GETINFO _IOR(CODEMERCS_MAGIC_NUMBER, 3, struct iowarrior_info)

#endif /* __LINUX_USB_IOWARRIOR_H */
