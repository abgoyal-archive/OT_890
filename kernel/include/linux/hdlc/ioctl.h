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

#ifndef __HDLC_IOCTL_H__
#define __HDLC_IOCTL_H__


#define GENERIC_HDLC_VERSION 4	/* For synchronization with sethdlc utility */

#define CLOCK_DEFAULT   0	/* Default setting */
#define CLOCK_EXT	1	/* External TX and RX clock - DTE */
#define CLOCK_INT	2	/* Internal TX and RX clock - DCE */
#define CLOCK_TXINT	3	/* Internal TX and external RX clock */
#define CLOCK_TXFROMRX	4	/* TX clock derived from external RX clock */


#define ENCODING_DEFAULT	0 /* Default setting */
#define ENCODING_NRZ		1
#define ENCODING_NRZI		2
#define ENCODING_FM_MARK	3
#define ENCODING_FM_SPACE	4
#define ENCODING_MANCHESTER	5


#define PARITY_DEFAULT		0 /* Default setting */
#define PARITY_NONE		1 /* No parity */
#define PARITY_CRC16_PR0	2 /* CRC16, initial value 0x0000 */
#define PARITY_CRC16_PR1	3 /* CRC16, initial value 0xFFFF */
#define PARITY_CRC16_PR0_CCITT	4 /* CRC16, initial 0x0000, ITU-T version */
#define PARITY_CRC16_PR1_CCITT	5 /* CRC16, initial 0xFFFF, ITU-T version */
#define PARITY_CRC32_PR0_CCITT	6 /* CRC32, initial value 0x00000000 */
#define PARITY_CRC32_PR1_CCITT	7 /* CRC32, initial value 0xFFFFFFFF */

#define LMI_DEFAULT		0 /* Default setting */
#define LMI_NONE		1 /* No LMI, all PVCs are static */
#define LMI_ANSI		2 /* ANSI Annex D */
#define LMI_CCITT		3 /* ITU-T Annex A */
#define LMI_CISCO		4 /* The "original" LMI, aka Gang of Four */

typedef struct { 
	unsigned int clock_rate; /* bits per second */
	unsigned int clock_type; /* internal, external, TX-internal etc. */
	unsigned short loopback;
} sync_serial_settings;          /* V.35, V.24, X.21 */

typedef struct { 
	unsigned int clock_rate; /* bits per second */
	unsigned int clock_type; /* internal, external, TX-internal etc. */
	unsigned short loopback;
	unsigned int slot_map;
} te1_settings;                  /* T1, E1 */

typedef struct {
	unsigned short encoding;
	unsigned short parity;
} raw_hdlc_proto;

typedef struct {
	unsigned int t391;
	unsigned int t392;
	unsigned int n391;
	unsigned int n392;
	unsigned int n393;
	unsigned short lmi;
	unsigned short dce; /* 1 for DCE (network side) operation */
} fr_proto;

typedef struct {
	unsigned int dlci;
} fr_proto_pvc;          /* for creating/deleting FR PVCs */

typedef struct {
	unsigned int dlci;
	char master[IFNAMSIZ];	/* Name of master FRAD device */
}fr_proto_pvc_info;		/* for returning PVC information only */

typedef struct {
    unsigned int interval;
    unsigned int timeout;
} cisco_proto;

/* PPP doesn't need any info now - supply length = 0 to ioctl */

#endif /* __HDLC_IOCTL_H__ */
