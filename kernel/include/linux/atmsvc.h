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

/* atmsvc.h - ATM signaling kernel-demon interface definitions */
 
/* Written 1995-2000 by Werner Almesberger, EPFL LRC/ICA */
 

#ifndef _LINUX_ATMSVC_H
#define _LINUX_ATMSVC_H

#include <linux/atmapi.h>
#include <linux/atm.h>
#include <linux/atmioc.h>


#define ATMSIGD_CTRL _IO('a',ATMIOC_SPECIAL)
				/* become ATM signaling demon control socket */

enum atmsvc_msg_type { as_catch_null, as_bind, as_connect, as_accept, as_reject,
		       as_listen, as_okay, as_error, as_indicate, as_close,
		       as_itf_notify, as_modify, as_identify, as_terminate,
		       as_addparty, as_dropparty };

struct atmsvc_msg {
	enum atmsvc_msg_type type;
	atm_kptr_t vcc;
	atm_kptr_t listen_vcc;		/* indicate */
	int reply;			/* for okay and close:		   */
					/*   < 0: error before active	   */
					/*        (sigd has discarded ctx) */
					/*   ==0: success		   */
				        /*   > 0: error when active (still */
					/*        need to close)	   */
	struct sockaddr_atmpvc pvc;	/* indicate, okay (connect) */
	struct sockaddr_atmsvc local;	/* local SVC address */
	struct atm_qos qos;		/* QOS parameters */
	struct atm_sap sap;		/* SAP */
	unsigned int session;		/* for p2pm */
	struct sockaddr_atmsvc svc;	/* SVC address */
} __ATM_API_ALIGN;

/*
 * Message contents: see ftp://icaftp.epfl.ch/pub/linux/atm/docs/isp-*.tar.gz
 */

/*
 * Some policy stuff for atmsigd and for net/atm/svc.c. Both have to agree on
 * what PCR is used to request bandwidth from the device driver. net/atm/svc.c
 * tries to do better than that, but only if there's no routing decision (i.e.
 * if signaling only uses one ATM interface).
 */

#define SELECT_TOP_PCR(tp) ((tp).pcr ? (tp).pcr : \
  (tp).max_pcr && (tp).max_pcr != ATM_MAX_PCR ? (tp).max_pcr : \
  (tp).min_pcr ? (tp).min_pcr : ATM_MAX_PCR)

#endif
