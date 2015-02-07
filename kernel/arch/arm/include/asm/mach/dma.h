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
 *  arch/arm/include/asm/mach/dma.h
 *
 *  Copyright (C) 1998-2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  This header file describes the interface between the generic DMA handler
 *  (dma.c) and the architecture-specific DMA backends (dma-*.c)
 */

struct dma_struct;
typedef struct dma_struct dma_t;

struct dma_ops {
	int	(*request)(dmach_t, dma_t *);		/* optional */
	void	(*free)(dmach_t, dma_t *);		/* optional */
	void	(*enable)(dmach_t, dma_t *);		/* mandatory */
	void 	(*disable)(dmach_t, dma_t *);		/* mandatory */
	int	(*residue)(dmach_t, dma_t *);		/* optional */
	int	(*setspeed)(dmach_t, dma_t *, int);	/* optional */
	char	*type;
};

struct dma_struct {
	void		*addr;		/* single DMA address		*/
	unsigned long	count;		/* single DMA size		*/
	struct scatterlist buf;		/* single DMA			*/
	int		sgcount;	/* number of DMA SG		*/
	struct scatterlist *sg;		/* DMA Scatter-Gather List	*/

	unsigned int	active:1;	/* Transfer active		*/
	unsigned int	invalid:1;	/* Address/Count changed	*/

	dmamode_t	dma_mode;	/* DMA mode			*/
	int		speed;		/* DMA speed			*/

	unsigned int	lock;		/* Device is allocated		*/
	const char	*device_id;	/* Device name			*/

	unsigned int	dma_base;	/* Controller base address	*/
	int		dma_irq;	/* Controller IRQ		*/
	struct scatterlist cur_sg;	/* Current controller buffer	*/
	unsigned int	state;

	struct dma_ops	*d_ops;
};

/* Prototype: void arch_dma_init(dma)
 * Purpose  : Initialise architecture specific DMA
 * Params   : dma - pointer to array of DMA structures
 */
extern void arch_dma_init(dma_t *dma);

extern void isa_init_dma(dma_t *dma);
