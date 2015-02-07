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

#ifndef _ASM_IA64_MACHVEC_HPZX1_h
#define _ASM_IA64_MACHVEC_HPZX1_h

extern ia64_mv_setup_t			dig_setup;
extern ia64_mv_dma_alloc_coherent	sba_alloc_coherent;
extern ia64_mv_dma_free_coherent	sba_free_coherent;
extern ia64_mv_dma_map_single_attrs	sba_map_single_attrs;
extern ia64_mv_dma_unmap_single_attrs	sba_unmap_single_attrs;
extern ia64_mv_dma_map_sg_attrs		sba_map_sg_attrs;
extern ia64_mv_dma_unmap_sg_attrs	sba_unmap_sg_attrs;
extern ia64_mv_dma_supported		sba_dma_supported;
extern ia64_mv_dma_mapping_error	sba_dma_mapping_error;

/*
 * This stuff has dual use!
 *
 * For a generic kernel, the macros are used to initialize the
 * platform's machvec structure.  When compiling a non-generic kernel,
 * the macros are used directly.
 */
#define platform_name				"hpzx1"
#define platform_setup				dig_setup
#define platform_dma_init			machvec_noop
#define platform_dma_alloc_coherent		sba_alloc_coherent
#define platform_dma_free_coherent		sba_free_coherent
#define platform_dma_map_single_attrs		sba_map_single_attrs
#define platform_dma_unmap_single_attrs		sba_unmap_single_attrs
#define platform_dma_map_sg_attrs		sba_map_sg_attrs
#define platform_dma_unmap_sg_attrs		sba_unmap_sg_attrs
#define platform_dma_sync_single_for_cpu	machvec_dma_sync_single
#define platform_dma_sync_sg_for_cpu		machvec_dma_sync_sg
#define platform_dma_sync_single_for_device	machvec_dma_sync_single
#define platform_dma_sync_sg_for_device		machvec_dma_sync_sg
#define platform_dma_supported			sba_dma_supported
#define platform_dma_mapping_error		sba_dma_mapping_error

#endif /* _ASM_IA64_MACHVEC_HPZX1_h */
