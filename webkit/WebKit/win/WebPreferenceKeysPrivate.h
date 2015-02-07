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
 * Copyright (C) 2005, 2006, 2007, 2008, 2009 Apple Inc.  All rights reserved.
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
 * 3.  Neither the name of Apple Computer, Inc. ("Apple" nor the names of
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

// These are private because callers should be using the cover methods. They are in
// a Private (as opposed to Internal) header file because Safari uses some of them
// for managed preferences.
#define WebKitLogLevelPreferenceKey "WebKitLogLevel"
#define WebKitStandardFontPreferenceKey "WebKitStandardFont"
#define WebKitFixedFontPreferenceKey "WebKitFixedFont"
#define WebKitSerifFontPreferenceKey "WebKitSerifFont"
#define WebKitSansSerifFontPreferenceKey "WebKitSansSerifFont"
#define WebKitCursiveFontPreferenceKey "WebKitCursiveFont"
#define WebKitFantasyFontPreferenceKey "WebKitFantasyFont"
#define WebKitMinimumFontSizePreferenceKey "WebKitMinimumFontSize"
#define WebKitMinimumLogicalFontSizePreferenceKey "WebKitMinimumLogicalFontSize"
#define WebKitDefaultFontSizePreferenceKey "WebKitDefaultFontSize"
#define WebKitDefaultFixedFontSizePreferenceKey "WebKitDefaultFixedFontSize"
#define WebKitDefaultTextEncodingNamePreferenceKey "WebKitDefaultTextEncodingName"
#define WebKitUserStyleSheetEnabledPreferenceKey "WebKitUserStyleSheetEnabledPreferenceKey"
#define WebKitUserStyleSheetLocationPreferenceKey "WebKitUserStyleSheetLocationPreferenceKey"
#define WebKitShouldPrintBackgroundsPreferenceKey "WebKitShouldPrintBackgroundsPreferenceKey"
#define WebKitTextAreasAreResizablePreferenceKey "WebKitTextAreasAreResizable"
#define WebKitJavaEnabledPreferenceKey "WebKitJavaEnabled"
#define WebKitJavaScriptEnabledPreferenceKey "WebKitJavaScriptEnabled"
#define WebKitWebSecurityEnabledPreferenceKey "WebKitWebSecurityEnabled"
#define WebKitAllowUniversalAccessFromFileURLsPreferenceKey "WebKitAllowUniversalAccessFromFileURLs"
#define WebKitJavaScriptCanOpenWindowsAutomaticallyPreferenceKey "WebKitJavaScriptCanOpenWindowsAutomatically"
#define WebKitPluginsEnabledPreferenceKey "WebKitPluginsEnabled"
#define WebKitDatabasesEnabledPreferenceKey "WebKitDatabasesEnabled"
#define WebKitLocalStorageEnabledPreferenceKey "WebKitLocalStorageEnabled"
#define WebKitAllowAnimatedImagesPreferenceKey "WebKitAllowAnimatedImagesPreferenceKey"
#define WebKitAllowAnimatedImageLoopingPreferenceKey "WebKitAllowAnimatedImageLoopingPreferenceKey"
#define WebKitDisplayImagesKey "WebKitDisplayImagesKey"
#define WebKitBackForwardCacheExpirationIntervalKey "WebKitBackForwardCacheExpirationIntervalKey"
#define WebKitTabToLinksPreferenceKey "WebKitTabToLinksPreferenceKey"
#define WebKitPrivateBrowsingEnabledPreferenceKey "WebKitPrivateBrowsingEnabled"
#define WebKitIconDatabaseLocationKey "WebKitIconDatabaseLocation"
#define WebKitIconDatabaseEnabledPreferenceKey "WebKitIconDatabaseEnabled"
#define WebKitUsesPageCachePreferenceKey "WebKitUsesPageCachePreferenceKey"
#define WebKitCacheModelPreferenceKey "WebKitCacheModelPreferenceKey"
#define WebKitLocalStorageDatabasePathPreferenceKey "WebKitLocalStorageDatabasePath"

// These are private both because callers should be using the cover methods and because the
// cover methods themselves are private.
#define WebKitRespectStandardStyleKeyEquivalentsPreferenceKey "WebKitRespectStandardStyleKeyEquivalents"
#define WebKitShowsURLsInToolTipsPreferenceKey "WebKitShowsURLsInToolTips"
#define WebKitPDFDisplayModePreferenceKey "WebKitPDFDisplayMode"
#define WebKitPDFScaleFactorPreferenceKey "WebKitPDFScaleFactor"
#define WebKitEditableLinkBehaviorPreferenceKey "WebKitEditableLinkBehavior"

// Window display is throttled to 60 frames per second if WebKitThrottleWindowDisplayPreferenceKey
// is set to YES.  The window display throttle is OFF by default for compatibility with Mac OS X
// 10.4.6.
#define WebKitThrottleWindowDisplayPreferenceKey "WebKitThrottleWindowDisplay"

// CoreGraphics deferred updates are disabled if WebKitEnableCoalescedUpdatesPreferenceKey is set
// to NO, or has no value.  For compatibility with Mac OS X 10.4.6, deferred updates are OFF by
// default.
#define WebKitEnableDeferredUpdatesPreferenceKey "WebKitEnableDeferredUpdates"

// For debugging only.  Don't use these.
#define WebKitPageCacheSizePreferenceKey "WebKitPageCacheSizePreferenceKey"
#define WebKitObjectCacheSizePreferenceKey "WebKitObjectCacheSizePreferenceKey"

// From WebHistory.h
#define WebKitHistoryItemLimitKey "WebKitHistoryItemLimit" // default: "1000"
#define WebKitHistoryAgeInDaysLimitKey "WebKitHistoryAgeInDaysLimit" // default: "7"

// Windows-specific keys
#define WebKitFontSmoothingTypePreferenceKey "WebKitFontSmoothingType" // default: FontSmoothingTypeMedium (2)
#define WebKitFontSmoothingContrastPreferenceKey "WebKitFontSmoothingContrast" // default: "2"
#define WebKitCookieStorageAcceptPolicyPreferenceKey "WebKitCookieStorageAcceptPolicy" // default: WebKitCookieStorageAcceptPolicyOnlyFromMainDocumentDomain

#define WebContinuousSpellCheckingEnabledPreferenceKey "WebContinuousSpellCheckingEnabled" // default: false
#define WebGrammarCheckingEnabledPreferenceKey "WebGrammarCheckingEnabled" // default: false

#define AllowContinuousSpellCheckingPreferenceKey "AllowContinuousSpellCheckingPreferenceKey" // default: true

#define WebKitDOMPasteAllowedPreferenceKey "WebKitDOMPasteAllowedPreferenceKey" // default: false

#define WebKitApplicationChromeModePreferenceKey "WebKitApplicationChromeMode" // default: false

#define WebKitOfflineWebApplicationCacheEnabledPreferenceKey "WebKitOfflineWebApplicationCacheEnabled" // default: false

// If this key is present and has a value of true, we have already removed the default values from the user's preferences <rdar://problem/5214504>
#define WebKitDidMigrateDefaultSettingsFromSafari3BetaPreferenceKey "WebKitDidMigrateDefaultSettingsFromSafari3BetaPreferenceKey"

#define WebKitDidMigrateWebKitPreferencesToCFPreferencesPreferenceKey "WebKitDidMigrateWebKitPreferencesToCFPreferences"

#define WebKitDeveloperExtrasEnabledPreferenceKey "WebKitDeveloperExtras"
#define DisableWebKitDeveloperExtrasPreferenceKey "DisableWebKitDeveloperExtras"

#define WebKitAuthorAndUserStylesEnabledPreferenceKey "WebKitAuthorAndUserStylesEnabled"

#define WebKitPaintCustomScrollbarsPreferenceKey "WebKitPaintCustomScrollbars"

#define WebKitPaintNativeControlsPreferenceKey "WebKitPaintNativeControls"

#define WebKitZoomsTextOnlyPreferenceKey "WebKitZoomsTextOnly"

#define WebKitXSSAuditorEnabledPreferenceKey "WebKitXSSAuditorEnabled"

