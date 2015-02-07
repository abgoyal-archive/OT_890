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

/* $Id$ 
 *
 * memcpy for large blocks, using memory-memory DMA channels 6 and 7 in Etrax
 */

#include <asm/svinto.h>
#include <asm/io.h>

#define D(x)

void *dma_memcpy(void *pdst,
		 const void *psrc,
		 unsigned int pn)
{
	static etrax_dma_descr indma, outdma;
	
	D(printk("dma_memcpy %d bytes... ", pn));

#if 0
	*R_GEN_CONFIG = genconfig_shadow = 
		(genconfig_shadow & ~0x3c0000) |
		IO_STATE(R_GEN_CONFIG, dma6, intdma7) |
		IO_STATE(R_GEN_CONFIG, dma7, intdma6);
#endif
	indma.sw_len = outdma.sw_len = pn;
	indma.ctrl = d_eol | d_eop;
	outdma.ctrl = d_eol;
	indma.buf = psrc;
	outdma.buf = pdst;

	*R_DMA_CH6_FIRST = &indma;
	*R_DMA_CH7_FIRST = &outdma;
	*R_DMA_CH6_CMD = IO_STATE(R_DMA_CH6_CMD, cmd, start);
	*R_DMA_CH7_CMD = IO_STATE(R_DMA_CH7_CMD, cmd, start);
	
	while(*R_DMA_CH7_CMD == 1) /* wait for completion */ ;

	D(printk("done\n"));

}



