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
//  Copyright (c) 1996-2002 Winbond Electronic Corporation
//
//  Module Name:
//    Wb35Tx.c
//
//  Abstract:
//    Processing the Tx message and put into down layer
//
//============================================================================
#include <linux/usb.h>

#include "wb35tx_f.h"
#include "mds_f.h"
#include "sysdef.h"

unsigned char
Wb35Tx_get_tx_buffer(phw_data_t pHwData, u8 **pBuffer)
{
	PWB35TX pWb35Tx = &pHwData->Wb35Tx;

	*pBuffer = pWb35Tx->TxBuffer[0];
	return true;
}

static void Wb35Tx(struct wbsoft_priv *adapter);

static void Wb35Tx_complete(struct urb * pUrb)
{
	struct wbsoft_priv *adapter = pUrb->context;
	phw_data_t	pHwData = &adapter->sHwData;
	PWB35TX		pWb35Tx = &pHwData->Wb35Tx;
	PMDS		pMds = &adapter->Mds;

	printk("wb35: tx complete\n");
	// Variable setting
	pWb35Tx->EP4vm_state = VM_COMPLETED;
	pWb35Tx->EP4VM_status = pUrb->status; //Store the last result of Irp
	pMds->TxOwner[ pWb35Tx->TxSendIndex ] = 0;// Set the owner. Free the owner bit always.
	pWb35Tx->TxSendIndex++;
	pWb35Tx->TxSendIndex %= MAX_USB_TX_BUFFER_NUMBER;

	if (pHwData->SurpriseRemove || pHwData->HwStop) // Let WbWlanHalt to handle surprise remove
		goto error;

	if (pWb35Tx->tx_halt)
		goto error;

	// The URB is completed, check the result
	if (pWb35Tx->EP4VM_status != 0) {
		printk("URB submission failed\n");
		pWb35Tx->EP4vm_state = VM_STOP;
		goto error;
	}

	Mds_Tx(adapter);
	Wb35Tx(adapter);
	return;

error:
	atomic_dec(&pWb35Tx->TxFireCounter);
	pWb35Tx->EP4vm_state = VM_STOP;
}

static void Wb35Tx(struct wbsoft_priv *adapter)
{
	phw_data_t	pHwData = &adapter->sHwData;
	PWB35TX		pWb35Tx = &pHwData->Wb35Tx;
	u8		*pTxBufferAddress;
	PMDS		pMds = &adapter->Mds;
	struct urb *	pUrb = (struct urb *)pWb35Tx->Tx4Urb;
	int         	retv;
	u32		SendIndex;


	if (pHwData->SurpriseRemove || pHwData->HwStop)
		goto cleanup;

	if (pWb35Tx->tx_halt)
		goto cleanup;

	// Ownership checking
	SendIndex = pWb35Tx->TxSendIndex;
	if (!pMds->TxOwner[SendIndex]) //No more data need to be sent, return immediately
		goto cleanup;

	pTxBufferAddress = pWb35Tx->TxBuffer[SendIndex];
	//
	// Issuing URB
	//
	usb_fill_bulk_urb(pUrb, pHwData->WbUsb.udev,
			  usb_sndbulkpipe(pHwData->WbUsb.udev, 4),
			  pTxBufferAddress, pMds->TxBufferSize[ SendIndex ],
			  Wb35Tx_complete, adapter);

	pWb35Tx->EP4vm_state = VM_RUNNING;
	retv = usb_submit_urb(pUrb, GFP_ATOMIC);
	if (retv<0) {
		printk("EP4 Tx Irp sending error\n");
		goto cleanup;
	}

	// Check if driver needs issue Irp for EP2
	pWb35Tx->TxFillCount += pMds->TxCountInBuffer[SendIndex];
	if (pWb35Tx->TxFillCount > 12)
		Wb35Tx_EP2VM_start(adapter);

	pWb35Tx->ByteTransfer += pMds->TxBufferSize[SendIndex];
	return;

 cleanup:
	pWb35Tx->EP4vm_state = VM_STOP;
	atomic_dec(&pWb35Tx->TxFireCounter);
}

void Wb35Tx_start(struct wbsoft_priv *adapter)
{
	phw_data_t pHwData = &adapter->sHwData;
	PWB35TX pWb35Tx = &pHwData->Wb35Tx;

	// Allow only one thread to run into function
	if (atomic_inc_return(&pWb35Tx->TxFireCounter) == 1) {
		pWb35Tx->EP4vm_state = VM_RUNNING;
		Wb35Tx(adapter);
	} else
		atomic_dec(&pWb35Tx->TxFireCounter);
}

unsigned char Wb35Tx_initial(phw_data_t pHwData)
{
	PWB35TX pWb35Tx = &pHwData->Wb35Tx;

	pWb35Tx->Tx4Urb = usb_alloc_urb(0, GFP_ATOMIC);
	if (!pWb35Tx->Tx4Urb)
		return false;

	pWb35Tx->Tx2Urb = usb_alloc_urb(0, GFP_ATOMIC);
	if (!pWb35Tx->Tx2Urb)
	{
		usb_free_urb( pWb35Tx->Tx4Urb );
		return false;
	}

	return true;
}

//======================================================
void Wb35Tx_stop(phw_data_t pHwData)
{
	PWB35TX pWb35Tx = &pHwData->Wb35Tx;

	// Trying to canceling the Trp of EP2
	if (pWb35Tx->EP2vm_state == VM_RUNNING)
		usb_unlink_urb( pWb35Tx->Tx2Urb ); // Only use unlink, let Wb35Tx_destrot to free them
	#ifdef _PE_TX_DUMP_
	WBDEBUG(("EP2 Tx stop\n"));
	#endif

	// Trying to canceling the Irp of EP4
	if (pWb35Tx->EP4vm_state == VM_RUNNING)
		usb_unlink_urb( pWb35Tx->Tx4Urb ); // Only use unlink, let Wb35Tx_destrot to free them
	#ifdef _PE_TX_DUMP_
	WBDEBUG(("EP4 Tx stop\n"));
	#endif
}

