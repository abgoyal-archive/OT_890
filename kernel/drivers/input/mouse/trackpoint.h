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
 * IBM TrackPoint PS/2 mouse driver
 *
 * Stephen Evanchik <evanchsa@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */

#ifndef _TRACKPOINT_H
#define _TRACKPOINT_H

/*
 * These constants are from the TrackPoint System
 * Engineering documentation Version 4 from IBM Watson
 * research:
 *	http://wwwcssrv.almaden.ibm.com/trackpoint/download.html
 */

#define TP_COMMAND		0xE2	/* Commands start with this */

#define TP_READ_ID		0xE1	/* Sent for device identification */
#define TP_MAGIC_IDENT		0x01	/* Sent after a TP_READ_ID followed */
					/* by the firmware ID */


/*
 * Commands
 */
#define TP_RECALIB		0x51	/* Recalibrate */
#define TP_POWER_DOWN		0x44	/* Can only be undone through HW reset */
#define TP_EXT_DEV		0x21	/* Determines if external device is connected (RO) */
#define TP_EXT_BTN		0x4B	/* Read extended button status */
#define TP_POR			0x7F	/* Execute Power on Reset */
#define TP_POR_RESULTS		0x25	/* Read Power on Self test results */
#define TP_DISABLE_EXT		0x40	/* Disable external pointing device */
#define TP_ENABLE_EXT		0x41	/* Enable external pointing device */

/*
 * Mode manipulation
 */
#define TP_SET_SOFT_TRANS	0x4E	/* Set mode */
#define TP_CANCEL_SOFT_TRANS	0xB9	/* Cancel mode */
#define TP_SET_HARD_TRANS	0x45	/* Mode can only be set */


/*
 * Register oriented commands/properties
 */
#define TP_WRITE_MEM		0x81
#define TP_READ_MEM		0x80	/* Not used in this implementation */

/*
* RAM Locations for properties
 */
#define TP_SENS			0x4A	/* Sensitivity */
#define TP_MB			0x4C	/* Read Middle Button Status (RO) */
#define TP_INERTIA		0x4D	/* Negative Inertia */
#define TP_SPEED		0x60	/* Speed of TP Cursor */
#define TP_REACH		0x57	/* Backup for Z-axis press */
#define TP_DRAGHYS		0x58	/* Drag Hysteresis */
					/* (how hard it is to drag */
					/* with Z-axis pressed) */

#define TP_MINDRAG		0x59	/* Minimum amount of force needed */
					/* to trigger dragging */

#define TP_THRESH		0x5C	/* Minimum value for a Z-axis press */
#define TP_UP_THRESH		0x5A	/* Used to generate a 'click' on Z-axis */
#define TP_Z_TIME		0x5E	/* How sharp of a press */
#define TP_JENKS_CURV		0x5D	/* Minimum curvature for double click */

/*
 * Toggling Flag bits
 */
#define TP_TOGGLE		0x47	/* Toggle command */

#define TP_TOGGLE_MB		0x23	/* Disable/Enable Middle Button */
#define TP_MASK_MB			0x01
#define TP_TOGGLE_EXT_DEV	0x23	/* Disable external device */
#define TP_MASK_EXT_DEV			0x02
#define TP_TOGGLE_DRIFT		0x23	/* Drift Correction */
#define TP_MASK_DRIFT			0x80
#define TP_TOGGLE_BURST		0x28	/* Burst Mode */
#define TP_MASK_BURST			0x80
#define TP_TOGGLE_PTSON		0x2C	/* Press to Select */
#define TP_MASK_PTSON			0x01
#define TP_TOGGLE_HARD_TRANS	0x2C	/* Alternate method to set Hard Transparency */
#define TP_MASK_HARD_TRANS		0x80
#define TP_TOGGLE_TWOHAND	0x2D	/* Two handed */
#define TP_MASK_TWOHAND			0x01
#define TP_TOGGLE_STICKY_TWO	0x2D	/* Sticky two handed */
#define TP_MASK_STICKY_TWO		0x04
#define TP_TOGGLE_SKIPBACK	0x2D	/* Suppress movement after drag release */
#define TP_MASK_SKIPBACK		0x08
#define TP_TOGGLE_SOURCE_TAG	0x20	/* Bit 3 of the first packet will be set to
					   to the origin of the packet (external or TP) */
#define TP_MASK_SOURCE_TAG		0x80
#define TP_TOGGLE_EXT_TAG	0x22	/* Bit 3 of the first packet coming from the
					   external device will be forced to 1 */
#define TP_MASK_EXT_TAG			0x04


/* Power on Self Test Results */
#define TP_POR_SUCCESS		0x3B

/*
 * Default power on values
 */
#define TP_DEF_SENS		0x80
#define TP_DEF_INERTIA		0x06
#define TP_DEF_SPEED		0x61
#define TP_DEF_REACH		0x0A

#define TP_DEF_DRAGHYS		0xFF
#define TP_DEF_MINDRAG		0x14

#define TP_DEF_THRESH		0x08
#define TP_DEF_UP_THRESH	0xFF
#define TP_DEF_Z_TIME		0x26
#define TP_DEF_JENKS_CURV	0x87

/* Toggles */
#define TP_DEF_MB		0x00
#define TP_DEF_PTSON		0x00
#define TP_DEF_SKIPBACK		0x00
#define TP_DEF_EXT_DEV		0x00	/* 0 means enabled */

#define MAKE_PS2_CMD(params, results, cmd) ((params<<12) | (results<<8) | (cmd))

struct trackpoint_data
{
	unsigned char sensitivity, speed, inertia, reach;
	unsigned char draghys, mindrag;
	unsigned char thresh, upthresh;
	unsigned char ztime, jenks;

	unsigned char press_to_select;
	unsigned char skipback;

	unsigned char ext_dev;
};

#ifdef CONFIG_MOUSE_PS2_TRACKPOINT
int trackpoint_detect(struct psmouse *psmouse, int set_properties);
#else
inline int trackpoint_detect(struct psmouse *psmouse, int set_properties)
{
	return -ENOSYS;
}
#endif /* CONFIG_MOUSE_PS2_TRACKPOINT */

#endif /* _TRACKPOINT_H */
