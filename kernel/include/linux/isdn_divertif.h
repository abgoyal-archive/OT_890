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
 * Header for the diversion supplementary interface for i4l.
 *
 * Author    Werner Cornelius (werner@titro.de)
 * Copyright by Werner Cornelius (werner@titro.de)
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */


/***********************************************************/
/* magic value is also used to control version information */
/***********************************************************/
#define DIVERT_IF_MAGIC 0x25873401
#define DIVERT_CMD_REG  0x00  /* register command */
#define DIVERT_CMD_REL  0x01  /* release command */
#define DIVERT_NO_ERR   0x00  /* return value no error */
#define DIVERT_CMD_ERR  0x01  /* invalid cmd */
#define DIVERT_VER_ERR  0x02  /* magic/version invalid */
#define DIVERT_REG_ERR  0x03  /* module already registered */
#define DIVERT_REL_ERR  0x04  /* module not registered */
#define DIVERT_REG_NAME isdn_register_divert

#ifdef __KERNEL__
#include <linux/isdnif.h>
#include <linux/types.h>

/***************************************************************/
/* structure exchanging data between isdn hl and divert module */
/***************************************************************/ 
typedef struct
  { ulong if_magic; /* magic info and version */
    int cmd; /* command */
    int (*stat_callback)(isdn_ctrl *); /* supplied by divert module when calling */
    int (*ll_cmd)(isdn_ctrl *); /* supplied by hl on return */
    char * (*drv_to_name)(int); /* map a driver id to name, supplied by hl */
    int (*name_to_drv)(char *); /* map a driver id to name, supplied by hl */
  } isdn_divert_if;

/*********************/
/* function register */
/*********************/
extern int DIVERT_REG_NAME(isdn_divert_if *);
#endif
