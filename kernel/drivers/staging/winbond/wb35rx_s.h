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

//============================================================================
// wb35rx.h --
//============================================================================

// Definition for this module used
#define MAX_USB_RX_BUFFER	4096	// This parameter must be 4096 931130.4.f

#define MAX_USB_RX_BUFFER_NUMBER	ETHERNET_RX_DESCRIPTORS		// Maximum 254, 255 is RESERVED ID
#define RX_INTERFACE				0	// Interface 1
#define RX_PIPE						2	// Pipe 3
#define MAX_PACKET_SIZE				1600 //1568	// 8 + 1532 + 4 + 24(IV EIV MIC ICV CRC) for check DMA data 931130.4.g
#define RX_END_TAG					0x0badbeef


//====================================
// Internal variable for module
//====================================
typedef struct _WB35RX
{
	u32			ByteReceived;// For calculating throughput of BulkIn
	atomic_t		RxFireCounter;// Does Wb35Rx module fire?

	u8	RxBuffer[ MAX_USB_RX_BUFFER_NUMBER ][ ((MAX_USB_RX_BUFFER+3) & ~0x03 ) ];
	u16	RxBufferSize[ ((MAX_USB_RX_BUFFER_NUMBER+1) & ~0x01) ];
	u8	RxOwner[ ((MAX_USB_RX_BUFFER_NUMBER+3) & ~0x03 ) ];//Ownership of buffer  0: SW 1:HW

	u32	RxProcessIndex;//The next index to process
	u32	RxBufferId;
	u32	EP3vm_state;

	u32	rx_halt; // For VM stopping

	u16	MoreDataSize;
	u16	PacketSize;

	u32	CurrentRxBufferId; // For complete routine usage
	u32	Rx3UrbCancel;

	u32	LastR1; // For RSSI reporting
	struct urb *				RxUrb;
	u32		Ep3ErrorCount2; // 20060625.1 Usbd for Rx DMA error count

	int		EP3VM_status;
	u8 *	pDRx;

} WB35RX, *PWB35RX;


