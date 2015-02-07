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
 * header for Linux ISDN subsystem, tty related functions (linklevel).
 *
 * Copyright 1994-1999  by Fritz Elfert (fritz@isdn4linux.de)
 * Copyright 1995,96    by Thinking Objects Software GmbH Wuerzburg
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */


#define DLE 0x10
#define ETX 0x03
#define DC4 0x14


/*
 * Definition of some special Registers of AT-Emulator
 */
#define REG_RINGATA   0
#define REG_RINGCNT   1  /* ring counter register */
#define REG_ESC       2
#define REG_CR        3
#define REG_LF        4
#define REG_BS        5

#define REG_WAITC     7

#define REG_RESP     12  /* show response messages register */
#define BIT_RESP      1  /* show response messages bit      */
#define REG_RESPNUM  12  /* show numeric responses register */
#define BIT_RESPNUM   2  /* show numeric responses bit      */
#define REG_ECHO     12
#define BIT_ECHO      4
#define REG_DCD      12
#define BIT_DCD       8
#define REG_CTS      12
#define BIT_CTS      16
#define REG_DTRR     12
#define BIT_DTRR     32
#define REG_DSR      12
#define BIT_DSR      64
#define REG_CPPP     12
#define BIT_CPPP    128

#define REG_DXMT     13
#define BIT_DXMT      1
#define REG_T70      13
#define BIT_T70       2
#define BIT_T70_EXT  32
#define REG_DTRHUP   13
#define BIT_DTRHUP    4
#define REG_RESPXT   13
#define BIT_RESPXT    8
#define REG_CIDONCE  13
#define BIT_CIDONCE  16
#define REG_RUNG     13  /* show RUNG message register      */
#define BIT_RUNG     64  /* show RUNG message bit           */
#define REG_DISPLAY  13
#define BIT_DISPLAY 128

#define REG_L2PROT   14
#define REG_L3PROT   15
#define REG_PSIZE    16
#define REG_WSIZE    17
#define REG_SI1      18
#define REG_SI2      19
#define REG_SI1I     20
#define REG_PLAN     21
#define REG_SCREEN   22

#define REG_CPN      23
#define BIT_CPN       1
#define REG_CPNFCON  23
#define BIT_CPNFCON   2
#define REG_CDN      23
#define BIT_CDN       4

/* defines for result codes */
#define RESULT_OK		0
#define RESULT_CONNECT		1
#define RESULT_RING		2
#define RESULT_NO_CARRIER	3
#define RESULT_ERROR		4
#define RESULT_CONNECT64000	5
#define RESULT_NO_DIALTONE	6
#define RESULT_BUSY		7
#define RESULT_NO_ANSWER	8
#define RESULT_RINGING		9
#define RESULT_NO_MSN_EAZ	10
#define RESULT_VCON		11
#define RESULT_RUNG		12

#define TTY_IS_FCLASS1(info) \
	((info->emu.mdmreg[REG_L2PROT] == ISDN_PROTO_L2_FAX) && \
	 (info->emu.mdmreg[REG_L3PROT] == ISDN_PROTO_L3_FCLASS1))
#define TTY_IS_FCLASS2(info) \
	((info->emu.mdmreg[REG_L2PROT] == ISDN_PROTO_L2_FAX) && \
	 (info->emu.mdmreg[REG_L3PROT] == ISDN_PROTO_L3_FCLASS2))

extern void isdn_tty_modem_escape(void);
extern void isdn_tty_modem_ring(void);
extern void isdn_tty_carrier_timeout(void);
extern void isdn_tty_modem_xmit(void);
extern int  isdn_tty_modem_init(void);
extern void isdn_tty_exit(void);
extern void isdn_tty_readmodem(void);
extern int  isdn_tty_find_icall(int, int, setup_parm *);
extern int  isdn_tty_stat_callback(int, isdn_ctrl *);
extern int  isdn_tty_rcv_skb(int, int, int, struct sk_buff *);
extern int  isdn_tty_capi_facility(capi_msg *cm); 
extern void isdn_tty_at_cout(char *, modem_info *);
extern void isdn_tty_modem_hup(modem_info *, int);
#ifdef CONFIG_ISDN_TTY_FAX
extern int  isdn_tty_cmd_PLUSF_FAX(char **, modem_info *);
extern int  isdn_tty_fax_command(modem_info *, isdn_ctrl *);
extern void isdn_tty_fax_bitorder(modem_info *, struct sk_buff *);
#endif
