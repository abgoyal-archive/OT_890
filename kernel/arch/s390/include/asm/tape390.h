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

/*************************************************************************
 *
 * tape390.h
 *	   enables user programs to display messages and control encryption
 *	   on s390 tape devices
 *
 *	   Copyright IBM Corp. 2001,2006
 *	   Author(s): Michael Holzheu <holzheu@de.ibm.com>
 *
 *************************************************************************/

#ifndef _TAPE390_H
#define _TAPE390_H

#define TAPE390_DISPLAY _IOW('d', 1, struct display_struct)

/*
 * The TAPE390_DISPLAY ioctl calls the Load Display command
 * which transfers 17 bytes of data from the channel to the subsystem:
 *     - 1 format control byte, and
 *     - two 8-byte messages
 *
 * Format control byte:
 *   0-2: New Message Overlay
 *     3: Alternate Messages
 *     4: Blink Message
 *     5: Display Low/High Message
 *     6: Reserved
 *     7: Automatic Load Request
 *
 */

typedef struct display_struct {
        char cntrl;
        char message1[8];
        char message2[8];
} display_struct;

/*
 * Tape encryption support
 */

struct tape390_crypt_info {
	char capability;
	char status;
	char medium_status;
} __attribute__ ((packed));


/* Macros for "capable" field */
#define TAPE390_CRYPT_SUPPORTED_MASK 0x01
#define TAPE390_CRYPT_SUPPORTED(x) \
	((x.capability & TAPE390_CRYPT_SUPPORTED_MASK))

/* Macros for "status" field */
#define TAPE390_CRYPT_ON_MASK 0x01
#define TAPE390_CRYPT_ON(x) (((x.status) & TAPE390_CRYPT_ON_MASK))

/* Macros for "medium status" field */
#define TAPE390_MEDIUM_LOADED_MASK 0x01
#define TAPE390_MEDIUM_ENCRYPTED_MASK 0x02
#define TAPE390_MEDIUM_ENCRYPTED(x) \
	(((x.medium_status) & TAPE390_MEDIUM_ENCRYPTED_MASK))
#define TAPE390_MEDIUM_LOADED(x) \
	(((x.medium_status) & TAPE390_MEDIUM_LOADED_MASK))

/*
 * The TAPE390_CRYPT_SET ioctl is used to switch on/off encryption.
 * The "encryption_capable" and "tape_status" fields are ignored for this ioctl!
 */
#define TAPE390_CRYPT_SET _IOW('d', 2, struct tape390_crypt_info)

/*
 * The TAPE390_CRYPT_QUERY ioctl is used to query the encryption state.
 */
#define TAPE390_CRYPT_QUERY _IOR('d', 3, struct tape390_crypt_info)

/* Values for "kekl1/2_type" and "kekl1/2_type_on_tape" fields */
#define TAPE390_KEKL_TYPE_NONE 0
#define TAPE390_KEKL_TYPE_LABEL 1
#define TAPE390_KEKL_TYPE_HASH 2

struct tape390_kekl {
	unsigned char type;
	unsigned char type_on_tape;
	char label[65];
} __attribute__ ((packed));

struct tape390_kekl_pair {
	struct tape390_kekl kekl[2];
} __attribute__ ((packed));

/*
 * The TAPE390_KEKL_SET ioctl is used to set Key Encrypting Key labels.
 */
#define TAPE390_KEKL_SET _IOW('d', 4, struct tape390_kekl_pair)

/*
 * The TAPE390_KEKL_QUERY ioctl is used to query Key Encrypting Key labels.
 */
#define TAPE390_KEKL_QUERY _IOR('d', 5, struct tape390_kekl_pair)

#endif 
