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
 * ISDN lowlevel-module for AVM B1-card.
 *
 * Copyright 1996 by Carsten Paeth (calle@calle.in-berlin.de)
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#ifndef _B1LLI_H_
#define _B1LLI_H_
/*
 * struct for loading t4 file 
 */
typedef struct avmb1_t4file {
	int len;
	unsigned char *data;
} avmb1_t4file;

typedef struct avmb1_loaddef {
	int contr;
	avmb1_t4file t4file;
} avmb1_loaddef;

typedef struct avmb1_loadandconfigdef {
	int contr;
	avmb1_t4file t4file;
        avmb1_t4file t4config; 
} avmb1_loadandconfigdef;

typedef struct avmb1_resetdef {
	int contr;
} avmb1_resetdef;

typedef struct avmb1_getdef {
	int contr;
	int cardtype;
	int cardstate;
} avmb1_getdef;

/*
 * struct for adding new cards 
 */
typedef struct avmb1_carddef {
	int port;
	int irq;
} avmb1_carddef;

#define AVM_CARDTYPE_B1		0
#define AVM_CARDTYPE_T1		1
#define AVM_CARDTYPE_M1		2
#define AVM_CARDTYPE_M2		3

typedef struct avmb1_extcarddef {
	int port;
	int irq;
        int cardtype;
        int cardnr;  /* for HEMA/T1 */
} avmb1_extcarddef;

#define	AVMB1_LOAD		0	/* load image to card */
#define AVMB1_ADDCARD		1	/* add a new card - OBSOLETE */
#define AVMB1_RESETCARD		2	/* reset a card */
#define	AVMB1_LOAD_AND_CONFIG	3	/* load image and config to card */
#define	AVMB1_ADDCARD_WITH_TYPE	4	/* add a new card, with cardtype */
#define AVMB1_GET_CARDINFO	5	/* get cardtype */
#define AVMB1_REMOVECARD	6	/* remove a card - OBSOLETE */

#define	AVMB1_REGISTERCARD_IS_OBSOLETE

#endif				/* _B1LLI_H_ */
