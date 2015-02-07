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
 *	Generic watchdog defines. Derived from..
 *
 * Berkshire PC Watchdog Defines
 * by Ken Hollis <khollis@bitgate.com>
 *
 */

#ifndef _LINUX_WATCHDOG_H
#define _LINUX_WATCHDOG_H

#include <linux/ioctl.h>
#include <linux/types.h>

#define	WATCHDOG_IOCTL_BASE	'W'

struct watchdog_info {
	__u32 options;		/* Options the card/driver supports */
	__u32 firmware_version;	/* Firmware version of the card */
	__u8  identity[32];	/* Identity of the board */
};

#define	WDIOC_GETSUPPORT	_IOR(WATCHDOG_IOCTL_BASE, 0, struct watchdog_info)
#define	WDIOC_GETSTATUS		_IOR(WATCHDOG_IOCTL_BASE, 1, int)
#define	WDIOC_GETBOOTSTATUS	_IOR(WATCHDOG_IOCTL_BASE, 2, int)
#define	WDIOC_GETTEMP		_IOR(WATCHDOG_IOCTL_BASE, 3, int)
#define	WDIOC_SETOPTIONS	_IOR(WATCHDOG_IOCTL_BASE, 4, int)
#define	WDIOC_KEEPALIVE		_IOR(WATCHDOG_IOCTL_BASE, 5, int)
#define	WDIOC_SETTIMEOUT        _IOWR(WATCHDOG_IOCTL_BASE, 6, int)
#define	WDIOC_GETTIMEOUT        _IOR(WATCHDOG_IOCTL_BASE, 7, int)
#define	WDIOC_SETPRETIMEOUT	_IOWR(WATCHDOG_IOCTL_BASE, 8, int)
#define	WDIOC_GETPRETIMEOUT	_IOR(WATCHDOG_IOCTL_BASE, 9, int)
#define	WDIOC_GETTIMELEFT	_IOR(WATCHDOG_IOCTL_BASE, 10, int)

#define	WDIOF_UNKNOWN		-1	/* Unknown flag error */
#define	WDIOS_UNKNOWN		-1	/* Unknown status error */

#define	WDIOF_OVERHEAT		0x0001	/* Reset due to CPU overheat */
#define	WDIOF_FANFAULT		0x0002	/* Fan failed */
#define	WDIOF_EXTERN1		0x0004	/* External relay 1 */
#define	WDIOF_EXTERN2		0x0008	/* External relay 2 */
#define	WDIOF_POWERUNDER	0x0010	/* Power bad/power fault */
#define	WDIOF_CARDRESET		0x0020	/* Card previously reset the CPU */
#define	WDIOF_POWEROVER		0x0040	/* Power over voltage */
#define	WDIOF_SETTIMEOUT	0x0080  /* Set timeout (in seconds) */
#define	WDIOF_MAGICCLOSE	0x0100	/* Supports magic close char */
#define	WDIOF_PRETIMEOUT	0x0200  /* Pretimeout (in seconds), get/set */
#define	WDIOF_KEEPALIVEPING	0x8000	/* Keep alive ping reply */

#define	WDIOS_DISABLECARD	0x0001	/* Turn off the watchdog timer */
#define	WDIOS_ENABLECARD	0x0002	/* Turn on the watchdog timer */
#define	WDIOS_TEMPPANIC		0x0004	/* Kernel panic on temperature trip */

#ifdef __KERNEL__

#ifdef CONFIG_WATCHDOG_NOWAYOUT
#define WATCHDOG_NOWAYOUT	1
#else
#define WATCHDOG_NOWAYOUT	0
#endif

#endif	/* __KERNEL__ */

#endif  /* ifndef _LINUX_WATCHDOG_H */
