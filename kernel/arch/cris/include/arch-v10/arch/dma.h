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

/* Defines for using and allocating dma channels. */

#ifndef _ASM_ARCH_DMA_H
#define _ASM_ARCH_DMA_H

#define MAX_DMA_CHANNELS	10

/* dma0 and dma1 used for network (ethernet) */
#define NETWORK_TX_DMA_NBR 0
#define NETWORK_RX_DMA_NBR 1

/* dma2 and dma3 shared by par0, scsi0, ser2 and ata */
#define PAR0_TX_DMA_NBR 2
#define PAR0_RX_DMA_NBR 3
#define SCSI0_TX_DMA_NBR 2
#define SCSI0_RX_DMA_NBR 3
#define SER2_TX_DMA_NBR 2
#define SER2_RX_DMA_NBR 3
#define ATA_TX_DMA_NBR 2
#define ATA_RX_DMA_NBR 3

/* dma4 and dma5 shared by par1, scsi1, ser3 and extdma0 */
#define PAR1_TX_DMA_NBR 4
#define PAR1_RX_DMA_NBR 5
#define SCSI1_TX_DMA_NBR 4
#define SCSI1_RX_DMA_NBR 5
#define SER3_TX_DMA_NBR 4
#define SER3_RX_DMA_NBR 5
#define EXTDMA0_TX_DMA_NBR 4
#define EXTDMA0_RX_DMA_NBR 5

/* dma6 and dma7 shared by ser0, extdma1 and mem2mem */
#define SER0_TX_DMA_NBR 6
#define SER0_RX_DMA_NBR 7
#define EXTDMA1_TX_DMA_NBR 6
#define EXTDMA1_RX_DMA_NBR 7
#define MEM2MEM_TX_DMA_NBR 6
#define MEM2MEM_RX_DMA_NBR 7

/* dma8 and dma9 shared by ser1 and usb */
#define SER1_TX_DMA_NBR 8
#define SER1_RX_DMA_NBR 9
#define USB_TX_DMA_NBR 8
#define USB_RX_DMA_NBR 9

#endif

enum dma_owner
{
  dma_eth,
  dma_ser0,
  dma_ser1, /* Async and sync */
  dma_ser2,
  dma_ser3, /* Async and sync */
  dma_ata,
  dma_par0,
  dma_par1,
  dma_ext0,
  dma_ext1,
  dma_int6,
  dma_int7,
  dma_usb,
  dma_scsi0,
  dma_scsi1
};

/* Masks used by cris_request_dma options: */
#define DMA_VERBOSE_ON_ERROR    (1<<0)
#define DMA_PANIC_ON_ERROR     ((1<<1)|DMA_VERBOSE_ON_ERROR)

int cris_request_dma(unsigned int dmanr, const char * device_id,
                     unsigned options, enum dma_owner owner);

void cris_free_dma(unsigned int dmanr, const char * device_id);
