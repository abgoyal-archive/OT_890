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
  NinjaSCSI I/O funtions 
      By: YOKOTA Hiroshi <yokota@netlab.is.tsukuba.ac.jp>
 
  This software may be used and distributed according to the terms of
  the GNU General Public License.

  */

/* $Id$ */

#ifndef __NSP_IO_H__
#define __NSP_IO_H__

static inline          void nsp_write(unsigned int base,
				      unsigned int index,
				      unsigned char val);
static inline unsigned char nsp_read(unsigned int base,
				     unsigned int index);
static inline          void nsp_index_write(unsigned int BaseAddr,
					    unsigned int Register,
					    unsigned char Value);
static inline unsigned char nsp_index_read(unsigned int BaseAddr,
					   unsigned int Register);

/*******************************************************************
 * Basic IO
 */

static inline void nsp_write(unsigned int  base,
			     unsigned int  index,
			     unsigned char val)
{
	outb(val, (base + index));
}

static inline unsigned char nsp_read(unsigned int base,
				     unsigned int index)
{
	return inb(base + index);
}


/**********************************************************************
 * Indexed IO
 */
static inline unsigned char nsp_index_read(unsigned int BaseAddr,
					   unsigned int Register)
{
	outb(Register, BaseAddr + INDEXREG);
	return inb(BaseAddr + DATAREG);
}

static inline void nsp_index_write(unsigned int  BaseAddr,
				   unsigned int  Register,
				   unsigned char Value)
{
	outb(Register, BaseAddr + INDEXREG);
	outb(Value, BaseAddr + DATAREG);
}

/*********************************************************************
 * fifo func
 */

/* read 8 bit FIFO */
static inline void nsp_multi_read_1(unsigned int   BaseAddr,
				    unsigned int   Register,
				    void          *buf,
				    unsigned long  count)
{
	insb(BaseAddr + Register, buf, count);
}

static inline void nsp_fifo8_read(unsigned int   base,
				  void          *buf,
				  unsigned long  count)
{
	/*nsp_dbg(NSP_DEBUG_DATA_IO, "buf=0x%p, count=0x%lx", buf, count);*/
	nsp_multi_read_1(base, FIFODATA, buf, count);
}

/*--------------------------------------------------------------*/

/* read 16 bit FIFO */
static inline void nsp_multi_read_2(unsigned int   BaseAddr,
				    unsigned int   Register,
				    void          *buf,
				    unsigned long  count)
{
	insw(BaseAddr + Register, buf, count);
}

static inline void nsp_fifo16_read(unsigned int   base,
				   void          *buf,
				   unsigned long  count)
{
	//nsp_dbg(NSP_DEBUG_DATA_IO, "buf=0x%p, count=0x%lx*2", buf, count);
	nsp_multi_read_2(base, FIFODATA, buf, count);
}

/*--------------------------------------------------------------*/

/* read 32bit FIFO */
static inline void nsp_multi_read_4(unsigned int   BaseAddr,
				    unsigned int   Register,
				    void          *buf,
				    unsigned long  count)
{
	insl(BaseAddr + Register, buf, count);
}

static inline void nsp_fifo32_read(unsigned int   base,
				   void          *buf,
				   unsigned long  count)
{
	//nsp_dbg(NSP_DEBUG_DATA_IO, "buf=0x%p, count=0x%lx*4", buf, count);
	nsp_multi_read_4(base, FIFODATA, buf, count);
}

/*----------------------------------------------------------*/

/* write 8bit FIFO */
static inline void nsp_multi_write_1(unsigned int   BaseAddr,
				     unsigned int   Register,
				     void          *buf,
				     unsigned long  count)
{
	outsb(BaseAddr + Register, buf, count);
}

static inline void nsp_fifo8_write(unsigned int   base,
				   void          *buf,
				   unsigned long  count)
{
	nsp_multi_write_1(base, FIFODATA, buf, count);
}

/*---------------------------------------------------------*/

/* write 16bit FIFO */
static inline void nsp_multi_write_2(unsigned int   BaseAddr,
				     unsigned int   Register,
				     void          *buf,
				     unsigned long  count)
{
	outsw(BaseAddr + Register, buf, count);
}

