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

/* Trivial implementations of basic i/o routines.  Assumes that all
   of the hard work has been done by ioremap and ioportmap, and that
   access to i/o space is linear.  */

/* This file may be included multiple times.  */

#if IO_CONCAT(__IO_PREFIX,trivial_io_bw)
__EXTERN_INLINE unsigned int
IO_CONCAT(__IO_PREFIX,ioread8)(void __iomem *a)
{
	return __kernel_ldbu(*(volatile u8 __force *)a);
}

__EXTERN_INLINE unsigned int
IO_CONCAT(__IO_PREFIX,ioread16)(void __iomem *a)
{
	return __kernel_ldwu(*(volatile u16 __force *)a);
}

__EXTERN_INLINE void
IO_CONCAT(__IO_PREFIX,iowrite8)(u8 b, void __iomem *a)
{
	__kernel_stb(b, *(volatile u8 __force *)a);
}

__EXTERN_INLINE void
IO_CONCAT(__IO_PREFIX,iowrite16)(u16 b, void __iomem *a)
{
	__kernel_stw(b, *(volatile u16 __force *)a);
}
#endif

#if IO_CONCAT(__IO_PREFIX,trivial_io_lq)
__EXTERN_INLINE unsigned int
IO_CONCAT(__IO_PREFIX,ioread32)(void __iomem *a)
{
	return *(volatile u32 __force *)a;
}

__EXTERN_INLINE void
IO_CONCAT(__IO_PREFIX,iowrite32)(u32 b, void __iomem *a)
{
	*(volatile u32 __force *)a = b;
}
#endif

#if IO_CONCAT(__IO_PREFIX,trivial_rw_bw) == 1
__EXTERN_INLINE u8
IO_CONCAT(__IO_PREFIX,readb)(const volatile void __iomem *a)
{
	return __kernel_ldbu(*(const volatile u8 __force *)a);
}

__EXTERN_INLINE u16
IO_CONCAT(__IO_PREFIX,readw)(const volatile void __iomem *a)
{
	return __kernel_ldwu(*(const volatile u16 __force *)a);
}

__EXTERN_INLINE void
IO_CONCAT(__IO_PREFIX,writeb)(u8 b, volatile void __iomem *a)
{
	__kernel_stb(b, *(volatile u8 __force *)a);
}

__EXTERN_INLINE void
IO_CONCAT(__IO_PREFIX,writew)(u16 b, volatile void __iomem *a)
{
	__kernel_stw(b, *(volatile u16 __force *)a);
}
#elif IO_CONCAT(__IO_PREFIX,trivial_rw_bw) == 2
__EXTERN_INLINE u8
IO_CONCAT(__IO_PREFIX,readb)(const volatile void __iomem *a)
{
	void __iomem *addr = (void __iomem *)a;
	return IO_CONCAT(__IO_PREFIX,ioread8)(addr);
}

__EXTERN_INLINE u16
IO_CONCAT(__IO_PREFIX,readw)(const volatile void __iomem *a)
{
	void __iomem *addr = (void __iomem *)a;
	return IO_CONCAT(__IO_PREFIX,ioread16)(addr);
}

__EXTERN_INLINE void
IO_CONCAT(__IO_PREFIX,writeb)(u8 b, volatile void __iomem *a)
{
	void __iomem *addr = (void __iomem *)a;
	IO_CONCAT(__IO_PREFIX,iowrite8)(b, addr);
}

__EXTERN_INLINE void
IO_CONCAT(__IO_PREFIX,writew)(u16 b, volatile void __iomem *a)
{
	void __iomem *addr = (void __iomem *)a;
	IO_CONCAT(__IO_PREFIX,iowrite16)(b, addr);
}
#endif

#if IO_CONCAT(__IO_PREFIX,trivial_rw_lq) == 1
__EXTERN_INLINE u32
IO_CONCAT(__IO_PREFIX,readl)(const volatile void __iomem *a)
{
	return *(const volatile u32 __force *)a;
}

__EXTERN_INLINE u64
IO_CONCAT(__IO_PREFIX,readq)(const volatile void __iomem *a)
{
	return *(const volatile u64 __force *)a;
}

__EXTERN_INLINE void
IO_CONCAT(__IO_PREFIX,writel)(u32 b, volatile void __iomem *a)
{
	*(volatile u32 __force *)a = b;
}

__EXTERN_INLINE void
IO_CONCAT(__IO_PREFIX,writeq)(u64 b, volatile void __iomem *a)
{
	*(volatile u64 __force *)a = b;
}
#endif

#if IO_CONCAT(__IO_PREFIX,trivial_iounmap)
__EXTERN_INLINE void IO_CONCAT(__IO_PREFIX,iounmap)(volatile void __iomem *a)
{
}
#endif
