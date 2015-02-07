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
 *  Copyright (C) 2006, 2007 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef ExceptionCode_h
#define ExceptionCode_h

namespace WebCore {

    // The DOM standards use unsigned short for exception codes.
    // In our DOM implementation we use int instead, and use different
    // numerical ranges for different types of DOM exception, so that
    // an exception of any type can be expressed with a single integer.
    typedef int ExceptionCode;

    enum {
        INDEX_SIZE_ERR = 1,
        DOMSTRING_SIZE_ERR = 2,
        HIERARCHY_REQUEST_ERR = 3,
        WRONG_DOCUMENT_ERR = 4,
        INVALID_CHARACTER_ERR = 5,
        NO_DATA_ALLOWED_ERR = 6,
        NO_MODIFICATION_ALLOWED_ERR = 7,
        NOT_FOUND_ERR = 8,
        NOT_SUPPORTED_ERR = 9,
        INUSE_ATTRIBUTE_ERR = 10,

        // Introduced in DOM Level 2:
        INVALID_STATE_ERR = 11,
        SYNTAX_ERR = 12,
        INVALID_MODIFICATION_ERR = 13,
        NAMESPACE_ERR = 14,
        INVALID_ACCESS_ERR = 15,

        // Introduced in DOM Level 3:
        VALIDATION_ERR = 16,
        TYPE_MISMATCH_ERR = 17,

        // XMLHttpRequest extension:
        SECURITY_ERR = 18,

        // Others introduced in HTML5:
        NETWORK_ERR = 19,
        ABORT_ERR = 20,
        URL_MISMATCH_ERR = 21,
        QUOTA_EXCEEDED_ERR = 22,
    };

    enum ExceptionType {
        DOMExceptionType,
        RangeExceptionType,
        EventExceptionType,
        XMLHttpRequestExceptionType
#if ENABLE(XPATH)
        , XPathExceptionType
#endif
#if ENABLE(SVG)
        , SVGExceptionType
#endif
    };        
    

    struct ExceptionCodeDescription {
        const char* typeName; // has spaces and is suitable for use in exception description strings; maximum length is 10 characters
        const char* name; // exception name, also intended for use in exception description strings; 0 if name not known; maximum length is 27 characters
        int code; // numeric value of the exception within a particular type
        ExceptionType type;
    };
    void getExceptionCodeDescription(ExceptionCode, ExceptionCodeDescription&);

} // namespace WebCore

#endif // ExceptionCode_h
