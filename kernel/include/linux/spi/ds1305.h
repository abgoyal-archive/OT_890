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

#ifndef __LINUX_SPI_DS1305_H
#define __LINUX_SPI_DS1305_H

/*
 * One-time configuration for ds1305 and ds1306 RTC chips.
 *
 * Put a pointer to this in spi_board_info.platform_data if you want to
 * be sure that Linux (re)initializes this as needed ... after losing
 * backup power, and potentially on the first boot.
 */
struct ds1305_platform_data {

	/* Trickle charge configuration:  it's OK to leave out the MAGIC
	 * bitmask; mask in either DS1 or DS2, and then one of 2K/4k/8K.
	 */
#define DS1305_TRICKLE_MAGIC	0xa0
#define DS1305_TRICKLE_DS2	0x08	/* two diodes */
#define DS1305_TRICKLE_DS1	0x04	/* one diode */
#define DS1305_TRICKLE_2K	0x01	/* 2 KOhm resistance */
#define DS1305_TRICKLE_4K	0x02	/* 4 KOhm resistance */
#define DS1305_TRICKLE_8K	0x03	/* 8 KOhm resistance */
	u8	trickle;

	/* set only on ds1306 parts */
	bool	is_ds1306;

	/* ds1306 only:  enable 1 Hz output */
	bool	en_1hz;

	/* REVISIT:  the driver currently expects nINT0 to be wired
	 * as the alarm IRQ.  ALM1 may also need to be set up ...
	 */
};

#endif /* __LINUX_SPI_DS1305_H */
