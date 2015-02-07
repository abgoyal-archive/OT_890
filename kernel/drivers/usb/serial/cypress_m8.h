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

#ifndef CYPRESS_M8_H
#define CYPRESS_M8_H

/* definitions and function prototypes used for the cypress USB to Serial controller */

/* For sending our feature buffer - controlling serial communication states */
/* Linux HID has no support for serial devices so we do this through the driver */
#define HID_REQ_GET_REPORT 0x01
#define HID_REQ_SET_REPORT 0x09

/* List other cypress USB to Serial devices here, and add them to the id_table */

/* DeLorme Earthmate USB - a GPS device */
#define	VENDOR_ID_DELORME		 0x1163
#define PRODUCT_ID_EARTHMATEUSB		 0x0100
#define PRODUCT_ID_EARTHMATEUSB_LT20	 0x0200

/* Cypress HID->COM RS232 Adapter */
#define VENDOR_ID_CYPRESS		 0x04b4
#define PRODUCT_ID_CYPHIDCOM		 0x5500

/* Powercom UPS, chip CY7C63723 */
#define VENDOR_ID_POWERCOM		 0x0d9f
#define PRODUCT_ID_UPS			 0x0002

/* Nokia CA-42 USB to serial cable */
#define VENDOR_ID_DAZZLE		0x07d0
#define PRODUCT_ID_CA42			0x4101
/* End of device listing */

/* Used for setting / requesting serial line settings */
#define CYPRESS_SET_CONFIG 0x01
#define CYPRESS_GET_CONFIG 0x02

/* Used for throttle control */
#define THROTTLED 0x1
#define ACTUALLY_THROTTLED 0x2

/* chiptypes - used in case firmware differs from the generic form ... offering
 * 	different baud speeds/etc.
 */

#define CT_EARTHMATE	0x01
#define CT_CYPHIDCOM	0x02
#define CT_CA42V2	0x03
#define CT_GENERIC	0x0F
/* End of chiptype definitions */

/* RS-232 serial data communication protocol definitions */
/* these are sent / read at byte 0 of the input/output hid reports */
/* You can find these values defined in the CY4601 USB to Serial design notes */

#define CONTROL_DTR 	0x20	/* data terminal ready - flow control - host to device */
#define UART_DSR	0x20	/* data set ready - flow control - device to host */
#define CONTROL_RTS 	0x10	/* request to send - flow control - host to device */
#define UART_CTS	0x10	/* clear to send - flow control - device to host */
#define	UART_RI		0x10	/* ring indicator - modem - device to host */
#define UART_CD		0x40	/* carrier detect - modem - device to host */
#define CYP_ERROR 	0x08	/* received from input report - device to host */
/* Note - the below has nothing to to with the "feature report" reset */
#define CONTROL_RESET	0x08  	/* sent with output report - host to device */

/* End of RS-232 protocol definitions */

#endif /* CYPRESS_M8_H */
