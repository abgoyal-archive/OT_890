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
 * Workbit NinjaSCSI-32Bi/UDE PCI/CardBus SCSI Host Bus Adapter driver
 * I/O routine
 *
 * This software may be used and distributed according to the terms of
 * the GNU General Public License.
 */

#ifndef _NSP32_IO_H
#define _NSP32_IO_H

static inline void nsp32_write1(unsigned int  base,
				unsigned int  index,
				unsigned char val)
{
	outb(val, (base + index));
}

static inline unsigned char nsp32_read1(unsigned int base,
					unsigned int index)
{
	return inb(base + index);
}

static inline void nsp32_write2(unsigned int   base,
				unsigned int   index,
				unsigned short val)
{
	outw(val, (base + index));
}

static inline unsigned short nsp32_read2(unsigned int base,
					 unsigned int index)
{
	return inw(base + index);
}

static inline void nsp32_write4(unsigned int  base,
				unsigned int  index,
				unsigned long val)
{
	outl(val, (base + index));
}

static inline unsigned long nsp32_read4(unsigned int base,
					unsigned int index)
{
	return inl(base + index);
}

/*==============================================*/

static inline void nsp32_mmio_write1(unsigned long base,
				     unsigned int  index,
				     unsigned char val)
{
	volatile unsigned char *ptr;

	ptr = (unsigned char *)(base + NSP32_MMIO_OFFSET + index);

	writeb(val, ptr);
}

static inline unsigned char nsp32_mmio_read1(unsigned long base,
					     unsigned int  index)
{
	volatile unsigned char *ptr;

	ptr = (unsigned char *)(base + NSP32_MMIO_OFFSET + index);

	return readb(ptr);
}

static inline void nsp32_mmio_write2(unsigned long  base,
				     unsigned int   index,
				     unsigned short val)
{
	volatile unsigned short *ptr;

	ptr = (unsigned short *)(base + NSP32_MMIO_OFFSET + index);

	writew(cpu_to_le16(val), ptr);
}

static inline unsigned short nsp32_mmio_read2(unsigned long base,
					      unsigned int  index)
{
	volatile unsigned short *ptr;

	ptr = (unsigned short *)(base + NSP32_MMIO_OFFSET + index);

	return le16_to_cpu(readw(ptr));
}

static inline void nsp32_mmio_write4(unsigned long base,
				     unsigned int  index,
				     unsigned long val)
{
	volatile unsigned long *ptr;

	ptr = (unsigned long *)(base + NSP32_MMIO_OFFSET + index);

	writel(cpu_to_le32(val), ptr);
}

static inline unsigned long nsp32_mmio_read4(unsigned long base,
					     unsigned int  index)
{
	volatile unsigned long *ptr;

	ptr = (unsigned long *)(base + NSP32_MMIO_OFFSET + index);

	return le32_to_cpu(readl(ptr));
}

/*==============================================*/

static inline unsigned char nsp32_index_read1(unsigned int base,
					      unsigned int reg)
{
	outb(reg, base + INDEX_REG);
	return inb(base + DATA_REG_LOW);
}

static inline void nsp32_index_write1(unsigned int  base,
				      unsigned int  reg,
				      unsigned char val)
{
	outb(reg, base + INDEX_REG   );
	outb(val, base + DATA_REG_LOW);
}

static inline unsigned short nsp32_index_read2(unsigned int base,
					       unsigned int reg)
{
	outb(reg, base + INDEX_REG);
	return inw(base + DATA_REG_LOW);
}

static inline void nsp32_index_write2(unsigned int   base,
				      unsigned int   reg,
				      unsigned short val)
{
	outb(reg, base + INDEX_REG   );
	outw(val, base + DATA_REG_LOW);
}

static inline unsigned long nsp32_index_read4(unsigned int base,
					      unsigned int reg)
{
	unsigned long h,l;

	outb(reg, base + INDEX_REG);
	l = inw(base + DATA_REG_LOW);
	h = inw(base + DATA_REG_HI );

	return ((h << 16) | l);
}

static inline void nsp32_index_write4(unsigned int  base,
				      unsigned int  reg,
				      unsigned long val)
{
	unsigned long h,l;

	h = (val & 0xffff0000) >> 16;
	l = (val & 0x0000ffff) >>  0;

	outb(reg, base + INDEX_REG   );
	outw(l,   base + DATA_REG_LOW);
	outw(h,   base + DATA_REG_HI );
}

/*==============================================*/

static inline unsigned char nsp32_mmio_index_read1(unsigned long base,
						   unsigned int reg)
{
	volatile unsigned short *index_ptr, *data_ptr;

	index_ptr = (unsigned short *)(base + NSP32_MMIO_OFFSET + INDEX_REG);
	data_ptr  = (unsigned short *)(base + NSP32_MMIO_OFFSET + DATA_REG_LOW);

	writeb(reg, index_ptr);
	return readb(data_ptr);
}

static inline void nsp32_mmio_index_write1(unsigned long base,
					   unsigned int  reg,
					   unsigned char val)
{
	volatile unsigned short *index_ptr, *data_ptr;

	index_ptr = (unsigned short *)(base + NSP32_MMIO_OFFSET + INDEX_REG);
	data_ptr  = (unsigned short *)(base + NSP32_MMIO_OFFSET + DATA_REG_LOW);

	writeb(reg, index_ptr);
	writeb(val, data_ptr );
}

static inline unsigned short nsp32_mmio_index_read2(unsigned long base,
						    unsigned int  reg)
{
	volatile unsigned short *index_ptr, *data_ptr;

	index_ptr = (unsigned short *)(base + NSP32_MMIO_OFFSET + INDEX_REG);
	data_ptr  = (unsigned short *)(base + NSP32_MMIO_OFFSET + DATA_REG_LOW);

	writeb(reg, index_ptr);
	return le16_to_cpu(readw(data_ptr));
}

static inline void nsp32_mmio_index_write2(unsigned long  base,
					   unsigned int   reg,
					   unsigned short val)
{
	volatile unsigned short *index_ptr, *data_ptr;

	index_ptr = (unsigned short *)(base + NSP32_MMIO_OFFSET + INDEX_REG);
	data_ptr  = (unsigned short *)(base + NSP32_MMIO_OFFSET + DATA_REG_LOW);

	writeb(reg,              index_ptr);
	writew(cpu_to_le16(val), data_ptr );
}

/*==============================================*/

static inline void nsp32_multi_read4(unsigned int   base,
				     unsigned int   reg,
				     void          *buf,
				     unsigned long  count)
{
	insl(base + reg, buf, count);
}

static inline void nsp32_fifo_read(unsigned int   base,
				   void          *buf,
				   unsigned long  count)
{
	nsp32_multi_read4(base, FIFO_DATA_LOW, buf, count);
}

static inline void nsp32_multi_write4(unsigned int   base,
				      unsigned int   reg,
				      void          *buf,
				      unsigned long  count)
{
	outsl(base + reg, buf, count);
}

static inline void nsp32_fifo_write(unsigned int   base,
				    void          *buf,
				    unsigned long  count)
{
	nsp32_multi_write4(base, FIFO_DATA_LOW, buf, count);
}

#endif /* _NSP32_IO_H */
/* end */
