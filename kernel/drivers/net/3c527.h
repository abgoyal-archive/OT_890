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
 *	3COM "EtherLink MC/32" Descriptions
 */

/*
 *	Registers
 */

#define HOST_CMD		0
#define         HOST_CMD_START_RX   (1<<3)
#define         HOST_CMD_SUSPND_RX  (3<<3)
#define         HOST_CMD_RESTRT_RX  (5<<3)

#define         HOST_CMD_SUSPND_TX  3
#define         HOST_CMD_RESTRT_TX  5


#define HOST_STATUS		2
#define		HOST_STATUS_CRR	(1<<6)
#define		HOST_STATUS_CWR	(1<<5)


#define HOST_CTRL		6
#define		HOST_CTRL_ATTN	(1<<7)
#define 	HOST_CTRL_RESET	(1<<6)
#define 	HOST_CTRL_INTE	(1<<2)

#define HOST_RAMPAGE		8

#define HALTED 0
#define RUNNING 1

struct mc32_mailbox
{
 	u16 mbox;
 	u16 data[1];
} __attribute((packed));

struct skb_header
{
	u8 status;
	u8 control;
	u16 next;	/* Do not change! */
	u16 length;
	u32 data;
} __attribute((packed));

struct mc32_stats
{
	/* RX Errors */
	u32 rx_crc_errors;
	u32 rx_alignment_errors;
	u32 rx_overrun_errors;
	u32 rx_tooshort_errors;
	u32 rx_toolong_errors;
	u32 rx_outofresource_errors;

	u32 rx_discarded;  /* via card pattern match filter */

	/* TX Errors */
	u32 tx_max_collisions;
	u32 tx_carrier_errors;
	u32 tx_underrun_errors;
	u32 tx_cts_errors;
	u32 tx_timeout_errors;

	/* various cruft */
	u32 dataA[6];
	u16 dataB[5];
	u32 dataC[14];
} __attribute((packed));

#define STATUS_MASK	0x0F
#define COMPLETED	(1<<7)
#define COMPLETED_OK	(1<<6)
#define BUFFER_BUSY	(1<<5)

#define CONTROL_EOP	(1<<7)	/* End Of Packet */
#define CONTROL_EOL	(1<<6)	/* End of List */

#define MCA_MC32_ID	0x0041	/* Our MCA ident */
