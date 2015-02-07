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
 * Details of the "wire" protocol between Xen Store Daemon and client
 * library or guest kernel.
 * Copyright (C) 2005 Rusty Russell IBM Corporation
 */

#ifndef _XS_WIRE_H
#define _XS_WIRE_H

enum xsd_sockmsg_type
{
    XS_DEBUG,
    XS_DIRECTORY,
    XS_READ,
    XS_GET_PERMS,
    XS_WATCH,
    XS_UNWATCH,
    XS_TRANSACTION_START,
    XS_TRANSACTION_END,
    XS_INTRODUCE,
    XS_RELEASE,
    XS_GET_DOMAIN_PATH,
    XS_WRITE,
    XS_MKDIR,
    XS_RM,
    XS_SET_PERMS,
    XS_WATCH_EVENT,
    XS_ERROR,
    XS_IS_DOMAIN_INTRODUCED
};

#define XS_WRITE_NONE "NONE"
#define XS_WRITE_CREATE "CREATE"
#define XS_WRITE_CREATE_EXCL "CREATE|EXCL"

/* We hand errors as strings, for portability. */
struct xsd_errors
{
    int errnum;
    const char *errstring;
};
#define XSD_ERROR(x) { x, #x }
static struct xsd_errors xsd_errors[] __attribute__((unused)) = {
    XSD_ERROR(EINVAL),
    XSD_ERROR(EACCES),
    XSD_ERROR(EEXIST),
    XSD_ERROR(EISDIR),
    XSD_ERROR(ENOENT),
    XSD_ERROR(ENOMEM),
    XSD_ERROR(ENOSPC),
    XSD_ERROR(EIO),
    XSD_ERROR(ENOTEMPTY),
    XSD_ERROR(ENOSYS),
    XSD_ERROR(EROFS),
    XSD_ERROR(EBUSY),
    XSD_ERROR(EAGAIN),
    XSD_ERROR(EISCONN)
};

struct xsd_sockmsg
{
    uint32_t type;  /* XS_??? */
    uint32_t req_id;/* Request identifier, echoed in daemon's response.  */
    uint32_t tx_id; /* Transaction id (0 if not related to a transaction). */
    uint32_t len;   /* Length of data following this. */

    /* Generally followed by nul-terminated string(s). */
};

enum xs_watch_type
{
    XS_WATCH_PATH = 0,
    XS_WATCH_TOKEN
};

/* Inter-domain shared memory communications. */
#define XENSTORE_RING_SIZE 1024
typedef uint32_t XENSTORE_RING_IDX;
#define MASK_XENSTORE_IDX(idx) ((idx) & (XENSTORE_RING_SIZE-1))
struct xenstore_domain_interface {
    char req[XENSTORE_RING_SIZE]; /* Requests to xenstore daemon. */
    char rsp[XENSTORE_RING_SIZE]; /* Replies and async watch events. */
    XENSTORE_RING_IDX req_cons, req_prod;
    XENSTORE_RING_IDX rsp_cons, rsp_prod;
};

#endif /* _XS_WIRE_H */
