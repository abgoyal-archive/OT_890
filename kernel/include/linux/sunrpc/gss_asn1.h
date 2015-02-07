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
 *  linux/include/linux/sunrpc/gss_asn1.h
 *
 *  minimal asn1 for generic encoding/decoding of gss tokens
 *
 *  Adapted from MIT Kerberos 5-1.2.1 lib/include/krb5.h,
 *  lib/gssapi/krb5/gssapiP_krb5.h, and others
 *
 *  Copyright (c) 2000 The Regents of the University of Michigan.
 *  All rights reserved.
 *
 *  Andy Adamson   <andros@umich.edu>
 */

/*
 * Copyright 1995 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 *
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  Furthermore if you modify this software you must label
 * your software as modified software and not distribute it in such a
 * fashion that it might be confused with the original M.I.T. software.
 * M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 *
 */


#include <linux/sunrpc/gss_api.h>

#define SIZEOF_INT 4

/* from gssapi_err_generic.h */
#define G_BAD_SERVICE_NAME                       (-2045022976L)
#define G_BAD_STRING_UID                         (-2045022975L)
#define G_NOUSER                                 (-2045022974L)
#define G_VALIDATE_FAILED                        (-2045022973L)
#define G_BUFFER_ALLOC                           (-2045022972L)
#define G_BAD_MSG_CTX                            (-2045022971L)
#define G_WRONG_SIZE                             (-2045022970L)
#define G_BAD_USAGE                              (-2045022969L)
#define G_UNKNOWN_QOP                            (-2045022968L)
#define G_NO_HOSTNAME                            (-2045022967L)
#define G_BAD_HOSTNAME                           (-2045022966L)
#define G_WRONG_MECH                             (-2045022965L)
#define G_BAD_TOK_HEADER                         (-2045022964L)
#define G_BAD_DIRECTION                          (-2045022963L)
#define G_TOK_TRUNC                              (-2045022962L)
#define G_REFLECT                                (-2045022961L)
#define G_WRONG_TOKID                            (-2045022960L)

#define g_OID_equal(o1,o2) \
   (((o1)->len == (o2)->len) && \
    (memcmp((o1)->data,(o2)->data,(int) (o1)->len) == 0))

u32 g_verify_token_header(
     struct xdr_netobj *mech,
     int *body_size,
     unsigned char **buf_in,
     int toksize);

int g_token_size(
     struct xdr_netobj *mech,
     unsigned int body_size);

void g_make_token_header(
     struct xdr_netobj *mech,
     int body_size,
     unsigned char **buf);
