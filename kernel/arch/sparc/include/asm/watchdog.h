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
 * watchdog - Driver interface for the hardware watchdog timers
 * present on Sun Microsystems boardsets
 *
 * Copyright (c) 2000 Eric Brower <ebrower@usa.net>
 *
 */

#ifndef _SPARC64_WATCHDOG_H
#define _SPARC64_WATCHDOG_H

#include <linux/watchdog.h>

/* Solaris compatibility ioctls--
 * Ref. <linux/watchdog.h> for standard linux watchdog ioctls
 */
#define WIOCSTART _IO (WATCHDOG_IOCTL_BASE, 10)		/* Start Timer		*/
#define WIOCSTOP  _IO (WATCHDOG_IOCTL_BASE, 11)		/* Stop Timer		*/
#define WIOCGSTAT _IOR(WATCHDOG_IOCTL_BASE, 12, int)/* Get Timer Status	*/

/* Status flags from WIOCGSTAT ioctl
 */
#define WD_FREERUN	0x01	/* timer is running, interrupts disabled	*/
#define WD_EXPIRED	0x02	/* timer has expired						*/
#define WD_RUNNING	0x04	/* timer is running, interrupts enabled		*/
#define WD_STOPPED	0x08	/* timer has not been started				*/
#define WD_SERVICED 0x10	/* timer interrupt was serviced				*/

#endif /* ifndef _SPARC64_WATCHDOG_H */

