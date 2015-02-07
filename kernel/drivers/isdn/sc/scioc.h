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

#ifndef __ISDN_SC_SCIOC_H__
#define __ISDN_SC_SCIOC_H__

/*
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 */

/*
 * IOCTL Command Codes
 */
#define SCIOCLOAD	0x01	/* Load a firmware record */
#define SCIOCRESET	0x02	/* Perform hard reset */
#define SCIOCDEBUG	0x03	/* Set debug level */
#define SCIOCREV	0x04	/* Get driver revision(s) */
#define SCIOCSTART	0x05	/* Start the firmware */
#define SCIOCGETSWITCH	0x06	/* Get switch type */
#define SCIOCSETSWITCH	0x07	/* Set switch type */
#define SCIOCGETSPID	0x08	/* Get channel SPID */
#define SCIOCSETSPID	0x09 	/* Set channel SPID */
#define SCIOCGETDN	0x0A	/* Get channel DN */
#define SCIOCSETDN	0x0B 	/* Set channel DN */
#define SCIOCTRACE	0x0C	/* Toggle trace mode */
#define SCIOCSTAT	0x0D	/* Get line status */
#define SCIOCGETSPEED	0x0E	/* Set channel speed */
#define SCIOCSETSPEED	0x0F	/* Set channel speed */
#define SCIOCLOOPTST	0x10	/* Perform loopback test */

typedef struct {
	int device;
	int channel;
	unsigned long command;
	void __user *dataptr;
} scs_ioctl;

/* Size of strings */
#define SCIOC_SPIDSIZE		49
#define SCIOC_DNSIZE		SCIOC_SPIDSIZE
#define SCIOC_REVSIZE		SCIOC_SPIDSIZE
#define SCIOC_SRECSIZE		49

typedef struct {
	unsigned long tx_good;
	unsigned long tx_bad;
	unsigned long rx_good;
	unsigned long rx_bad;
} ChLinkStats;

typedef struct {
	char spid[49];
	char dn[49];
	char call_type;
	char phy_stat;
	ChLinkStats link_stats;
} BRIStat;

typedef BRIStat POTStat;

typedef struct {
	char call_type;
	char call_state;
	char serv_state;
	char phy_stat;
	ChLinkStats link_stats;
} PRIStat;

typedef char PRIInfo;
typedef char BRIInfo;
typedef char POTInfo;


typedef struct {
	char acfa_nos;
	char acfa_ais;
	char acfa_los;
	char acfa_rra;
	char acfa_slpp;
	char acfa_slpn;
	char acfa_fsrf;
} ACFAStat;

typedef struct {
	unsigned char modelid;
	char serial_no[13];
	char part_no[13];
	char load_ver[11];
	char proc_ver[11];
	int iobase;
	long rambase;
	char irq;
	long ramsize;
	char interface;
	char switch_type;
	char l1_status;
	char l2_status;
	ChLinkStats dch_stats;
	ACFAStat AcfaStats;
	union {
		PRIStat pristats[23];
		BRIStat bristats[2];
		POTStat potsstats[2];
	} status;
	union {
		PRIInfo priinfo;
		BRIInfo briinfo;
		POTInfo potsinfo;
	} info;
} boardInfo;

#endif  /*  __ISDN_SC_SCIOC_H__  */

