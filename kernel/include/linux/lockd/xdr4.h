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
 * linux/include/linux/lockd/xdr4.h
 *
 * XDR types for the NLM protocol
 *
 * Copyright (C) 1996 Olaf Kirch <okir@monad.swb.de>
 */

#ifndef LOCKD_XDR4_H
#define LOCKD_XDR4_H

#include <linux/fs.h>
#include <linux/nfs.h>
#include <linux/sunrpc/xdr.h>
#include <linux/lockd/xdr.h>

/* error codes new to NLMv4 */
#define	nlm4_deadlock		__constant_htonl(NLM_DEADLCK)
#define	nlm4_rofs		__constant_htonl(NLM_ROFS)
#define	nlm4_stale_fh		__constant_htonl(NLM_STALE_FH)
#define	nlm4_fbig		__constant_htonl(NLM_FBIG)
#define	nlm4_failed		__constant_htonl(NLM_FAILED)



int	nlm4svc_decode_testargs(struct svc_rqst *, __be32 *, struct nlm_args *);
int	nlm4svc_encode_testres(struct svc_rqst *, __be32 *, struct nlm_res *);
int	nlm4svc_decode_lockargs(struct svc_rqst *, __be32 *, struct nlm_args *);
int	nlm4svc_decode_cancargs(struct svc_rqst *, __be32 *, struct nlm_args *);
int	nlm4svc_decode_unlockargs(struct svc_rqst *, __be32 *, struct nlm_args *);
int	nlm4svc_encode_res(struct svc_rqst *, __be32 *, struct nlm_res *);
int	nlm4svc_decode_res(struct svc_rqst *, __be32 *, struct nlm_res *);
int	nlm4svc_encode_void(struct svc_rqst *, __be32 *, void *);
int	nlm4svc_decode_void(struct svc_rqst *, __be32 *, void *);
int	nlm4svc_decode_shareargs(struct svc_rqst *, __be32 *, struct nlm_args *);
int	nlm4svc_encode_shareres(struct svc_rqst *, __be32 *, struct nlm_res *);
int	nlm4svc_decode_notify(struct svc_rqst *, __be32 *, struct nlm_args *);
int	nlm4svc_decode_reboot(struct svc_rqst *, __be32 *, struct nlm_reboot *);
/*
int	nlmclt_encode_testargs(struct rpc_rqst *, u32 *, struct nlm_args *);
int	nlmclt_encode_lockargs(struct rpc_rqst *, u32 *, struct nlm_args *);
int	nlmclt_encode_cancargs(struct rpc_rqst *, u32 *, struct nlm_args *);
int	nlmclt_encode_unlockargs(struct rpc_rqst *, u32 *, struct nlm_args *);
 */
extern struct rpc_version nlm_version4;

#endif /* LOCKD_XDR4_H */
