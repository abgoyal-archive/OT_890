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
* Common Header File for the Elan Digital Systems U132 adapter
* this file should be included by both the "ftdi-u132" and
* the "u132-hcd" modules.
*
* Copyright(C) 2006 Elan Digital Systems Limited
*(http://www.elandigitalsystems.com)
*
* Author and Maintainer - Tony Olech - Elan Digital Systems
*(tony.olech@elandigitalsystems.com)
*
* This program is free software;you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation, version 2.
*
*
* The driver was written by Tony Olech(tony.olech@elandigitalsystems.com)
* based on various USB client drivers in the 2.6.15 linux kernel
* with constant reference to the 3rd Edition of Linux Device Drivers
* published by O'Reilly
*
* The U132 adapter is a USB to CardBus adapter specifically designed
* for PC cards that contain an OHCI host controller. Typical PC cards
* are the Orange Mobile 3G Option GlobeTrotter Fusion card.
*
* The U132 adapter will *NOT *work with PC cards that do not contain
* an OHCI controller. A simple way to test whether a PC card has an
* OHCI controller as an interface is to insert the PC card directly
* into a laptop(or desktop) with a CardBus slot and if "lspci" shows
* a new USB controller and "lsusb -v" shows a new OHCI Host Controller
* then there is a good chance that the U132 adapter will support the
* PC card.(you also need the specific client driver for the PC card)
*
* Please inform the Author and Maintainer about any PC cards that
* contain OHCI Host Controller and work when directly connected to
* an embedded CardBus slot but do not work when they are connected
* via an ELAN U132 adapter.
*
* The driver consists of two modules, the "ftdi-u132" module is
* a USB client driver that interfaces to the FTDI chip within
* the U132 adapter manufactured by Elan Digital Systems, and the
* "u132-hcd" module is a USB host controller driver that talks
* to the OHCI controller within CardBus card that are inserted
* in the U132 adapter.
*
* The "ftdi-u132" module should be loaded automatically by the
* hot plug system when the U132 adapter is plugged in. The module
* initialises the adapter which mostly consists of synchronising
* the FTDI chip, before continuously polling the adapter to detect
* PC card insertions. As soon as a PC card containing a recognised
* OHCI controller is seen the "ftdi-u132" module explicitly requests
* the kernel to load the "u132-hcd" module.
*
* The "ftdi-u132" module provides the interface to the inserted
* PC card and the "u132-hcd" module uses the API to send and receive
* data. The API features call-backs, so that part of the "u132-hcd"
* module code will run in the context of one of the kernel threads
* of the "ftdi-u132" module.
*
*/
int ftdi_elan_switch_on_diagnostics(int number);
void ftdi_elan_gone_away(struct platform_device *pdev);
void start_usb_lock_device_tracing(void);
struct u132_platform_data {
        u16 vendor;
        u16 device;
        u8 potpg;
        void (*port_power) (struct device *dev, int is_on);
        void (*reset) (struct device *dev);
};
int usb_ftdi_elan_edset_single(struct platform_device *pdev, u8 ed_number,
        void *endp, struct urb *urb, u8 address, u8 ep_number, u8 toggle_bits,
        void (*callback) (void *endp, struct urb *urb, u8 *buf, int len,
        int toggle_bits, int error_count, int condition_code, int repeat_number,
         int halted, int skipped, int actual, int non_null));
int usb_ftdi_elan_edset_output(struct platform_device *pdev, u8 ed_number,
        void *endp, struct urb *urb, u8 address, u8 ep_number, u8 toggle_bits,
        void (*callback) (void *endp, struct urb *urb, u8 *buf, int len,
        int toggle_bits, int error_count, int condition_code, int repeat_number,
         int halted, int skipped, int actual, int non_null));
int usb_ftdi_elan_edset_empty(struct platform_device *pdev, u8 ed_number,
        void *endp, struct urb *urb, u8 address, u8 ep_number, u8 toggle_bits,
        void (*callback) (void *endp, struct urb *urb, u8 *buf, int len,
        int toggle_bits, int error_count, int condition_code, int repeat_number,
         int halted, int skipped, int actual, int non_null));
int usb_ftdi_elan_edset_input(struct platform_device *pdev, u8 ed_number,
        void *endp, struct urb *urb, u8 address, u8 ep_number, u8 toggle_bits,
        void (*callback) (void *endp, struct urb *urb, u8 *buf, int len,
        int toggle_bits, int error_count, int condition_code, int repeat_number,
         int halted, int skipped, int actual, int non_null));
int usb_ftdi_elan_edset_setup(struct platform_device *pdev, u8 ed_number,
        void *endp, struct urb *urb, u8 address, u8 ep_number, u8 toggle_bits,
        void (*callback) (void *endp, struct urb *urb, u8 *buf, int len,
        int toggle_bits, int error_count, int condition_code, int repeat_number,
         int halted, int skipped, int actual, int non_null));
int usb_ftdi_elan_edset_flush(struct platform_device *pdev, u8 ed_number,
        void *endp);
int usb_ftdi_elan_read_pcimem(struct platform_device *pdev, int mem_offset,
			      u8 width, u32 *data);
int usb_ftdi_elan_write_pcimem(struct platform_device *pdev, int mem_offset,
			       u8 width, u32 data);
