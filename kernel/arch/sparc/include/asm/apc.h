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

/* apc - Driver definitions for power management functions
 * of Aurora Personality Chip (APC) on SPARCstation-4/5 and 
 * derivatives
 *
 * Copyright (c) 2001 Eric Brower (ebrower@usa.net)
 *
 */

#ifndef _SPARC_APC_H
#define _SPARC_APC_H

#include <linux/ioctl.h>

#define APC_IOC	'A'

#define APCIOCGFANCTL _IOR(APC_IOC, 0x00, int)	/* Get fan speed	*/
#define APCIOCSFANCTL _IOW(APC_IOC, 0x01, int)	/* Set fan speed	*/

#define APCIOCGCPWR   _IOR(APC_IOC, 0x02, int)	/* Get CPOWER state	*/
#define APCIOCSCPWR   _IOW(APC_IOC, 0x03, int)	/* Set CPOWER state	*/

#define APCIOCGBPORT   _IOR(APC_IOC, 0x04, int)	/* Get BPORT state 	*/
#define APCIOCSBPORT   _IOW(APC_IOC, 0x05, int)	/* Set BPORT state	*/

/*
 * Register offsets
 */
#define APC_IDLE_REG	0x00
#define APC_FANCTL_REG	0x20
#define APC_CPOWER_REG	0x24
#define APC_BPORT_REG	0x30

#define APC_REGMASK		0x01
#define APC_BPMASK		0x03

/*
 * IDLE - CPU standby values (set to initiate standby)
 */
#define APC_IDLE_ON		0x01

/*
 * FANCTL - Fan speed control state values
 */
#define APC_FANCTL_HI	0x00	/* Fan speed high	*/
#define APC_FANCTL_LO	0x01	/* Fan speed low	*/

/*
 * CPWR - Convenience power outlet state values 
 */
#define APC_CPOWER_ON	0x00	/* Conv power on	*/
#define APC_CPOWER_OFF	0x01	/* Conv power off	*/

/*
 * BPA/BPB - Read-Write "Bit Ports" state values (reset to 0 at power-on)
 *
 * WARNING: Internal usage of bit ports is platform dependent--
 * don't modify BPORT settings unless you know what you are doing.
 * 
 * On SS5 BPA seems to toggle onboard ethernet loopback... -E
 */
#define APC_BPORT_A		0x01	/* Bit Port A		*/
#define APC_BPORT_B		0x02	/* Bit Port B		*/

#endif /* !(_SPARC_APC_H) */
