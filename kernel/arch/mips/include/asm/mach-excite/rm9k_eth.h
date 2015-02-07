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

#if !defined(__RM9K_ETH_H__)
#define __RM9K_ETH_H__

#define RM9K_GE_NAME			"rm9k_ge"

/* Resource names */
#define RM9K_GE_RESOURCE_MAC      	"rm9k_ge_mac"
#define RM9K_GE_RESOURCE_MSTAT      	"rm9k_ge_mstat"
#define RM9K_GE_RESOURCE_PKTPROC	"rm9k_ge_pktproc"
#define RM9K_GE_RESOURCE_XDMA		"rm9k_ge_xdma"
#define RM9K_GE_RESOURCE_FIFO_RX  	"rm9k_ge_fifo_rx"
#define RM9K_GE_RESOURCE_FIFO_TX  	"rm9k_ge_fifo_tx"
#define RM9K_GE_RESOURCE_FIFOMEM_RX  	"rm9k_ge_fifo_memory_rx"
#define RM9K_GE_RESOURCE_FIFOMEM_TX  	"rm9k_ge_fifo_memory_tx"
#define RM9K_GE_RESOURCE_PHY      	"rm9k_ge_phy"
#define RM9K_GE_RESOURCE_DMADESC_RX  	"rm9k_ge_dmadesc_rx"
#define RM9K_GE_RESOURCE_DMADESC_TX  	"rm9k_ge_dmadesc_tx"
#define RM9K_GE_RESOURCE_IRQ_MAIN	"rm9k_ge_irq_main"
#define RM9K_GE_RESOURCE_IRQ_PHY	"rm9k_ge_irq_phy"
#define RM9K_GE_RESOURCE_GPI_SLICE	"rm9k_ge_gpi_slice"
#define RM9K_GE_RESOURCE_MDIO_CHANNEL	"rm9k_ge_mdio_channel"

#endif /* !defined(__RM9K_ETH_H__) */