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

#ifndef __ALPHA_STRING_H__
#define __ALPHA_STRING_H__

#ifdef __KERNEL__

/*
 * GCC of any recent vintage doesn't do stupid things with bcopy.
 * EGCS 1.1 knows all about expanding memcpy inline, others don't.
 *
 * Similarly for a memset with data = 0.
 */

#define __HAVE_ARCH_MEMCPY
extern void * memcpy(void *, const void *, size_t);
#define __HAVE_ARCH_MEMMOVE
extern void * memmove(void *, const void *, size_t);

/* For backward compatibility with modules.  Unused otherwise.  */
extern void * __memcpy(void *, const void *, size_t);

#define memcpy __builtin_memcpy

#define __HAVE_ARCH_MEMSET
extern void * __constant_c_memset(void *, unsigned long, size_t);
extern void * __memset(void *, int, size_t);
extern void * memset(void *, int, size_t);

#define memset(s, c, n)							    \
(__builtin_constant_p(c)						    \
 ? (__builtin_constant_p(n) && (c) == 0					    \
    ? __builtin_memset((s),0,(n)) 					    \
    : __constant_c_memset((s),0x0101010101010101UL*(unsigned char)(c),(n))) \
 : __memset((s),(c),(n)))

#define __HAVE_ARCH_STRCPY
extern char * strcpy(char *,const char *);
#define __HAVE_ARCH_STRNCPY
extern char * strncpy(char *, const char *, size_t);
#define __HAVE_ARCH_STRCAT
extern char * strcat(char *, const char *);
#define __HAVE_ARCH_STRNCAT
extern char * strncat(char *, const char *, size_t);
#define __HAVE_ARCH_STRCHR
extern char * strchr(const char *,int);
#define __HAVE_ARCH_STRRCHR
extern char * strrchr(const char *,int);
#define __HAVE_ARCH_STRLEN
extern size_t strlen(const char *);
#define __HAVE_ARCH_MEMCHR
extern void * memchr(const void *, int, size_t);

/* The following routine is like memset except that it writes 16-bit
   aligned values.  The DEST and COUNT parameters must be even for 
   correct operation.  */

#define __HAVE_ARCH_MEMSETW
extern void * __memsetw(void *dest, unsigned short, size_t count);

#define memsetw(s, c, n)						 \
(__builtin_constant_p(c)						 \
 ? __constant_c_memset((s),0x0001000100010001UL*(unsigned short)(c),(n)) \
 : __memsetw((s),(c),(n)))

#endif /* __KERNEL__ */

#endif /* __ALPHA_STRING_H__ */
