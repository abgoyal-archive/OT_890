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

/*******************************************************************************
*
*   (c) 1998 by Computone Corporation
*
********************************************************************************
*
*
*   PACKAGE:     Linux tty Device Driver for IntelliPort II family of multiport
*                serial I/O controllers.
*
*   DESCRIPTION: Driver constants and type definitions.
*
*   NOTES:
*
*******************************************************************************/
#ifndef IP2TYPES_H
#define IP2TYPES_H

//*************
//* Constants *
//*************

// Define some limits for this driver. Ports per board is a hardware limitation
// that will not change. Current hardware limits this to 64 ports per board.
// Boards per driver is a self-imposed limit.
//
#define IP2_MAX_BOARDS        4
#define IP2_PORTS_PER_BOARD   ABS_MOST_PORTS
#define IP2_MAX_PORTS         (IP2_MAX_BOARDS*IP2_PORTS_PER_BOARD)

#define ISA    0
#define PCI    1
#define EISA   2

//********************
//* Type Definitions *
//********************

typedef struct tty_struct *   PTTY;
typedef wait_queue_head_t   PWAITQ;

typedef unsigned char         UCHAR;
typedef unsigned int          UINT;
typedef unsigned short        USHORT;
typedef unsigned long         ULONG;

typedef struct 
{
	short irq[IP2_MAX_BOARDS]; 
	unsigned short addr[IP2_MAX_BOARDS];
	int type[IP2_MAX_BOARDS];
#ifdef CONFIG_PCI
	struct pci_dev *pci_dev[IP2_MAX_BOARDS];
#endif
} ip2config_t;

#endif
