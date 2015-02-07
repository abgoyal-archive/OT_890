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
 * Copyright (C) 2005 Apple Computer, Inc.  All rights reserved.
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

#import <Foundation/Foundation.h>

#if MAC_OS_X_VERSION_MAX_ALLOWED <= MAC_OS_X_VERSION_10_4
#define WebNSUInteger unsigned int
#else
#define WebNSUInteger NSUInteger
#endif

#if defined(BUILDING_ON_TIGER) || defined(BUILDING_ON_LEOPARD)
typedef int WebSourceId;
#else
typedef intptr_t WebSourceId;
#endif

@class WebView;
@class WebFrame;
@class WebScriptCallFrame;
@class WebScriptCallFramePrivate;
@class WebScriptObject;

extern NSString * const WebScriptErrorDomain;
extern NSString * const WebScriptErrorDescriptionKey;
extern NSString * const WebScriptErrorLineNumberKey;

enum {
    WebScriptGeneralErrorCode = -100
};

// WebScriptDebugDelegate messages

@interface NSObject (WebScriptDebugDelegate)

// some source was parsed, establishing a "source ID" (>= 0) for future reference
// this delegate method is deprecated, please switch to the new version below
- (void)webView:(WebView *)webView       didParseSource:(NSString *)source
                                                fromURL:(NSString *)url
                                               sourceId:(WebSourceId)sid
                                            forWebFrame:(WebFrame *)webFrame;

// some source was parsed, establishing a "source ID" (>= 0) for future reference
- (void)webView:(WebView *)webView       didParseSource:(NSString *)source
                                         baseLineNumber:(WebNSUInteger)lineNumber
                                                fromURL:(NSURL *)url
                                               sourceId:(WebSourceId)sid
                                            forWebFrame:(WebFrame *)webFrame;

// some source failed to parse
- (void)webView:(WebView *)webView  failedToParseSource:(NSString *)source
                                         baseLineNumber:(WebNSUInteger)lineNumber
                                                fromURL:(NSURL *)url
                                              withError:(NSError *)error
                                            forWebFrame:(WebFrame *)webFrame;

// just entered a stack frame (i.e. called a function, or started global scope)
- (void)webView:(WebView *)webView    didEnterCallFrame:(WebScriptCallFrame *)frame
                                               sourceId:(WebSourceId)sid
                                                   line:(int)lineno
                                            forWebFrame:(WebFrame *)webFrame;

// about to execute some code
- (void)webView:(WebView *)webView willExecuteStatement:(WebScriptCallFrame *)frame
                                               sourceId:(WebSourceId)sid
                                                   line:(int)lineno
                                            forWebFrame:(WebFrame *)webFrame;

// about to leave a stack frame (i.e. return from a function)
- (void)webView:(WebView *)webView   willLeaveCallFrame:(WebScriptCallFrame *)frame
                                               sourceId:(WebSourceId)sid
                                                   line:(int)lineno
                                            forWebFrame:(WebFrame *)webFrame;

// exception is being thrown
- (void)webView:(WebView *)webView   exceptionWasRaised:(WebScriptCallFrame *)frame
                                               sourceId:(WebSourceId)sid
                                                   line:(int)lineno
                                            forWebFrame:(WebFrame *)webFrame;
@end



// WebScriptCallFrame interface
//
// These objects are passed as arguments to the debug delegate.

@interface WebScriptCallFrame : NSObject
{
@private
    WebScriptCallFramePrivate* _private;
    id                         _userInfo;
}

// associate user info with frame
- (void)setUserInfo:(id)userInfo;

// retrieve user info
- (id)userInfo;

// get next frame on call stack (or nil if this is already the "global" frame)
- (WebScriptCallFrame *)caller;

// get array of WebScriptObjects for each scope (innermost first, last is always global object)
- (NSArray *)scopeChain;

// get name of function (if available) or nil
- (NSString *)functionName;

// get pending exception (if any) or nil
- (id)exception;

// evaluate a script (as if by "eval") in the context of this frame
- (id)evaluateWebScript:(NSString *)script;

@end

#undef WebNSUInteger

