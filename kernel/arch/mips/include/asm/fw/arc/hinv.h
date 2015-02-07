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
 * ARCS hardware/memory inventory/configuration and system ID definitions.
 */
#ifndef _ASM_ARC_HINV_H
#define _ASM_ARC_HINV_H

#include <asm/sgidefs.h>
#include <asm/fw/arc/types.h>

/* configuration query defines */
typedef enum configclass {
	SystemClass,
	ProcessorClass,
	CacheClass,
#ifndef	_NT_PROM
	MemoryClass,
	AdapterClass,
	ControllerClass,
	PeripheralClass
#else	/* _NT_PROM */
	AdapterClass,
	ControllerClass,
	PeripheralClass,
	MemoryClass
#endif	/* _NT_PROM */
} CONFIGCLASS;

typedef enum configtype {
	ARC,
	CPU,
	FPU,
	PrimaryICache,
	PrimaryDCache,
	SecondaryICache,
	SecondaryDCache,
	SecondaryCache,
#ifndef	_NT_PROM
	Memory,
#endif
	EISAAdapter,
	TCAdapter,
	SCSIAdapter,
	DTIAdapter,
	MultiFunctionAdapter,
	DiskController,
	TapeController,
	CDROMController,
	WORMController,
	SerialController,
	NetworkController,
	DisplayController,
	ParallelController,
	PointerController,
	KeyboardController,
	AudioController,
	OtherController,
	DiskPeripheral,
	FloppyDiskPeripheral,
	TapePeripheral,
	ModemPeripheral,
	MonitorPeripheral,
	PrinterPeripheral,
	PointerPeripheral,
	KeyboardPeripheral,
	TerminalPeripheral,
	LinePeripheral,
	NetworkPeripheral,
#ifdef	_NT_PROM
	Memory,
#endif
	OtherPeripheral,

	/* new stuff for IP30 */
	/* added without moving anything */
	/* except ANONYMOUS. */

	XTalkAdapter,
	PCIAdapter,
	GIOAdapter,
	TPUAdapter,

	Anonymous
} CONFIGTYPE;

typedef enum {
	Failed = 1,
	ReadOnly = 2,
	Removable = 4,
	ConsoleIn = 8,
	ConsoleOut = 16,
	Input = 32,
	Output = 64
} IDENTIFIERFLAG;

#ifndef NULL			/* for GetChild(NULL); */
#define	NULL	0
#endif

union key_u {
	struct {
#ifdef	_MIPSEB
		unsigned char  c_bsize;		/* block size in lines */
		unsigned char  c_lsize;		/* line size in bytes/tag */
		unsigned short c_size;		/* cache size in 4K pages */
#else	/* _MIPSEL */
		unsigned short c_size;		/* cache size in 4K pages */
		unsigned char  c_lsize;		/* line size in bytes/tag */
		unsigned char  c_bsize;		/* block size in lines */
#endif	/* _MIPSEL */
	} cache;
	ULONG FullKey;
};

#if _MIPS_SIM == _MIPS_SIM_ABI64
#define SGI_ARCS_VERS	64			/* sgi 64-bit version */
#define SGI_ARCS_REV	0			/* rev .00 */
#else
#define SGI_ARCS_VERS	1			/* first version */
#define SGI_ARCS_REV	10			/* rev .10, 3/04/92 */
#endif

typedef struct component {
	CONFIGCLASS	Class;
	CONFIGTYPE	Type;
	IDENTIFIERFLAG	Flags;
	USHORT		Version;
	USHORT		Revision;
	ULONG 		Key;
	ULONG		AffinityMask;
	ULONG		ConfigurationDataSize;
	ULONG		IdentifierLength;
	char		*Identifier;
} COMPONENT;

/* internal structure that holds pathname parsing data */
struct cfgdata {
	char *name;			/* full name */
	int minlen;			/* minimum length to match */
	CONFIGTYPE type;		/* type of token */
};

/* System ID */
typedef struct systemid {
	CHAR VendorId[8];
	CHAR ProductId[8];
} SYSTEMID;

/* memory query functions */
typedef enum memorytype {
	ExceptionBlock,
	SPBPage,			/* ARCS == SystemParameterBlock */
#ifndef	_NT_PROM
	FreeContiguous,
	FreeMemory,
	BadMemory,
	LoadedProgram,
	FirmwareTemporary,
	FirmwarePermanent
#else	/* _NT_PROM */
	FreeMemory,
	BadMemory,
	LoadedProgram,
	FirmwareTemporary,
	FirmwarePermanent,
	FreeContiguous
#endif	/* _NT_PROM */
} MEMORYTYPE;

typedef struct memorydescriptor {
	MEMORYTYPE	Type;
	LONG		BasePage;
	LONG		PageCount;
} MEMORYDESCRIPTOR;

#endif /* _ASM_ARC_HINV_H */
