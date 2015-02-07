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
 * Driver for Microtek Scanmaker X6 USB scanner and possibly others.
 * 
 * (C) Copyright 2000 John Fremlin <vii@penguinpowered.com>
 * (C) Copyright 2000 Oliver Neukum <Oliver.Neukum@lrz.uni-muenchen.de>
 *
 * See microtek.c for history
 *
 */

typedef void (*mts_scsi_cmnd_callback)(struct scsi_cmnd *);


struct mts_transfer_context
{
	struct mts_desc* instance;
	mts_scsi_cmnd_callback final_callback;
	struct scsi_cmnd *srb;
	
	void* data;
	unsigned data_length;
	int data_pipe;
	int fragment;

	u8 *scsi_status; /* status returned from ep_response after command completion */
};


struct mts_desc {
	struct mts_desc *next;
	struct mts_desc *prev;

	struct usb_device *usb_dev;
	struct usb_interface *usb_intf;

	/* Endpoint addresses */
	u8 ep_out;
	u8 ep_response;
	u8 ep_image;

	struct Scsi_Host * host;

	struct urb *urb;
	struct mts_transfer_context context;
};


#define MTS_EP_OUT	0x1
#define MTS_EP_RESPONSE	0x2
#define MTS_EP_IMAGE	0x3
#define MTS_EP_TOTAL	0x3

#define MTS_SCSI_ERR_MASK ~0x3fu