static inline void nsp_fifo16_write(unsigned int   base,
				    void          *buf,
				    unsigned long  count)
{
	nsp_multi_write_2(base, FIFODATA, buf, count);
}

/*---------------------------------------------------------*/

/* write 32bit FIFO */
static inline void nsp_multi_write_4(unsigned int   BaseAddr,
				     unsigned int   Register,
				     void          *buf,
				     unsigned long  count)
{
	outsl(BaseAddr + Register, buf, count);
}

static inline void nsp_fifo32_write(unsigned int   base,
				    void          *buf,
				    unsigned long  count)
{
	nsp_multi_write_4(base, FIFODATA, buf, count);
}


/*====================================================================*/

static inline void nsp_mmio_write(unsigned long base,
				  unsigned int  index,
				  unsigned char val)
{
	unsigned char *ptr = (unsigned char *)(base + NSP_MMIO_OFFSET + index);

	writeb(val, ptr);
}

static inline unsigned char nsp_mmio_read(unsigned long base,
					  unsigned int  index)
{
	unsigned char *ptr = (unsigned char *)(base + NSP_MMIO_OFFSET + index);

	return readb(ptr);
}

/*-----------*/

static inline unsigned char nsp_mmio_index_read(unsigned long base,
						unsigned int  reg)
{
	unsigned char *index_ptr = (unsigned char *)(base + NSP_MMIO_OFFSET + INDEXREG);
	unsigned char *data_ptr  = (unsigned char *)(base + NSP_MMIO_OFFSET + DATAREG);

	writeb((unsigned char)reg, index_ptr);
	return readb(data_ptr);
}

static inline void nsp_mmio_index_write(unsigned long base,
					unsigned int  reg,
					unsigned char val)
{
	unsigned char *index_ptr = (unsigned char *)(base + NSP_MMIO_OFFSET + INDEXREG);
	unsigned char *data_ptr  = (unsigned char *)(base + NSP_MMIO_OFFSET + DATAREG);

	writeb((unsigned char)reg, index_ptr);
	writeb(val,                data_ptr);
}

/* read 32bit FIFO */
static inline void nsp_mmio_multi_read_4(unsigned long  base,
					 unsigned int   Register,
					 void          *buf,
					 unsigned long  count)
{
	unsigned long *ptr = (unsigned long *)(base + Register);
	unsigned long *tmp = (unsigned long *)buf;
	int i;

	//nsp_dbg(NSP_DEBUG_DATA_IO, "base 0x%0lx ptr 0x%p",base,ptr);

	for (i = 0; i < count; i++) {
		*tmp = readl(ptr);
		//nsp_dbg(NSP_DEBUG_DATA_IO, "<%d,%p,%p,%lx>", i, ptr, tmp, *tmp);
		tmp++;
	}
}

static inline void nsp_mmio_fifo32_read(unsigned int   base,
					void          *buf,
					unsigned long  count)
{
	//nsp_dbg(NSP_DEBUG_DATA_IO, "buf=0x%p, count=0x%lx*4", buf, count);
	nsp_mmio_multi_read_4(base, FIFODATA, buf, count);
}

static inline void nsp_mmio_multi_write_4(unsigned long  base,
					  unsigned int   Register,
					  void          *buf,
					  unsigned long  count)
{
	unsigned long *ptr = (unsigned long *)(base + Register);
	unsigned long *tmp = (unsigned long *)buf;
	int i;

	//nsp_dbg(NSP_DEBUG_DATA_IO, "base 0x%0lx ptr 0x%p",base,ptr);

	for (i = 0; i < count; i++) {
		writel(*tmp, ptr);
		//nsp_dbg(NSP_DEBUG_DATA_IO, "<%d,%p,%p,%lx>", i, ptr, tmp, *tmp);
		tmp++;
	}
}

static inline void nsp_mmio_fifo32_write(unsigned int   base,
					 void          *buf,
					 unsigned long  count)
{
	//nsp_dbg(NSP_DEBUG_DATA_IO, "buf=0x%p, count=0x%lx*4", buf, count);
	nsp_mmio_multi_write_4(base, FIFODATA, buf, count);
}



#endif
/* end */