//======================================================
void Wb35Tx_destroy(phw_data_t pHwData)
{
	PWB35TX pWb35Tx = &pHwData->Wb35Tx;

	// Wait for VM stop
	do {
		msleep(10);  // Delay for waiting function enter 940623.1.a
	} while( (pWb35Tx->EP2vm_state != VM_STOP) && (pWb35Tx->EP4vm_state != VM_STOP) );
	msleep(10);  // Delay for waiting function enter 940623.1.b

	if (pWb35Tx->Tx4Urb)
		usb_free_urb( pWb35Tx->Tx4Urb );

	if (pWb35Tx->Tx2Urb)
		usb_free_urb( pWb35Tx->Tx2Urb );

	#ifdef _PE_TX_DUMP_
	WBDEBUG(("Wb35Tx_destroy OK\n"));
	#endif
}

void Wb35Tx_CurrentTime(struct wbsoft_priv *adapter, u32 TimeCount)
{
	phw_data_t pHwData = &adapter->sHwData;
	PWB35TX pWb35Tx = &pHwData->Wb35Tx;
	unsigned char Trigger = false;

	if (pWb35Tx->TxTimer > TimeCount)
		Trigger = true;
	else if (TimeCount > (pWb35Tx->TxTimer+500))
		Trigger = true;

	if (Trigger) {
		pWb35Tx->TxTimer = TimeCount;
		Wb35Tx_EP2VM_start(adapter);
	}
}

static void Wb35Tx_EP2VM(struct wbsoft_priv *adapter);

static void Wb35Tx_EP2VM_complete(struct urb * pUrb)
{
	struct wbsoft_priv *adapter = pUrb->context;
	phw_data_t	pHwData = &adapter->sHwData;
	T02_DESCRIPTOR	T02, TSTATUS;
	PWB35TX		pWb35Tx = &pHwData->Wb35Tx;
	u32 *		pltmp = (u32 *)pWb35Tx->EP2_buf;
	u32		i;
	u16		InterruptInLength;


	// Variable setting
	pWb35Tx->EP2vm_state = VM_COMPLETED;
	pWb35Tx->EP2VM_status = pUrb->status;

	// For Linux 2.4. Interrupt will always trigger
	if (pHwData->SurpriseRemove || pHwData->HwStop) // Let WbWlanHalt to handle surprise remove
		goto error;

	if (pWb35Tx->tx_halt)
		goto error;

	//The Urb is completed, check the result
	if (pWb35Tx->EP2VM_status != 0) {
		WBDEBUG(("EP2 IoCompleteRoutine return error\n"));
		pWb35Tx->EP2vm_state= VM_STOP;
		goto error;
	}

	// Update the Tx result
	InterruptInLength = pUrb->actual_length;
	// Modify for minimum memory access and DWORD alignment.
	T02.value = cpu_to_le32(pltmp[0]) >> 8; // [31:8] -> [24:0]
	InterruptInLength -= 1;// 20051221.1.c Modify the follow for more stable
	InterruptInLength >>= 2; // InterruptInLength/4
	for (i = 1; i <= InterruptInLength; i++) {
		T02.value |= ((cpu_to_le32(pltmp[i]) & 0xff) << 24);

		TSTATUS.value = T02.value;  //20061009 anson's endian
		Mds_SendComplete( adapter, &TSTATUS );
		T02.value = cpu_to_le32(pltmp[i]) >> 8;
	}

	return;
error:
	atomic_dec(&pWb35Tx->TxResultCount);
	pWb35Tx->EP2vm_state = VM_STOP;
}

static void Wb35Tx_EP2VM(struct wbsoft_priv *adapter)
{
	phw_data_t	pHwData = &adapter->sHwData;
	PWB35TX pWb35Tx = &pHwData->Wb35Tx;
	struct urb *	pUrb = (struct urb *)pWb35Tx->Tx2Urb;
	u32 *	pltmp = (u32 *)pWb35Tx->EP2_buf;
	int		retv;

	if (pHwData->SurpriseRemove || pHwData->HwStop)
		goto error;

	if (pWb35Tx->tx_halt)
		goto error;

	//
	// Issuing URB
	//
	usb_fill_int_urb( pUrb, pHwData->WbUsb.udev, usb_rcvintpipe(pHwData->WbUsb.udev,2),
			  pltmp, MAX_INTERRUPT_LENGTH, Wb35Tx_EP2VM_complete, adapter, 32);

	pWb35Tx->EP2vm_state = VM_RUNNING;
	retv = usb_submit_urb(pUrb, GFP_ATOMIC);

	if (retv < 0) {
		#ifdef _PE_TX_DUMP_
		WBDEBUG(("EP2 Tx Irp sending error\n"));
		#endif
		goto error;
	}

	return;
error:
	pWb35Tx->EP2vm_state = VM_STOP;
	atomic_dec(&pWb35Tx->TxResultCount);
}

void Wb35Tx_EP2VM_start(struct wbsoft_priv *adapter)
{
	phw_data_t pHwData = &adapter->sHwData;
	PWB35TX pWb35Tx = &pHwData->Wb35Tx;

	// Allow only one thread to run into function
	if (atomic_inc_return(&pWb35Tx->TxResultCount) == 1) {
		pWb35Tx->EP2vm_state = VM_RUNNING;
		Wb35Tx_EP2VM(adapter);
	}
	else
		atomic_dec(&pWb35Tx->TxResultCount);
}