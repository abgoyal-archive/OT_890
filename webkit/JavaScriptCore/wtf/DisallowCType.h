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
 * Copyright (C) 2007 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WTF_DisallowCType_h
#define WTF_DisallowCType_h

// The behavior of many of the functions in the <ctype.h> header is dependent
// on the current locale. But almost all uses of these functions are for
// locale-independent, ASCII-specific purposes. In WebKit code we use our own
// ASCII-specific functions instead. This header makes sure we get a compile-time
// error if we use one of the <ctype.h> functions by accident.

#include <ctype.h>

#undef isalnum
#undef isalpha
#undef isascii
#undef isblank
#undef iscntrl
#undef isdigit
#undef isgraph
#undef islower
#undef isprint
#undef ispunct
#undef isspace
#undef isupper
#undef isxdigit
#undef toascii
#undef tolower
#undef toupper

#define isalnum WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define isalpha WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define isascii WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define isblank WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define iscntrl WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define isdigit WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define isgraph WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define islower WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define isprint WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define ispunct WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define isspace WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define isupper WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define isxdigit WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define toascii WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define tolower WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define toupper WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h

#endif
