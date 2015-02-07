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

#ifndef __ASM_ARCH_DMA_H
#define __ASM_ARCH_DMA_H

#define MAX_DMA_ADDRESS		0xffffffff
#define MAX_DMA_CHANNELS	0

#endif

#ifndef MT_DMA_H
#define MT_DMA_H

#include <linux/types.h>

typedef u32 INFO;

typedef enum{
    DMA_FALSE = 0,
    DMA_TRUE
}DMA_BOOL;

typedef enum{
    DMA_FULL_CHANNEL = 0,
    DMA_HALF_CHANNEL,
    DMA_VIRTUAL_FIFO,
    DMA_VIRTUAL_FIFO_UART0,
    DMA_VIRTUAL_FIFO_UART1,
    DMA_VIRTUAL_FIFO_UART2,
    DMA_VIRTUAL_FIFO_UART3,
    DMA_VIRTUAL_FIFO_UART4,
    DMA_VIRTUAL_FIFO_UART5
}DMA_TYPE;

typedef enum{
    DMA_OK = 0,
    DMA_FAIL
}DMA_STATUS;

typedef enum{
    REMAINING_LENGTH = 0, /* not valid for virtual FIFO */
    VF_READPTR,           /* only valid for virtual FIFO */
    VF_WRITEPTR,          /* only valid for virtual FIFO */
    VF_FFCNT,             /* only valid for virtual FIFO */
    VF_ALERT,             /* only valid for virtual FIFO */
    VF_EMPTY,             /* only valid for virtual FIFO */
    VF_FULL,              /* only valid for virtual FIFO */
    VF_PORT
}INFO_TYPE;

typedef enum{
    ALL = 0,
    SRC,
    DST,
    SRC_AND_DST
}DMA_CONF_FLAG;

/* MASTER */   
#define DMA_CON_MASTER_MSDC0         	0x00000000
#define DMA_CON_MASTER_MSDC1            0x00100000
#define DMA_CON_MASTER_MSDC2       	    0x00200000
#define DMA_CON_MASTER_IRDATX       	0x00300000
#define DMA_CON_MASTER_IRDARX       	0x00400000
#define DMA_CON_MASTER_UART0TX      	0x00500000
#define DMA_CON_MASTER_UART0RX      	0x00600000
#define DMA_CON_MASTER_UART1TX      	0x00700000
#define DMA_CON_MASTER_UART1RX      	0x00800000
#define DMA_CON_MASTER_UART2TX      	0x00900000
#define DMA_CON_MASTER_UART2RX      	0x00a00000
#define DMA_CON_MASTER_UART3TX      	0x00b00000
#define DMA_CON_MASTER_UART3RX      	0x00c00000
#define DMA_CON_MASTER_UART4TX      	0x00d00000
#define DMA_CON_MASTER_UART4RX      	0x00e00000
#define DMA_CON_MASTER_I2CTX         	0x00f00000
#define DMA_CON_MASTER_I2CRX            0x01000000

/* burst */
#define DMA_CON_BURST_SINGLE        	0x00000000  /*without burst mode*/
#define DMA_CON_BURST_4BEAT         	0x00000200  /*4-beat incrementing burst*/
#define DMA_CON_BURST_8BEAT         	0x00000400  /*8-beat incrementing burst*/
#define DMA_CON_BURST_16BEAT        	0x00000600  /*16-beat incrementing burst*/

/* size */                        
#define DMA_CON_SIZE_BYTE           	0x00000000
#define DMA_CON_SIZE_SHORT          	0x00000001
#define DMA_CON_SIZE_LONG           	0x00000002

/*
 *  configuration structure
 */

/*
 *  Full-Size    Half-Size    Virtual FIFO
 *          -            -               -
 *        SRC            -               -
 *        DST            -               -
 *       WPPT         WPPT               -
 *       WPTO         WPTO               - 
 *      COUNT        COUNT           COUNT
 *        CON          CON             CON
 *      START        START           START
 *     INTSTA       INTSTA          INTSTA
 *     ACKINT       ACKINT          ACKINT
 *       RLCT         RLCT            RLCT
 *    LIMITER      LIMITER         LIMITER
 *          -      PGMADDR         PGMADDR
 *          -            -           WRPTR
 *          -            -           RDPTR
 *          -            -           FFCNT
 *          -            -           FFSTA
 *          -            -          ALTLEN
 *          -            -          FFSIZE
 */

