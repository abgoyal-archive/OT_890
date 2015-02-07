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
 *
 * Includes for cdc-acm.c
 *
 * Mainly take from usbnet's cdc-ether part
 *
 */

/*
 * CMSPAR, some architectures can't have space and mark parity.
 */

#ifndef CMSPAR
#define CMSPAR			0
#endif

/*
 * Major and minor numbers.
 */

#define ACM_TTY_MAJOR		166
#define ACM_TTY_MINORS		32

/*
 * Requests.
 */

#define USB_RT_ACM		(USB_TYPE_CLASS | USB_RECIP_INTERFACE)

/*
 * Output control lines.
 */

#define ACM_CTRL_DTR		0x01
#define ACM_CTRL_RTS		0x02

/*
 * Input control lines and line errors.
 */

#define ACM_CTRL_DCD		0x01
#define ACM_CTRL_DSR		0x02
#define ACM_CTRL_BRK		0x04
#define ACM_CTRL_RI		0x08

#define ACM_CTRL_FRAMING	0x10
#define ACM_CTRL_PARITY		0x20
#define ACM_CTRL_OVERRUN	0x40

/*
 * Internal driver structures.
 */

/*
 * The only reason to have several buffers is to accomodate assumptions
 * in line disciplines. They ask for empty space amount, receive our URB size,
 * and proceed to issue several 1-character writes, assuming they will fit.
 * The very first write takes a complete URB. Fortunately, this only happens
 * when processing onlcr, so we only need 2 buffers. These values must be
 * powers of 2.
 */
#define ACM_NW  16
#define ACM_NR  16

struct acm_wb {
	unsigned char *buf;
	dma_addr_t dmah;
	int len;
	int use;
	struct urb		*urb;
	struct acm		*instance;
};

struct acm_rb {
	struct list_head	list;
	int			size;
	unsigned char		*base;
	dma_addr_t		dma;
};

struct acm_ru {
	struct list_head	list;
	struct acm_rb		*buffer;
	struct urb		*urb;
	struct acm		*instance;
};

struct acm {
	struct usb_device *dev;				/* the corresponding usb device */
	struct usb_interface *control;			/* control interface */
	struct usb_interface *data;			/* data interface */
	struct tty_struct *tty;				/* the corresponding tty */
	struct urb *ctrlurb;			/* urbs */
	u8 *ctrl_buffer;				/* buffers of urbs */
	dma_addr_t ctrl_dma;				/* dma handles of buffers */
	u8 *country_codes;				/* country codes from device */
	unsigned int country_code_size;			/* size of this buffer */
	unsigned int country_rel_date;			/* release date of version */
	struct acm_wb wb[ACM_NW];
	struct acm_ru ru[ACM_NR];
	struct acm_rb rb[ACM_NR];
	int rx_buflimit;
	int rx_endpoint;
	spinlock_t read_lock;
	struct list_head spare_read_urbs;
	struct list_head spare_read_bufs;
	struct list_head filled_read_bufs;
	int write_used;					/* number of non-empty write buffers */
	int processing;
	int transmitting;
	spinlock_t write_lock;
	struct mutex mutex;
	struct usb_cdc_line_coding line;		/* bits, stop, parity */
	struct work_struct work;			/* work queue entry for line discipline waking up */
	struct work_struct waker;
	wait_queue_head_t drain_wait;			/* close processing */
	struct tasklet_struct urb_task;                 /* rx processing */
	spinlock_t throttle_lock;			/* synchronize throtteling and read callback */
	unsigned int ctrlin;				/* input control lines (DCD, DSR, RI, break, overruns) */
	unsigned int ctrlout;				/* output control lines (DTR, RTS) */
	unsigned int writesize;				/* max packet size for the output bulk endpoint */
	unsigned int readsize,ctrlsize;			/* buffer sizes for freeing */
	unsigned int used;				/* someone has this acm's device open */
	unsigned int minor;				/* acm minor number */
	unsigned char throttle;				/* throttled by tty layer */
	unsigned char clocal;				/* termios CLOCAL */
	unsigned int ctrl_caps;				/* control capabilities from the class specific header */
	unsigned int susp_count;			/* number of suspended interfaces */
	struct acm_wb *delayed_wb;			/* write queued for a device about to be woken */
};

#define CDC_DATA_INTERFACE_TYPE	0x0a

/* constants describing various quirks and errors */
#define NO_UNION_NORMAL			1
#define SINGLE_RX_URB			2
