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

#ifndef __WINBOND_WB35_TX_S_H
#define __WINBOND_WB35_TX_S_H

#include "mds_s.h"

//====================================
// IS89C35 Tx related definition
//====================================
#define TX_INTERFACE			0	// Interface 1
#define TX_PIPE					3	// endpoint 4
#define TX_INTERRUPT			1	// endpoint 2
#define MAX_INTERRUPT_LENGTH	64	// It must be 64 for EP2 hardware



//====================================
// Internal variable for module
//====================================


typedef struct _WB35TX
{
	// For Tx buffer
	u8	TxBuffer[ MAX_USB_TX_BUFFER_NUMBER ][ MAX_USB_TX_BUFFER ];

	// For Interrupt pipe
	u8	EP2_buf[MAX_INTERRUPT_LENGTH];

	atomic_t	TxResultCount;// For thread control of EP2 931130.4.m
	atomic_t	TxFireCounter;// For thread control of EP4 931130.4.n
	u32			ByteTransfer;

	u32	    TxSendIndex;// The next index of Mds array to be sent
	u32	    EP2vm_state; // for EP2vm state
	u32	    EP4vm_state; // for EP4vm state
	u32	    tx_halt; // Stopping VM

	struct urb *				Tx4Urb;
	struct urb *				Tx2Urb;

	int		EP2VM_status;
	int		EP4VM_status;

	u32	TxFillCount; // 20060928
	u32	TxTimer; // 20060928 Add if sending packet not great than 13

} WB35TX, *PWB35TX;

#endif
