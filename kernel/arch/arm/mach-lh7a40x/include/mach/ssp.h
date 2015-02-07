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

/* ssp.h

   written by Marc Singer
   6 Dec 2004

   Copyright (C) 2004 Marc Singer

   -----------
   DESCRIPTION
   -----------

   This SSP header is available throughout the kernel, for this
   machine/architecture, because drivers that use it may be dispersed.

   This file was cloned from the 7952x implementation.  It would be
   better to share them, but we're taking an easier approach for the
   time being.

*/

#if !defined (__SSP_H__)
#    define   __SSP_H__

/* ----- Includes */

/* ----- Types */

struct ssp_driver {
	int  (*init)		(void);
	void (*exit)		(void);
	void (*acquire)		(void);
	void (*release)		(void);
	int  (*configure)	(int device, int mode, int speed,
				 int frame_size_write, int frame_size_read);
	void (*chip_select)	(int enable);
	void (*set_callbacks)   (void* handle,
				 irqreturn_t (*callback_tx)(void*),
				 irqreturn_t (*callback_rx)(void*));
	void (*enable)		(void);
	void (*disable)		(void);
//	int  (*save_state)	(void*);
//	void (*restore_state)	(void*);
	int  (*read)		(void);
	int  (*write)		(u16 data);
	int  (*write_read)	(u16 data);
	void (*flush)		(void);
	void (*write_async)	(void* pv, size_t cb);
	size_t (*write_pos)	(void);
};

	/* These modes are only available on the LH79524 */
#define SSP_MODE_SPI		(1)
#define SSP_MODE_SSI		(2)
#define SSP_MODE_MICROWIRE	(3)
#define SSP_MODE_I2S		(4)

	/* CPLD SPI devices */
#define DEVICE_EEPROM	0	/* Configuration eeprom */
#define DEVICE_MAC	1	/* MAC eeprom (LPD79524) */
#define DEVICE_CODEC	2	/* Audio codec */
#define DEVICE_TOUCH	3	/* Touch screen (LPD79520) */

/* ----- Globals */

/* ----- Prototypes */

//extern struct ssp_driver lh79520_i2s_driver;
extern struct ssp_driver lh7a400_cpld_ssp_driver;

#endif  /* __SSP_H__ */
