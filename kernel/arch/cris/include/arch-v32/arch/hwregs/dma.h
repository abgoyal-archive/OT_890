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
 * DMA C definitions and help macros
 *
 */

#ifndef dma_h
#define dma_h

/* registers */ /* Really needed, since both are listed in sw.list? */
#include "dma_defs.h"


/* descriptors */

// ------------------------------------------------------------ dma_descr_group
typedef struct dma_descr_group {
  struct dma_descr_group       *next;
  unsigned                      eol        : 1;
  unsigned                      tol        : 1;
  unsigned                      bol        : 1;
  unsigned                                 : 1;
  unsigned                      intr       : 1;
  unsigned                                 : 2;
  unsigned                      en         : 1;
  unsigned                                 : 7;
  unsigned                      dis        : 1;
  unsigned                      md         : 16;
  struct dma_descr_group       *up;
  union {
    struct dma_descr_context   *context;
    struct dma_descr_group     *group;
  }                             down;
} dma_descr_group;

// ---------------------------------------------------------- dma_descr_context
typedef struct dma_descr_context {
  struct dma_descr_context     *next;
  unsigned                      eol        : 1;
  unsigned                                 : 3;
  unsigned                      intr       : 1;
  unsigned                                 : 1;
  unsigned                      store_mode : 1;
  unsigned                      en         : 1;
  unsigned                                 : 7;
  unsigned                      dis        : 1;
  unsigned                      md0        : 16;
  unsigned                      md1;
  unsigned                      md2;
  unsigned                      md3;
  unsigned                      md4;
  struct dma_descr_data        *saved_data;
  char                         *saved_data_buf;
} dma_descr_context;

// ------------------------------------------------------------- dma_descr_data
typedef struct dma_descr_data {
  struct dma_descr_data        *next;
  char                         *buf;
  unsigned                      eol        : 1;
  unsigned                                 : 2;
  unsigned                      out_eop    : 1;
  unsigned                      intr       : 1;
  unsigned                      wait       : 1;
  unsigned                                 : 2;
  unsigned                                 : 3;
  unsigned                      in_eop     : 1;
  unsigned                                 : 4;
  unsigned                      md         : 16;
  char                         *after;
} dma_descr_data;

// --------------------------------------------------------------------- macros

// enable DMA channel
#define DMA_ENABLE( inst ) \
   do { reg_dma_rw_cfg e = REG_RD( dma, inst, rw_cfg );\
        e.en = regk_dma_yes; \
        REG_WR( dma, inst, rw_cfg, e); } while( 0 )

// reset DMA channel
#define DMA_RESET( inst ) \
   do { reg_dma_rw_cfg r = REG_RD( dma, inst, rw_cfg );\
        r.en = regk_dma_no; \
        REG_WR( dma, inst, rw_cfg, r); } while( 0 )

// stop DMA channel
#define DMA_STOP( inst ) \
   do { reg_dma_rw_cfg s = REG_RD( dma, inst, rw_cfg );\
        s.stop = regk_dma_yes; \
        REG_WR( dma, inst, rw_cfg, s); } while( 0 )

// continue DMA channel operation
#define DMA_CONTINUE( inst ) \
   do { reg_dma_rw_cfg c = REG_RD( dma, inst, rw_cfg );\
        c.stop = regk_dma_no; \
        REG_WR( dma, inst, rw_cfg, c); } while( 0 )

// give stream command
#define DMA_WR_CMD( inst, cmd_par ) \
   do { reg_dma_rw_stream_cmd __x = {0}; \
	do { __x = REG_RD(dma, inst, rw_stream_cmd); } while (__x.busy); \
	__x.cmd = (cmd_par); \
	REG_WR(dma, inst, rw_stream_cmd, __x); \
   } while (0)

// load: g,c,d:burst
#define DMA_START_GROUP( inst, group_descr ) \
   do { REG_WR_INT( dma, inst, rw_group, (int) group_descr ); \
        DMA_WR_CMD( inst, regk_dma_load_g ); \
        DMA_WR_CMD( inst, regk_dma_load_c ); \
        DMA_WR_CMD( inst, regk_dma_load_d | regk_dma_burst ); \
      } while( 0 )

// load: c,d:burst
#define DMA_START_CONTEXT( inst, ctx_descr ) \
   do { REG_WR_INT( dma, inst, rw_group_down, (int) ctx_descr ); \
        DMA_WR_CMD( inst, regk_dma_load_c ); \
        DMA_WR_CMD( inst, regk_dma_load_d | regk_dma_burst ); \
      } while( 0 )

// if the DMA is at the end of the data list, the last data descr is reloaded
#define DMA_CONTINUE_DATA( inst ) \
do { reg_dma_rw_cmd c = {0}; \
     c.cont_data = regk_dma_yes;\
     REG_WR( dma, inst, rw_cmd, c ); } while( 0 )

#endif
