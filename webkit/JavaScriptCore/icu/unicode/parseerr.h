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
**********************************************************************
*   Copyright (C) 1999-2004, International Business Machines
*   Corporation and others.  All Rights Reserved.
**********************************************************************
*   Date        Name        Description
*   03/14/00    aliu        Creation.
*   06/27/00    aliu        Change from C++ class to C struct
**********************************************************************
*/
#ifndef PARSEERR_H
#define PARSEERR_H

#include "unicode/utypes.h"


/**
 * The capacity of the context strings in UParseError.
 * @stable ICU 2.0
 */ 
enum { U_PARSE_CONTEXT_LEN = 16 };

/**
 * A UParseError struct is used to returned detailed information about
 * parsing errors.  It is used by ICU parsing engines that parse long
 * rules, patterns, or programs, where the text being parsed is long
 * enough that more information than a UErrorCode is needed to
 * localize the error.
 *
 * <p>The line, offset, and context fields are optional; parsing
 * engines may choose not to use to use them.
 *
 * <p>The preContext and postContext strings include some part of the
 * context surrounding the error.  If the source text is "let for=7"
 * and "for" is the error (e.g., because it is a reserved word), then
 * some examples of what a parser might produce are the following:
 *
 * <pre>
 * preContext   postContext
 * ""           ""            The parser does not support context
 * "let "       "=7"          Pre- and post-context only
 * "let "       "for=7"       Pre- and post-context and error text
 * ""           "for"         Error text only
 * </pre>
 *
 * <p>Examples of engines which use UParseError (or may use it in the
 * future) are Transliterator, RuleBasedBreakIterator, and
 * RegexPattern.
 * 
 * @stable ICU 2.0
 */
typedef struct UParseError {

    /**
     * The line on which the error occured.  If the parser uses this
     * field, it sets it to the line number of the source text line on
     * which the error appears, which will be be a value >= 1.  If the
     * parse does not support line numbers, the value will be <= 0.
     * @stable ICU 2.0
     */
    int32_t        line;

    /**
     * The character offset to the error.  If the line field is >= 1,
     * then this is the offset from the start of the line.  Otherwise,
     * this is the offset from the start of the text.  If the parser
     * does not support this field, it will have a value < 0.
     * @stable ICU 2.0
     */
    int32_t        offset;

    /**
     * Textual context before the error.  Null-terminated.  The empty
     * string if not supported by parser.
     * @stable ICU 2.0   
     */
    UChar          preContext[U_PARSE_CONTEXT_LEN];

    /**
     * The error itself and/or textual context after the error.
     * Null-terminated.  The empty string if not supported by parser.
     * @stable ICU 2.0   
     */
    UChar          postContext[U_PARSE_CONTEXT_LEN];

} UParseError;

#endif
