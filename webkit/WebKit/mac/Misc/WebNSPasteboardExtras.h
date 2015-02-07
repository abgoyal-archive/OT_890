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
 * Copyright (C) 2005, 2006, 2007 Apple Inc. All rights reserved.
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

@class DOMElement;
@class WebArchive;
@class WebHTMLView;

#ifdef __cplusplus
extern "C" {
#endif

extern NSString *WebURLPboardType;
extern NSString *WebURLNamePboardType;

@interface NSPasteboard (WebExtras)

// Returns the array of types that _web_writeURL:andTitle: handles.
+ (NSArray *)_web_writableTypesForURL;

// Returns the array of types that _web_writeImage handles.
+ (NSArray *)_web_writableTypesForImageIncludingArchive:(BOOL)hasArchive;

// Returns the array of drag types that _web_bestURL handles; note that the presence
// of one or more of these drag types on the pasteboard is not a guarantee that
// _web_bestURL will return a non-nil result.
+ (NSArray *)_web_dragTypesForURL;

// Finds the best URL from the data on the pasteboard, giving priority to http and https URLs
- (NSURL *)_web_bestURL;

// Writes the URL to the pasteboard with the passed types.
- (void)_web_writeURL:(NSURL *)URL andTitle:(NSString *)title types:(NSArray *)types;

// Sets the text on the NSFindPboard. Returns the new changeCount for the NSFindPboard.
+ (int)_web_setFindPasteboardString:(NSString *)string withOwner:(id)owner;

// Writes a file wrapper to the pasteboard as an RTFD attachment.
// NSRTFDPboardType must be declared on the pasteboard before calling this method.
- (void)_web_writeFileWrapperAsRTFDAttachment:(NSFileWrapper *)wrapper;

// Writes an image, URL and other optional types to the pasteboard.
- (void)_web_writeImage:(NSImage *)image 
                element:(DOMElement*)element
                    URL:(NSURL *)URL 
                  title:(NSString *)title
                archive:(WebArchive *)archive
                  types:(NSArray *)types
                 source:(WebHTMLView *)source;

- (id)_web_declareAndWriteDragImageForElement:(DOMElement *)element
                                       URL:(NSURL *)URL 
                                     title:(NSString *)title
                                   archive:(WebArchive *)archive
                                    source:(WebHTMLView *)source;

- (void)_web_writePromisedRTFDFromArchive:(WebArchive*)archive containsImage:(BOOL)containsImage;

@end

#ifdef __cplusplus
}
#endif
