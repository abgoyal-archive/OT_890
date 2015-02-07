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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   mt6516_h264_kernel_driver.h
 *
 * Project:
 * --------
 *   MT6516
 *
 * Description:
 * ------------
 *   MT6516 h264 driver 
 *
 * Author:
 * -------
 *   Jackal Chen   (mtk02532)
 *
 ****************************************************************************/

#ifndef __MT6516_H264_KERNEL_DRIVER_H__
#define __MT6516_H264_KERNEL_DRIVER_H__

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <linux/jiffies.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <asm/page.h>
#include <linux/vmalloc.h>
#include <linux/interrupt.h>
#include <mach/irqs.h>
#include <linux/wait.h>
#include <mach/mt6516_pll.h>
#include <mach/mt6516_typedefs.h> 
#include <mach/mt6516_reg_base.h> 
#include <mach/mt6516_mm_mem.h>
#include <asm/cacheflush.h>
#include <asm/tcm.h>
#include "mt6516_h264_hw.h"

#define H264_DEC_DEVNAME "MT6516_H264_DEC"

#define HW_WRITE(ptr,data) (*(ptr) = (data))
#define HW_READ(ptr)       (*(ptr))

//#define MT6516_H264_DEC_DEBUG
#ifdef MT6516_H264_DEC_DEBUG
#define H264_DEC_DEBUG printk
#else
#define H264_DEC_DEBUG(x,...)
#endif

#define MT6516_H264_DEC
#ifdef MT6516_H264_DEC
#define H264_DEC printk
#else
#define H264_DEC(x,...)
#endif

#define DMA_ALLOC(ptr,dev,size,hdl,flag) \
do { \
    ptr = dma_alloc_coherent(dev, size, hdl, flag); \
    BUG_ON(!ptr); \
    H264_DEC("[H264_DEC_MEM_ALLOC] Alloc_vaddr=0x%08x, Alloc_paddr=0x%08x, Alloc_size=%d, LINE:%d\n", ptr, *hdl, size, __LINE__); \
} while (0)

#define DMA_FREE(dev, size, ptr, hdl)\
do { \
    H264_DEC("[H264_DEC_MEM_FREE] Free_addr=0x%08x, Alloc_size=%d, LINE:%d\n", ptr, size, __LINE__); \
    dma_free_coherent(dev, size, ptr, hdl); \
} while (0)

/*
#define DMA_ALLOC(ptr,dev,size,hdl,flag) \
do { \
    ptr = dma_alloc_coherent(dev, size, hdl, flag); \
    BUG_ON(!ptr); \
} while (0)

#define DMA_FREE(dev, size, ptr, hdl)\
do { \
    dma_free_coherent(dev, size, ptr, hdl); \
} while (0)
*/

typedef enum 
{
    H264_DEC_SET_INTERNAL_SRAM = 0,
    H264_DEC_WAIT_DECODE_DONE = 1,
    H264_DEC_RST = 2,
    H264_DEC_GET_IRQ_STS_SET_ACK = 3,
    H264_DEC_RELOAD_VLC_DMA = 4,
    H264_DEC_SET_SLICE_MAP_ADDR = 5,
    H264_DEC_SET_REF_FRAME_ADDR = 6,
    H264_DEC_SET_PIC_CONF = 7,
    H264_DEC_SET_SLICE_CONF = 8,
    H264_DEC_SET_DMA_LIMIT = 9,
    H264_DEC_SET_MC_LINE_BUF_OFFSET = 10,
    H264_DEC_SET_INTERNAL_SRAM_TO_HW = 11,
    H264_DEC_SET_MC_LINE_BUF_SIZE = 12,
    H264_DEC_SET_REC_ADDR = 13,
    H264_DEC_SET_REC_Y_SIZE = 14,
    H264_DEC_SET_IRQ_POS_AND_MASK = 15,
    H264_DEC_SET_HW_START = 16,
    H264_DEC_CLEAN_DCACHE = 17,
    H264_DEC_FLUSH_DCACHE = 18,
    MAX_H264_DEC_CMD = 0xFFFFFFFF
} H264_DEC_CMD;

typedef struct
{
    kal_uint32 va;
    kal_uint32 size;
}CleanDCache;

typedef struct
{
    kal_uint32 va;
    kal_uint32 size;
}FlushDCache;

#endif //__MT6516_H264_KERNEL_DRIVER_H__
