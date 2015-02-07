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

#define _BULK_DATA_LEN 64
typedef struct
{
	unsigned char data[_BULK_DATA_LEN];
	unsigned int size;
	unsigned int pipe;
}bulk_transfer_t,*pbulk_transfer_t;

#define DABUSB_MINOR 240		/* some unassigned USB minor */
#define DABUSB_VERSION 0x1000
#define IOCTL_DAB_BULK              _IOWR('d', 0x30, bulk_transfer_t)
#define IOCTL_DAB_OVERRUNS	    _IOR('d',  0x15, int)
#define IOCTL_DAB_VERSION           _IOR('d', 0x3f, int)

#ifdef __KERNEL__

typedef enum { _stopped=0, _started } driver_state_t;

typedef struct
{
	struct mutex mutex;
	struct usb_device *usbdev;
	wait_queue_head_t wait;
	wait_queue_head_t remove_ok;
	spinlock_t lock;
	atomic_t pending_io;
	driver_state_t state;
	int remove_pending;
	int got_mem;
	int total_buffer_size;
	unsigned int overruns;
	int readptr;
	int opened;
	int devnum;
	struct list_head free_buff_list;
	struct list_head rec_buff_list;
} dabusb_t,*pdabusb_t;

typedef struct
{
	pdabusb_t s;
	struct urb *purb;
	struct list_head buff_list;
} buff_t,*pbuff_t;

typedef struct
{
	wait_queue_head_t wait;
} bulk_completion_context_t, *pbulk_completion_context_t;


#define _DABUSB_IF 2
#define _DABUSB_ISOPIPE 0x09
#define _ISOPIPESIZE	16384

#define _BULK_DATA_LEN 64
// Vendor specific request code for Anchor Upload/Download
// This one is implemented in the core
#define ANCHOR_LOAD_INTERNAL  0xA0

// EZ-USB Control and Status Register.  Bit 0 controls 8051 reset
#define CPUCS_REG    0x7F92
#define _TOTAL_BUFFERS 384

#define MAX_INTEL_HEX_RECORD_LENGTH 16

#ifndef _BYTE_DEFINED
#define _BYTE_DEFINED
typedef unsigned char BYTE;
#endif // !_BYTE_DEFINED

#ifndef _WORD_DEFINED
#define _WORD_DEFINED
typedef unsigned short WORD;
#endif // !_WORD_DEFINED

typedef struct _INTEL_HEX_RECORD
{
   BYTE  Length;
   WORD  Address;
   BYTE  Type;
   BYTE  Data[MAX_INTEL_HEX_RECORD_LENGTH];
} INTEL_HEX_RECORD, *PINTEL_HEX_RECORD;

#endif
