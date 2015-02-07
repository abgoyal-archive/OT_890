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
 * Platform information definitions for the CPM Uart driver.
 *
 * 2006 (c) MontaVista Software, Inc.
 * Vitaly Bordug <vbordug@ru.mvista.com>
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */

#ifndef FS_UART_PD_H
#define FS_UART_PD_H

#include <asm/types.h>

enum fs_uart_id {
	fsid_smc1_uart,
	fsid_smc2_uart,
	fsid_scc1_uart,
	fsid_scc2_uart,
	fsid_scc3_uart,
	fsid_scc4_uart,
	fs_uart_nr,
};

static inline int fs_uart_id_scc2fsid(int id)
{
    return fsid_scc1_uart + id - 1;
}

static inline int fs_uart_id_fsid2scc(int id)
{
    return id - fsid_scc1_uart + 1;
}

static inline int fs_uart_id_smc2fsid(int id)
{
    return fsid_smc1_uart + id - 1;
}

static inline int fs_uart_id_fsid2smc(int id)
{
    return id - fsid_smc1_uart + 1;
}

struct fs_uart_platform_info {
        void(*init_ioports)(struct fs_uart_platform_info *);
	/* device specific information */
	int fs_no;		/* controller index */
	char fs_type[4];        /* controller type  */
	u32 uart_clk;
	u8 tx_num_fifo;
	u8 tx_buf_size;
	u8 rx_num_fifo;
	u8 rx_buf_size;
	u8 brg;
	u8 clk_rx;
	u8 clk_tx;
};

static inline int fs_uart_get_id(struct fs_uart_platform_info *fpi)
{
        if(strstr(fpi->fs_type, "SMC"))
                return fs_uart_id_smc2fsid(fpi->fs_no);
        if(strstr(fpi->fs_type, "SCC"))
                return fs_uart_id_scc2fsid(fpi->fs_no);
        return fpi->fs_no;
}

#endif
