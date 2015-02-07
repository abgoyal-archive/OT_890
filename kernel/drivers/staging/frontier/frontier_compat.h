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

/* USB defines for older kernels */

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 19)

/**
 * usb_endpoint_dir_out - check if the endpoint has OUT direction
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint is of type OUT, otherwise it returns false.
 */

static inline int usb_endpoint_dir_out(const struct usb_endpoint_descriptor *epd)
{
       return ((epd->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_OUT);
}

static inline int usb_endpoint_dir_in(const struct usb_endpoint_descriptor *epd)
{
       return ((epd->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN);
}


/**
 * usb_endpoint_xfer_int - check if the endpoint has interrupt transfer type
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint is of type interrupt, otherwise it returns
 * false.
 */
static inline int usb_endpoint_xfer_int(const struct usb_endpoint_descriptor *epd)
{
       return ((epd->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
               USB_ENDPOINT_XFER_INT);
}


/**
 * usb_endpoint_is_int_in - check if the endpoint is interrupt IN
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint has interrupt transfer type and IN direction,
 * otherwise it returns false.
 */

static inline int usb_endpoint_is_int_in(const struct usb_endpoint_descriptor *epd)
{
       return (usb_endpoint_xfer_int(epd) && usb_endpoint_dir_in(epd));
}

/**
 * usb_endpoint_is_int_out - check if the endpoint is interrupt OUT
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint has interrupt transfer type and OUT direction,
 * otherwise it returns false.
 */

static inline int usb_endpoint_is_int_out(const struct usb_endpoint_descriptor *epd)
{
       return (usb_endpoint_xfer_int(epd) && usb_endpoint_dir_out(epd));
}

#endif /* older kernel versions */
