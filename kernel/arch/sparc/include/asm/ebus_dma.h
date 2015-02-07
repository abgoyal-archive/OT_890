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

#ifndef __ASM_SPARC_EBUS_DMA_H
#define __ASM_SPARC_EBUS_DMA_H

struct ebus_dma_info {
	spinlock_t	lock;
	void __iomem	*regs;

	unsigned int	flags;
#define EBUS_DMA_FLAG_USE_EBDMA_HANDLER		0x00000001
#define EBUS_DMA_FLAG_TCI_DISABLE		0x00000002

	/* These are only valid is EBUS_DMA_FLAG_USE_EBDMA_HANDLER is
	 * set.
	 */
	void (*callback)(struct ebus_dma_info *p, int event, void *cookie);
	void *client_cookie;
	unsigned int	irq;
#define EBUS_DMA_EVENT_ERROR	1
#define EBUS_DMA_EVENT_DMA	2
#define EBUS_DMA_EVENT_DEVICE	4

	unsigned char	name[64];
};

extern int ebus_dma_register(struct ebus_dma_info *p);
extern int ebus_dma_irq_enable(struct ebus_dma_info *p, int on);
extern void ebus_dma_unregister(struct ebus_dma_info *p);
extern int ebus_dma_request(struct ebus_dma_info *p, dma_addr_t bus_addr,
			    size_t len);
extern void ebus_dma_prepare(struct ebus_dma_info *p, int write);
extern unsigned int ebus_dma_residue(struct ebus_dma_info *p);
extern unsigned int ebus_dma_addr(struct ebus_dma_info *p);
extern void ebus_dma_enable(struct ebus_dma_info *p, int on);

#endif /* __ASM_SPARC_EBUS_DMA_H */
