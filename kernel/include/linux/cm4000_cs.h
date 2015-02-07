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

#ifndef	_CM4000_H_
#define	_CM4000_H_

#define	MAX_ATR			33

#define	CM4000_MAX_DEV		4

/* those two structures are passed via ioctl() from/to userspace.  They are
 * used by existing userspace programs, so I kepth the awkward "bIFSD" naming
 * not to break compilation of userspace apps. -HW */

typedef struct atreq {
	int32_t atr_len;
	unsigned char atr[64];
	int32_t power_act;
	unsigned char bIFSD;
	unsigned char bIFSC;
} atreq_t;


/* what is particularly stupid in the original driver is the arch-dependant
 * member sizes. This leads to CONFIG_COMPAT breakage, since 32bit userspace
 * will lay out the structure members differently than the 64bit kernel.
 *
 * I've changed "ptsreq.protocol" from "unsigned long" to "u_int32_t".
 * On 32bit this will make no difference.  With 64bit kernels, it will make
 * 32bit apps work, too.
 */

typedef struct ptsreq {
	u_int32_t protocol; /*T=0: 2^0, T=1:  2^1*/
 	unsigned char flags;
 	unsigned char pts1;
 	unsigned char pts2;
	unsigned char pts3;
} ptsreq_t;

#define	CM_IOC_MAGIC		'c'
#define	CM_IOC_MAXNR	        255

#define	CM_IOCGSTATUS		_IOR (CM_IOC_MAGIC, 0, unsigned char *)
#define	CM_IOCGATR		_IOWR(CM_IOC_MAGIC, 1, atreq_t *)
#define	CM_IOCSPTS		_IOW (CM_IOC_MAGIC, 2, ptsreq_t *)
#define	CM_IOCSRDR		_IO  (CM_IOC_MAGIC, 3)
#define CM_IOCARDOFF            _IO  (CM_IOC_MAGIC, 4)

#define CM_IOSDBGLVL            _IOW(CM_IOC_MAGIC, 250, int*)

/* card and device states */
#define	CM_CARD_INSERTED		0x01
#define	CM_CARD_POWERED			0x02
#define	CM_ATR_PRESENT			0x04
#define	CM_ATR_VALID	 		0x08
#define	CM_STATE_VALID			0x0f
/* extra info only from CM4000 */
#define	CM_NO_READER			0x10
#define	CM_BAD_CARD			0x20


#ifdef __KERNEL__

#define	DEVICE_NAME		"cmm"
#define	MODULE_NAME		"cm4000_cs"

#endif	/* __KERNEL__ */
#endif	/* _CM4000_H_ */