/*
 * ========================================
 * Definition
 * ========================================
 *    count: For full-size DMA channel and half-size DMA channel, it means the
 *           amount of data of current transfer. But for virtual FIFO, it means the
 *           amount of data stored before the interrupt is issued.
 *      mas: Master. For half-size DMA channel and virtual FIFO, after the 
 *           master is determined, the address of one side is also determined. 
 *           For full-size DMA channel, master is used only when handshaking is 
 *           required. Use the symbolic symbols defined above this file to pass 
 *           this parameter.
 *     iten: TRUE: interrupt mode, FALSE: polling mode
 *    burst: Burst mode, use the symbolic symbols defined above to pass this 
 *           parameter. It means the number of beats before DMA controller 
 *           releases the bus.
 *     dreq: TRUE: enable hardware handshaking, FALSE: disable hardware 
 *           handshaking.
 *     dinc: TRUE: destination address increases after a transfer. FALSE: 
 *           doesn't increase.
 *     sinc: TRUE: source address increases after a transfer. FALSE: doesn't 
 *           increase.
 *     size: Transfer size per bus cycle. Note that size * burst_beat cannot 
 *           exceed 16 bytes.
 *  limiter: Used to suppress DMA bus utilization. Range: 0 ~ 255. Default: 0.
 *           At this time, simply pass 0 would be ok.
 *     data: The user data to be used in callback function.
 * callback: If the DMA channel is set to interrupt mode, then this function 
 *           will be called when DMA transfer is complete.
 * src, dst: Only valid for full-size DMA channel, used to set source and 
 *           destination addresses.
 *     wpen: Wrapping function enable/disable. TRUE: enable, FALSE: disable.
 *     wpsd: Choose the wrapping side. 0: wrap at source, 1: wrap at 
 *           destination.
 *     wppt: When the wrapping function is enabled, this value determines the amount of 
 *           transfer count from start address to jumping point.
 *     wpto: When the wrapping function is enabled, this value determines the 
 *           address where DMA transfer jumps to.
 *  pgmaddr: Only valid for half-size DMA channel and virtual FIFO. The address
 *           that could be chosen by programmer.
 *      dir: Only valid for half-size DMA channel and virtual FIFO. Indicates
 *           the data flow direction. write: from master to pgmaddr, read: from
 *           pgmaddr to master. 0: read, 1: write.
 *      b2w: Only valid for half-size DMA channel. Determines whether the 
 *           channel supports non-word-aligned-address data access. 0: disable, 
 *           1: enable.
 *   altlen: Only valid for virtual FIFO. If the amount of data stored in 
 *           virtual exceeds this value, then DMA controller will issue an 
 *           alert signal to activate UART flow control is UART module supports 
 *           it.
 *   ffsize: Only valid for virtual FIFO. Determines the size of virtual FIFO.
 */

struct mt_dma_conf{             /*   full-size    half-size    virtual-FIFO */
    
    u16 count;                  /*           o            o               o */
    u32   mas;                  /*           o            o               o */
    DMA_BOOL iten;              /*           o            o               o */
    u32 burst;                  /*           o            o               o */
    DMA_BOOL dreq;              /*           o            o               o */
    DMA_BOOL dinc;              /*           o            o               o */
    DMA_BOOL sinc;              /*           o            o               o */
    u8 size;                    /*           o            o               o */
    u8 limiter;                 /*           o            o               o */
    void *data;                 /*           o            o               o */
    void (*callback)(void *);   /*           o            o               o */  
    u32 src;                    /*           o            x               x */
    u32 dst;                    /*           o            x               x */
    DMA_BOOL wpen;              /*           o            o               x */
    DMA_BOOL wpsd;              /*           o            o               x */
    u16 wppt;                   /*           o            o               x */
    u32 wpto;                   /*           o            o               x */
    u32 pgmaddr;                /*           x            o               o */
    DMA_BOOL dir;               /*           x            o               o */
    DMA_BOOL b2w;               /*           x            o               x */
    u8 altlen;                  /*           x            x               o */
    u16 ffsize;                 /*           x            x               o */
};

/* 
 * Function: 
 *    Request a DMA channel of the given type. 
 */
struct mt_dma_conf *mt_request_dma(DMA_TYPE type);
/*
 * Function: 
 *   Free the DMA channel previously granted.
 */
void mt_free_dma(struct mt_dma_conf *);
/*
 * Function: 
 *   Activate DMA transfer by the settings written into DMA controller
 *   registers. Note that before calling this function, you must call 
 *   mt_config_dma function to write the configuration stored in struct mt_dma_conf
 *   back into DMA controller registers.
 */
void mt_start_dma(struct mt_dma_conf *);
/*
 * Function:
 *   Stop DMA transfer of the granted channel.
 */
void mt_stop_dma(struct mt_dma_conf *);

/*
 * Function:
 *   Write the configuration stored in struct mt_dma_conf back into DMA 
 *   controller registers.
 */
DMA_STATUS mt_config_dma(struct mt_dma_conf *, DMA_CONF_FLAG flag);

/*
 * Function:
 *    Reset the configuration of the granted DMA channel. 
 */
void mt_reset_dma(struct mt_dma_conf *);

/*
 * Function:
 *    Get information of the granted DMA channel. The information is stored in
 *    INFO *info.
 * Returns:
 *    Whether the query succeeds.
 *       DMA_SUCCESS: success
 *       DMA_FAIL: fail
 */
DMA_STATUS mt_get_info(struct mt_dma_conf *, INFO_TYPE type, INFO *info);

#endif

