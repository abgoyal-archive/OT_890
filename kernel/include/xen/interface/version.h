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

/******************************************************************************
 * version.h
 *
 * Xen version, type, and compile information.
 *
 * Copyright (c) 2005, Nguyen Anh Quynh <aquynh@gmail.com>
 * Copyright (c) 2005, Keir Fraser <keir@xensource.com>
 */

#ifndef __XEN_PUBLIC_VERSION_H__
#define __XEN_PUBLIC_VERSION_H__

/* NB. All ops return zero on success, except XENVER_version. */

/* arg == NULL; returns major:minor (16:16). */
#define XENVER_version      0

/* arg == xen_extraversion_t. */
#define XENVER_extraversion 1
struct xen_extraversion {
    char extraversion[16];
};
#define XEN_EXTRAVERSION_LEN (sizeof(struct xen_extraversion))

/* arg == xen_compile_info_t. */
#define XENVER_compile_info 2
struct xen_compile_info {
    char compiler[64];
    char compile_by[16];
    char compile_domain[32];
    char compile_date[32];
};

#define XENVER_capabilities 3
struct xen_capabilities_info {
    char info[1024];
};
#define XEN_CAPABILITIES_INFO_LEN (sizeof(struct xen_capabilities_info))

#define XENVER_changeset 4
struct xen_changeset_info {
    char info[64];
};
#define XEN_CHANGESET_INFO_LEN (sizeof(struct xen_changeset_info))

#define XENVER_platform_parameters 5
struct xen_platform_parameters {
    unsigned long virt_start;
};

#define XENVER_get_features 6
struct xen_feature_info {
    unsigned int submap_idx;    /* IN: which 32-bit submap to return */
    uint32_t     submap;        /* OUT: 32-bit submap */
};

/* Declares the features reported by XENVER_get_features. */
#include "features.h"

#endif /* __XEN_PUBLIC_VERSION_H__ */
