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

/*************************************************************************
 * Defines and structure definitions for PCI BIOS Interface 
 *************************************************************************/
#define	PCIMAX  32		/* maximum number of PCI boards */


#define	PCI_VENDOR_DIGI		0x114F
#define	PCI_DEVICE_EPC		0x0002
#define	PCI_DEVICE_RIGHTSWITCH 0x0003  /* For testing */
#define	PCI_DEVICE_XEM		0x0004
#define	PCI_DEVICE_XR		0x0005
#define	PCI_DEVICE_CX		0x0006
#define	PCI_DEVICE_XRJ		0x0009   /* Jupiter boards with */
#define	PCI_DEVICE_EPCJ		0x000a   /* PLX 9060 chip for PCI  */


/*
 * On the PCI boards, there is no IO space allocated 
 * The I/O registers will be in the first 3 bytes of the   
 * upper 2MB of the 4MB memory space.  The board memory 
 * will be mapped into the low 2MB of the 4MB memory space 
 */

/* Potential location of PCI Bios from E0000 to FFFFF*/
#define PCI_BIOS_SIZE		0x00020000	

/* Size of Memory and I/O for PCI (4MB) */
#define PCI_RAM_SIZE		0x00400000	

/* Size of Memory (2MB) */
#define PCI_MEM_SIZE		0x00200000	

/* Offset of I/0 in Memory (2MB) */
#define PCI_IO_OFFSET 		0x00200000	

#define MEMOUTB(basemem, pnum, setmemval)  *(caddr_t)((basemem) + ( PCI_IO_OFFSET | pnum << 4 | pnum )) = (setmemval)
#define MEMINB(basemem, pnum)  *(caddr_t)((basemem) + (PCI_IO_OFFSET | pnum << 4 | pnum ))   /* for PCI I/O */





